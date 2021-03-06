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
#pragma once
#ifndef CONNECTION_LISTENER_HPP
#define CONNECTION_LISTENER_HPP

/*
 * Include Files
 */
#include <stdint.h>
#include <unistd.h>
#include <thread>
#include "Logger.hpp"
#include "Server.hpp"
#include "TcpTransport.hpp"
#include "ListenerInterface.hpp"

/**
 * A stream agnostic connection listener for Kinetic clients.  Kinetic supports both encrypted and
 * clear text data streams for its connections.  This listener is used for both.  When a client is
 * discovered through the listening port, a connection is created with the proper data stream.
 */
template <class StreamType> class ConnectionListener : public ListenerInterface {
public:
    /**
     * Initializes the Connection Listener.
     *
     * @param   server  The server that will manage the new connections
     * @param   port    The port to listen on for new connections
     */
    explicit ConnectionListener(Server* const server, uint32_t port)
        : m_server(server), m_port(port), m_terminated(false), m_thread(nullptr) {
    }

    /**
     * Indicates if the listener has started.
     *
     * @return true if the listener has started, false otherwise
     */
    inline bool started() const {return (m_thread != nullptr);}

    /**
     * Starts the listener.
     *
     * @return true if the thread was successfully created
     */
    bool start() {
        try {
            if (!started() && !m_terminated) {
                m_listeningSocket = TcpTransport::serverSetup(m_port);
                m_thread = new std::thread(&ConnectionListener::listenerThread, this);
                return true;
            }
        }
        catch (std::exception& e) {
            LOG(ERROR) << "Failed to create TCP Transport thread: Description=" << e.what();
            m_thread = nullptr;
        }
        return false;
    }

    /**
     * Terminates the listener.
     */
    void stop() {
        if (started() && !m_terminated) {
            m_terminated = true;
            TcpTransport::serverShutdown(m_listeningSocket);
        }
    }

private:
    /**
     * Listens (on the listening port) for a client to attempt a connection. When an attempt is
     * made, a connection and stream object are created to service the client. If a critical error
     * is encountered, the listener waits for a period of time before attempting to resume
     * listening.
     */
    void listenerThread() {
        const uint32_t BACKOFF_PERIOD(60);
        while (!m_terminated) {
            try {
                ClientServerConnectionInfo clientServerConnectionInfo = TcpTransport::serverConnect(m_port, m_listeningSocket);
                StreamInterface* stream = static_cast<StreamInterface*>(new StreamType(clientServerConnectionInfo.socketDescriptor()));
                m_server->addConnection(new Connection(m_server, stream, clientServerConnectionInfo));
            }
            catch (std::exception& ex) {
                LOG(ERROR) << "Exception encounter: " << ex.what();
                sleep(BACKOFF_PERIOD);
            }
            catch (...) {
                LOG(ERROR) << "Unexpected exception encounter";
                sleep(BACKOFF_PERIOD);
            }
        }
    }

    /*
     * Private Data Members
     */
    Server* const   m_server;           //!< Server that will manager the new connections
    const uint32_t  m_port;             //!< Listening port
    bool            m_terminated;       //!< Indicates if listener is terminated
    int32_t         m_listeningSocket;  //!< File descriptor of socket listening on
    std::thread*    m_thread;           //!< Listening thread
};

#endif
