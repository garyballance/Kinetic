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
#pragma once
#ifndef CONNECTION_HPP
#define CONNECTION_HPP

/*
 * Include Files
 */
#include <stdint.h>
#include <map>
#include <list>
#include <mutex>
#include <thread>
#include <string>
#include <memory>
#include <atomic>
#include "Common.hpp"
#include "KineticMessage.hpp"
#include "StreamInterface.hpp"
#include "CommunicationsManager.hpp"
#include "ClientServerConnectionInfo.hpp"

/*
 * Incomplete Class Definition (to avoid circular dependencies)
 */
class Transaction;

/*
 * Batch List Pointer and Map.
 */
typedef std::shared_ptr<KineticMessageList> BatchListPtr;
typedef std::map<uint32_t, BatchListPtr> BatchListMap;

/**
 * Describes a single Kinetic network connection between the server and a client.
 */
class Connection {
public:

    /*
     * Constructor/Destructor
     */
    Connection(CommunicationsManager* communicationsManager, StreamInterface* stream, ClientServerConnectionInfo clientServerConnectionInfo);
    ~Connection();

    /*
     * Public Member Functions
     */
    bool sendResponse(KineticMessagePtr response);

    /*
     * Public Accessors
     */
    inline CommunicationsManager* communicationsManager() {return m_communicationsManager;}
    inline int64_t connectionId() {return m_connectionId;}
    inline int64_t previousSequence() {return m_previousSequence;}
    inline bool processedFirstRequest() {return m_processedFirstRequest;}
    inline void setPreviousSequence(int64_t previousSequence) {m_previousSequence = previousSequence;}
    inline void setProcessedFirstRequest(int64_t processedFirstRequest) {m_processedFirstRequest = processedFirstRequest;}
    inline uint32_t batchCount() {return m_batchListMap.size();}
    inline Security security() {return m_stream->security();}

    /*
     * Batch Functions
     */
    inline bool createBatchList(uint32_t batchId) {
        std::unique_lock<std::mutex> m_scopedLock(m_mutex);
        if (m_batchListMap.find(batchId) != m_batchListMap.end())
            return false;
        BatchListPtr batchList(new KineticMessageList());
        m_batchListMap[batchId] = batchList;
        return true;
    }

    inline BatchListPtr getBatchList(uint32_t batchId) {
        std::unique_lock<std::mutex> m_scopedLock(m_mutex);
        if (m_batchListMap.find(batchId) == m_batchListMap.end())  {
            BatchListPtr nullListPtr;
            return nullListPtr;
        }
        return m_batchListMap[batchId];
    }

    inline bool deleteBatchList(uint32_t batchId) {
        std::unique_lock<std::mutex> m_scopedLock(m_mutex);
        return (m_batchListMap.erase(batchId) > 0);
    }

private:

    /*
     * Private Member Functions
     */
    void receiveRequest(Transaction* transaction);
    void sendUnsolicitedStatusMessage();
    void run();

    /*
     * Private Data Member
     */
    CommunicationsManager*      m_communicationsManager;    //!< Manager of the connection
    Security                    m_security;                 //!< Connection's security (SSL or None)
    StreamInterface* const      m_stream;                   //!< Connection's I/O Stream (encrypted or clear text)
    const uint32_t              m_serverPort;               //!< Server's TCP port number
    const std::string           m_serverIpAddress;          //!< Server's IP address
    const uint32_t              m_clientPort;               //!< Client's TCP port number
    const std::string           m_clientIpAddress;          //!< Client's IP address
    std::thread* const          m_thread;                   //!< Thread that receives messages
    const int64_t               m_connectionId;             //!< Identification number for connection
    std::atomic<int64_t>        m_previousSequence;         //!< Last request message sequence number
    std::atomic_bool            m_processedFirstRequest;    //!< Indicates if the first request has been processed yet
    BatchListMap                m_batchListMap;             //!< Lists of batch commands indexed by batch ID
    std::mutex                  m_mutex;                    //!< Mutex used to make class thread safe

    DISALLOW_COPY_AND_ASSIGN(Connection);
};

#endif
