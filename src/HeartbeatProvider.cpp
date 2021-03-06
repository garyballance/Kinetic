/*
 * Copyright (c) 2014-2016 Western Digital Corporation or its affiliates <copyrightagent@wdc.com>
 * @author Gary Ballance <gary.ballance@wdc.com>
 *
 * SPDX-License-Identifier: GPL-2.0+
 * This file is part of Kinetic Advanced Object Store (KAOS).
 *
 * This program is free software: you may copy, redistribute and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA. <http://www.gnu.org/licenses/>
 */

/*
 * Include Files
 */
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <string>
#include <thread>
#include <sstream>
#include <exception>
#include "Logger.hpp"
#include "GlobalConfig.hpp"
#include "HeartbeatProvider.hpp"

/**
 * Initializes the Heartbeat Provider object.
 */
HeartbeatProvider::HeartbeatProvider()
    : m_active(false), m_thread(nullptr), m_socketFd(-1) {

    memset(&m_address, 0, sizeof(m_address));
    m_address.sin_family = AF_INET;
    m_address.sin_addr.s_addr = inet_addr(globalConfig.multicastIpAddress());
    m_address.sin_port = htons(globalConfig.multicastPort());
}

/**
 * Starts the heartbeat provider thread, which sends out a heartbeat message at a recurring interval
 * (around five seconds).
 *
 * @return  True if the thread was successfully created
 */
bool HeartbeatProvider::start() {
    try {
        m_active = true;
        m_thread = new std::thread(&HeartbeatProvider::run, this);
        return true;
    }
    catch (std::exception& e) {
        LOG(ERROR) << "Failed to create heartbeat provider thread: description=" << e.what();
        m_thread = nullptr;
        return false;
    }
}

/**
 * Opens a datagram socket for sending multicast messages.
 *
 * @throws  A runtime error if a socket could not be opened or configured successfully
 */
void HeartbeatProvider::openSocket() {
    const uint8_t MULTICAST_TTL(1);
    m_socketFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (m_socketFd == STATUS_FAILURE) {
        LOG(ERROR) << "Failed to create multicast socket: error code=" << errno << ", description=" << strerror(errno);
        throw std::runtime_error("Failed to create multicast socket");
    }

    if (setsockopt(m_socketFd, IPPROTO_IP, IP_MULTICAST_TTL, &MULTICAST_TTL, sizeof(MULTICAST_TTL)) == STATUS_FAILURE) {
        LOG(ERROR) << "Failed to set socket option IP_MULTICAST_TTL: error code=" << errno << ", description=" << strerror(errno);
        throw std::runtime_error("Failed to set socket option IP_MULTICAST_TTL");
    }
}

/**
 * Closes the datagram socket that was sending the multicast messages.
 */
void HeartbeatProvider::closeSocket() {
    /*
     * If the socket was successfully opened, close it (logging an error if one is encountered).
     */
    if (m_socketFd != STATUS_FAILURE) {
        if (close(m_socketFd) == STATUS_FAILURE) {
            LOG(ERROR) << "Failed to close socket: error code=" << errno << ", description=" << strerror(errno);
        }
    }
}

/**
 * Send a Kinetic heartbeat message to the multicast address.  Since some of the values in the
 * message can change, it is built from the latest values before sending.
 *
 * @throws  A runtime error if the message wasn't sent successfully
 */
void HeartbeatProvider::sendHeartbeatMessage() {
    std::stringstream stream;
    stream << "{"
           << "\"manufacturer\":\"" << globalConfig.vendor() << "\","
           << "\"model\":\"" << globalConfig.model() << "\","
           << "\"firmware_version\":\"" << globalConfig.version() << "\","
           << "\"serial_number\":\"" << globalConfig.serialNumber() << "\","
           << "\"world_wide_name\":\"" << globalConfig.worldWideName() << "\","
           << "\"protocol_version\":\"" << globalConfig.protocolVersion() << "\","
           << "\"port\":" << globalConfig.tcpPort() << ","
           << "\"tlsPort\":" << globalConfig.sslPort() << ","
           << "\"network_interfaces\":[";

    NetworkInterfaceList networkInterfaceList = globalConfig.networkInterfaceList();
    auto interfaceCount = networkInterfaceList.size();
    for (auto networkInterface : networkInterfaceList) {
        stream << "{\"name\":\"" << networkInterface->name() << "\","
               << "\"ipv4_addr\":\"" << networkInterface->ipv4() << "\","
               << "\"ipv6_addr\":\"" << networkInterface->ipv6() << "\","
               << "\"mac_addr\":\"" << networkInterface->macAddress() << "\"}";
        if (--interfaceCount > 0)
            stream << ",";
    }
    stream << "]}";
    std::string message = stream.str();

    if (sendto(m_socketFd, message.c_str(), message.length(), 0, (struct sockaddr*) &m_address, sizeof(m_address)) < 0)
        throw std::runtime_error("Failed to send multicast message");
}

/**
 * Run
 *
 * This is the function performed by the heartbeat provider thread.  It opens a multicast sockets
 * and sends heartbeat messages at the specified interval.
 */
void HeartbeatProvider::run() {
    while (m_active) {
        try {
            openSocket();
            while (m_active) {
                sendHeartbeatMessage();
                sleep(globalConfig.heartbeatSendInterval());
            }
        }
        catch (std::exception& ex) {
            LOG(ERROR) << "Heartbeat provider thread failure, description: " << ex.what();
            sleep(globalConfig.heartbeatConnectionRetryInterval());
        }
        closeSocket();
    }
}

/**
 * Stop
 *
 * Causes the heartbeat provider thread to terminate.
 */
void HeartbeatProvider::stop() {
    m_active = false;
}

/**
 * Wait
 *
 * Causes the caller to be blocked until the thread ends.
 */
void HeartbeatProvider::wait() {
    if (m_thread != nullptr)
        m_thread->join();
}
