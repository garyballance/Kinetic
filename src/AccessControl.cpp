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
#include <stdint.h>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include "Hmac.hpp"
#include "Kinetic.pb.hpp"
#include "AccessScope.hpp"
#include "AccessControl.hpp"

/*
 * Used Namespace Members
 */
using std::string;
using com::seagate::kinetic::proto::Command_Body;

/**
 * Initializes the Access Control object.
 *
 * @param   identity        Value of identity for new access control
 * @param   hmacKey         HMAC key of new access control
 * @param   hmacAlgorithm   HMAC algorithm of new access control
 * @param   scopeList       List of access scope for new access control
 */
AccessControl::AccessControl(int64_t identity, string hmacKey, HmacAlgorithm hmacAlgorithm, AccessScopeList scopeList)
    : m_identity(identity), m_hmacKey(hmacKey), m_hmacAlgorithm(hmacAlgorithm), m_scopeList(scopeList),
      m_readScopeList(getFilteredScopeList(Operation::READ)), m_rangeScopeList(getFilteredScopeList(Operation::RANGE)),
      m_tlsRequiredArray(getTlsRequiredArray()) {
}

/**
 * Determines if the specified operation is permitted to be performed according to the access
 * control settings. Kinetic's access control settings indicate what operations are permitted for
 * for a given user can also specify restrictions on possible key values (for key-based commands).
 *
 * @param   operation               The operation to be performed
 * @param   operationInvolvesKey    True if the operation uses a key
 * @param   commandBody             Body of the request (used to get key if it has one)
 *
 * @return  True if the operation has be performed, false otherwise
 */
bool AccessControl::operationPermitted(Operation operation, bool operationInvolvesKey, const Command_Body& commandBody) const {
    if (operation == Operation::INVALID)
        return true;

    for (AccessScope scope : m_scopeList) {
        if (scope.operationPermitted(operation)) {
            const string& requiredKeySubstring = scope.keySubstring();
            if (requiredKeySubstring.empty() || !operationInvolvesKey)
                return true;
            if (commandBody.keyvalue().has_key()) {
                if (requiredKeySubstring.empty())
                    return true;
                const string& key = commandBody.keyvalue().key();
                if ((scope.minimumKeySize() <= key.size())
                        && key.substr(scope.keySubstringOffset(), requiredKeySubstring.size()) == requiredKeySubstring) {
                    return true;
                }
            }
        }
    }
    return false;
}

/**
 * Builds a list that describes the permitted access for commands that perform the specified
 * operation (for example commands 'Get Next' and 'Get Previous' use the 'read' operation).  The
 * created list provides a fast access check for the specific operation (by not having to search
 * through access descriptors that don't apply to the operation being checked).
 *
 * @param   filterOperation     The operation which the scope list is to be built for
 *
 * @return  A list of scopes that only apply to the specified operation
 */
AccessScopeList AccessControl::getFilteredScopeList(Operation filterOperation) const {
    AccessScopeList filteredScopeList;
    for (auto scope : m_scopeList) {
        if (scope.operationPermitted(filterOperation)) {
            filteredScopeList.push_back(scope);
        }
    }
    return filteredScopeList;
}

/**
 * Builds a look-up-table (that is based on the access settings) to allow for a quick check if an
 * operation requires TLS/SSL.
 *
 * @return  An array that describes which operations (if any) require TLS/SSL)
 */
OperationSizedBoolArray AccessControl::getTlsRequiredArray() const {
    OperationSizedBoolArray tlsRequiredArray;
    tlsRequiredArray.fill(false);
    for (auto scope : m_scopeList) {
        if (scope.tlsRequired()) {
            for (uint32_t operation = 0; operation < NUMBER_OF_OPERATIONS; operation++) {
                if (scope.operationPermitted(UINT32_TO_OPERATION(operation))) {
                    tlsRequiredArray[operation] = true;
                }
            }
        }
    }
    return tlsRequiredArray;
}

/**
 * Determines if the operation with the specified key can be performed by checking the access
 * settings.
 *
 * @param   key         The key to use for operation
 * @param   scopeList   Operation specific list of scopes
 *
 * @return  True if the key can be used for the operation
 */
bool AccessControl::permissionToPerformOperation(const string& key, const AccessScopeList& scopeList) const {
    for (AccessScope scope : scopeList) {
        if (scope.keySubstring().empty())
            return true;
        if ((key.size() >= scope.minimumKeySize()) && (key.substr(scope.keySubstringOffset(), scope.keySubstring().size()) == scope.keySubstring()))
            return true;
    }
    return false;
}
