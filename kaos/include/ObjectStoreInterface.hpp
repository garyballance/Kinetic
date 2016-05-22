/*
 * Copyright (c) 2014-2016 Western Digital Technologies, Inc. <copyrightagent@wdc.com>
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
#ifndef OBJECT_STORE_INTERFACE_HPP
#define OBJECT_STORE_INTERFACE_HPP

/*
 * Include Files
 */
#include <stdint.h>
#include <string>
#include "Kinetic.pb.hpp"
#include "AccessControl.hpp"

// Need to replace with non-leveldb specific object
#include "leveldb/db.h"
typedef leveldb::WriteBatch BatchDescriptor;

/**
 * Defines the interface for Kinetic object store.
 */
class ObjectStoreInterface {
public:
    virtual void putEntry(const com::seagate::kinetic::proto::Command_KeyValue& params, const std::string& value) = 0;
    virtual void deleteEntry(const com::seagate::kinetic::proto::Command_KeyValue& params) = 0;
    virtual void getEntry(const std::string& key, std::string& returnValue, com::seagate::kinetic::proto::Command_KeyValue* returnMetadata) = 0;
    virtual void getNextEntry(const std::string& key, std::string& returnValue,
                              com::seagate::kinetic::proto::Command_KeyValue* returnMetadata) = 0;
    virtual void getPreviousEntry(const std::string& key, std::string& returnValue,
                                  com::seagate::kinetic::proto::Command_KeyValue* returnMetadata) = 0;
    virtual void getEntryMetadata(const std::string& key, bool versionOnly, com::seagate::kinetic::proto::Command_KeyValue* returnMetadata) = 0;
    virtual void getKeyRange(const com::seagate::kinetic::proto::Command_Range& params, AccessControlPtr& accessControl,
                             com::seagate::kinetic::proto::Command_Range* returnData) = 0;
    virtual void getKeyRangeReversed(const com::seagate::kinetic::proto::Command_Range& params, AccessControlPtr& accessControl,
                                     com::seagate::kinetic::proto::Command_Range* returnData) = 0;
    virtual bool open() = 0;
    virtual void close() = 0;
    virtual void erase() = 0;
    virtual void flush() = 0;
    virtual void optimizeMedia() = 0;

    // these will be eliminated (and put into a batch object assoicated with the specified database
    virtual void batchedPutEntry(BatchDescriptor& batch, const com::seagate::kinetic::proto::Command_KeyValue& params, const std::string& value) = 0;
    virtual void batchedDeleteEntry(BatchDescriptor& batch, const com::seagate::kinetic::proto::Command_KeyValue& params) = 0;
    virtual void batchCommit(BatchDescriptor& batch) = 0;
};

#endif
