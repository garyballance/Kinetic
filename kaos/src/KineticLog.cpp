/*
 * Copyright (c) [2014 - 2016] Western Digital Technologies, Inc.
 *
 * This code is CONFIDENTIAL and a TRADE SECRET of Western Digital Technologies, Inc. and its
 * affiliates ("WD").  This code is protected under copyright laws as an unpublished work of WD.
 * Notice is for informational purposes only and does not imply publication.
 *
 * The receipt or possession of this code does not convey any rights to reproduce or disclose its
 * contents, or to manufacture, use, or sell anything that it may describe, in whole or in part,
 * without the specific written consent of WD.  Any reproduction or distribution of this code
 * without the express written consent of WD is strictly prohibited, is a violation of the copyright
 * laws, and may subject you to criminal prosecution.
 */

/*
 * Include Files
 */
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/statvfs.h>
#include <string>
#include "Logger.hpp"
#include "Server.hpp"
#include "Kinetic.pb.hpp"
#include "KineticLog.hpp"
#include "SystemConfig.hpp"
#include "MessageStatistics.hpp"

/**
 * Builds the response with the requested configuration information.
 *
 * @param   response    Pointer to the get log response to be populated
 */
void
KineticLog::getConfiguration(com::seagate::kinetic::proto::Command_GetLog* response) {

    com::seagate::kinetic::proto::Command_GetLog_Configuration* configuration(response->mutable_configuration());
    configuration->set_vendor(systemConfig.vendor());
    configuration->set_model(systemConfig.model());
    configuration->set_version(systemConfig.version());
    configuration->set_serialnumber(systemConfig.serialNumber());
    configuration->set_compilationdate(systemConfig.compilationDate());
    configuration->set_sourcehash(systemConfig.sourceHash());
    configuration->set_port(systemConfig.tcpPort());
    configuration->set_tlsport(systemConfig.sslPort());
    configuration->set_worldwidename(systemConfig.worldWideName());

    for (auto networkInterfaceSet : systemConfig.networkInterfaceMap()) {
        auto networkInterfaceConfig = networkInterfaceSet.second;
        ::com::seagate::kinetic::proto::Command_GetLog_Configuration_Interface* networkInterface = configuration->add_interface();
        networkInterface->set_name(networkInterfaceConfig->name());
        networkInterface->set_mac(networkInterfaceConfig->macAddress());
        networkInterface->set_ipv4address(networkInterfaceConfig->ipv4());
        networkInterface->set_ipv6address(networkInterfaceConfig->ipv6());
    }
}

/**
 * Build the response with the requested limit information.
 *
 * @param   response    Pointer to the get log response to be populated
 */
void
KineticLog::getLimits(com::seagate::kinetic::proto::Command_GetLog* response) {

    com::seagate::kinetic::proto::Command_GetLog_Limits* limits(response->mutable_limits());
    limits->set_maxkeysize(systemConfig.maxKeySize());
    limits->set_maxvaluesize(systemConfig.maxValueSize());
    limits->set_maxversionsize(systemConfig.maxVersionSize());
    limits->set_maxtagsize(systemConfig.maxTagSize());
    limits->set_maxconnections(systemConfig.maxConnections());
    limits->set_maxoutstandingreadrequests(systemConfig.maxOutstandingReadRequests());
    limits->set_maxoutstandingwriterequests(systemConfig.maxOutstandingWriteRequests());
    limits->set_maxmessagesize(systemConfig.maxMessageSize());
    limits->set_maxkeyrangecount(systemConfig.maxKeyRangeCount());
    limits->set_maxidentitycount(systemConfig.maxIdentityCount());
    limits->set_maxpinsize(systemConfig.maxPinSize());
    limits->set_maxoperationcountperbatch(systemConfig.maxOperationCountPerBatch());
    limits->set_maxbatchcountperdevice(systemConfig.maxBatchCountPerDevice());
}

/**
 * Builds the response with the requested statistics information.
 *
 * @param   response    Pointer to the get log response to be populated
 */
void
KineticLog::getStatistics(com::seagate::kinetic::proto::Command_GetLog* response) {
    server->messageStatistics().get(response);
}

/**
 * Builds the response with the requested capacity information, which is taken from the total and
 * available capacity of the data partition.
 *
 * @param   response    Pointer to the get log response to be populated
 */
void
KineticLog::getCapacities(com::seagate::kinetic::proto::Command_GetLog* response) {

    struct statvfs fiData;
    memset(&fiData, 0, sizeof(fiData));
    std::string mountPoint = systemConfig.databaseDirectory();

    float totalCapacity(0);
    float remainingCapacity(0);

    if ((statvfs(mountPoint.c_str(), &fiData)) != STATUS_SUCCESS) {
        LOG(ERROR) << "Failed to get file system information for mount point " << mountPoint << ", Error Code=" << errno << ", Description=" << strerror(errno);
    }
    else {
        totalCapacity = static_cast<float>(fiData.f_bsize) * static_cast<float>(fiData.f_blocks);
        remainingCapacity = static_cast<float>(fiData.f_bsize) * static_cast<float>(fiData.f_bfree);
    }

    com::seagate::kinetic::proto::Command_GetLog_Capacity* capacity(response->mutable_capacity());
    capacity->set_nominalcapacityinbytes(totalCapacity);
    capacity->set_portionfull((totalCapacity - remainingCapacity) / totalCapacity);
}

/**
 * Builds the response with the requested message information.
 *
 * @param   response    Pointer to the get log response to be populated
 */
void
KineticLog::getMessage(com::seagate::kinetic::proto::Command_GetLog* response) {

    std::string* message = response->mutable_messages();
    message->assign("System started");
}

/**
 * Builds the response with the requested device information.
 *
 * @param   response    Pointer to the get log response to be populated
 */
void
KineticLog::getDevice(const com::seagate::kinetic::proto::Command_GetLog& request, std::string& responseValue) {

    if (!request.has_device() || !request.device().has_name())
        throw MessageException(com::seagate::kinetic::proto::Command_Status_StatusCode_NOT_FOUND, "Missing device log name.");

#ifdef WORK_WITH_JAVA_SMOKE_TEST
    if (request.device().name() == "com.seagate.simulator:dummy")
        responseValue = "this is a sample log";
    else
#endif
        throw MessageException(com::seagate::kinetic::proto::Command_Status_StatusCode_NOT_FOUND,
                               "No device log for the specified name: " + request.device().name());
}

/**
 * Builds the response with the requested temperature information.
 *
 * @param   response    Pointer to the get log response to be populated
 */
void
KineticLog::getTemperatures(com::seagate::kinetic::proto::Command_GetLog* response) {

    com::seagate::kinetic::proto::Command_GetLog_Temperature* temperature(response->add_temperatures());
    temperature->set_name("HDA");
    temperature->set_current(75);
    temperature->set_target(25);
    temperature->set_minimum(5);
    temperature->set_maximum(100);
}

/**
 * Builds the response with the requested utilization information.
 *
 * @param   response    Pointer to the get log response to be populated
 */
void
KineticLog::getUtilizations(com::seagate::kinetic::proto::Command_GetLog* response) {

    com::seagate::kinetic::proto::Command_GetLog_Utilization* utilization(response->add_utilizations());
    utilization->set_name("HDA");
    utilization->set_value(.1);
#ifdef WORK_WITH_JAVA_SMOKE_TEST
    utilization = response->add_utilizations();
    utilization->set_name("EN0");
    utilization->set_value(.25);
    utilization = response->add_utilizations();
    utilization->set_name("EN1");
    utilization->set_value(.25);
    utilization = response->add_utilizations();
    utilization->set_name("CPU");
    utilization->set_value(.05);
#endif
}

