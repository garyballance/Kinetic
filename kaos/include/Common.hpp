/*
 * Copyright (c) [2014 - 2016] Western Digital Technologies, Inc. All rights reserved.
 */
#pragma once
#ifndef COMMON_HPP
#define COMMON_HPP

/*
 * Include Files
 */
#include <stdint.h>
#include <string>
#include <sstream>

/*
 * Commonly Used Constants
 */
const int32_t  STATUS_SUCCESS(0);
const int32_t  STATUS_FAILURE(-1);

/**
 * Converts a variable value to a string format
 *
 * @param T         type of variable whose value is to be converted
 * @param value     value to be converted into a string
 *
 * @return the specified value in string form
 */
template<typename T>
std::string toString(T value) {
    std::stringstream stream;
    stream << value;
    return stream.str();
}

/*
 * Prevents a class from have a copy or assignment constructor when placed in the private
 * declaration section of a class (from Google's style guide).
 */
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&); \
    void operator=(const TypeName&)

#endif
