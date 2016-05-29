/*
 * Copyright (c) 2014-2016 Western Digital Technologies, Inc. <copyrightagent@wdc.com>
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
#ifndef MESSAGE_STATISTICS_HPP
#define MESSAGE_STATISTICS_HPP

/*
 * Include Files
 */
#include <map>
#include <mutex>
#include "Kinetic.pb.hpp"
#include "KineticMessage.hpp"

/**
 * Maintains statistics on the messages processed.  For each message type, the number of time
 * the message type has been processed and the number of bytes transferred for the request and
 * response messages are maintained.  This information is reported through a Kinetic get log
 * request.
 */
class MessageStatistics {
    /**
     * Private Stats Record
     *
     * Maintains the count for number of times a message type has been processed and the number of
     * bytes transferred for the request and response messages.  There is one stats object for every
     * message type.
     */
    class Stats {
    public:
        /**
         * Constructor
         */
        Stats() : m_count(0), m_byteCount(0) {}

        /*
         * Public Accessors
         */
        inline int32_t  count() const {return m_count;}
        inline uint64_t byteCount() const {return m_byteCount;}

        /**
         * Increments the message count and add the size of the message to the total bytes.
         *
         * @param size The size of the message
         */
        void add(uint32_t size) {
            m_byteCount += size;
            m_count++;
        }

    private:
        /*
         * Private Data Members
         */
        uint32_t    m_count;        //!< Number of times message has been processed
        uint64_t    m_byteCount;    //!< Number of bytes received and send in processing of message
    };

    typedef std::map<com::seagate::kinetic::proto::Command_MessageType, Stats*> StatsMap;

public:
    /**
     * Message Statistics Constructor
     *
     * Initializes the tables that maintains the stats for each message type.
     */
    MessageStatistics()
        : m_statsTable({
        {com::seagate::kinetic::proto::Command_MessageType_PUT,           {new Stats()}},
        {com::seagate::kinetic::proto::Command_MessageType_GET,           {new Stats()}},
        {com::seagate::kinetic::proto::Command_MessageType_GETNEXT,       {new Stats()}},
        {com::seagate::kinetic::proto::Command_MessageType_GETPREVIOUS,   {new Stats()}},
        {com::seagate::kinetic::proto::Command_MessageType_GETVERSION,    {new Stats()}},
        {com::seagate::kinetic::proto::Command_MessageType_GETKEYRANGE,   {new Stats()}},
        {com::seagate::kinetic::proto::Command_MessageType_GETLOG,        {new Stats()}},
        {com::seagate::kinetic::proto::Command_MessageType_DELETE,        {new Stats()}},
        {com::seagate::kinetic::proto::Command_MessageType_SETUP,         {new Stats()}},
        {com::seagate::kinetic::proto::Command_MessageType_SECURITY,      {new Stats()}},
        {com::seagate::kinetic::proto::Command_MessageType_PEER2PEERPUSH, {new Stats()}},
        {com::seagate::kinetic::proto::Command_MessageType_NOOP,          {new Stats()}},
        {com::seagate::kinetic::proto::Command_MessageType_FLUSHALLDATA,  {new Stats()}},
        {com::seagate::kinetic::proto::Command_MessageType_PINOP,         {new Stats()}},
        {com::seagate::kinetic::proto::Command_MessageType_MEDIASCAN,     {new Stats()}},
        {com::seagate::kinetic::proto::Command_MessageType_MEDIAOPTIMIZE, {new Stats()}},
        {com::seagate::kinetic::proto::Command_MessageType_START_BATCH,   {new Stats()}},
        {com::seagate::kinetic::proto::Command_MessageType_END_BATCH,     {new Stats()}},
        {com::seagate::kinetic::proto::Command_MessageType_ABORT_BATCH,   {new Stats()}}
    }) {}

    /**
     * Update
     *
     * @param requestMessage    a processed request message
     * @param responseMessage   the associated response
     *
     * Updates the number times this operation has been performed and the number of bytes
     * transferred for its request and response.  Some operations have no response (such as a
     * batched put).  In those cases, there is still a response message, but it is unused and will
     * have a message size of zero.
     */
    void update(const KineticMessagePtr& requestMessage, const KineticMessagePtr& responseMessage) {
        std::unique_lock<std::mutex> scopedLock(m_mutex);
        StatsMap::iterator iter = m_statsTable.find(requestMessage->command()->header().messagetype());
        if (iter != m_statsTable.end()) {
            Stats* messageStats = iter->second;
            messageStats->add(requestMessage->totalSize() + responseMessage->totalSize());
        }
    }

    /**
     * Get
     *
     * @param response  pointer to where the statistics are to be set
     *
     * Sets the statistic values for all the message types.
     */
    void get(com::seagate::kinetic::proto::Command_GetLog* const response) {
        std::unique_lock<std::mutex> scopedLock(m_mutex);
        for (auto& messageStats : m_statsTable) {
            com::seagate::kinetic::proto::Command_GetLog_Statistics* statistics(response->add_statistics());
            statistics->set_messagetype(messageStats.first);
            statistics->set_count(messageStats.second->count());
            statistics->set_bytes(messageStats.second->byteCount());
        }
    }

private:
    /*
     * Private Data Members
     */
    std::mutex  m_mutex;        //!< Mutex to make the class thread safe
    StatsMap    m_statsTable;   //!< Statistics for all message types
};

#endif
