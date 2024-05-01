////////////////////////
//
//  Created: Wed May 01 2024
//  File: tests.hpp
//
////////////////////////

#pragma once

#include <iostream>

#define UNUSED(X) ((void)X)

#define INIT_COUNTER(X, V) std::size_t X = V;
#define INIT_STR(X, V) std::string X = V;

#define TESTING_COUNTER_LABEL testing_counter
#define TOTAL_TESTING_COUNTER_LABEL total_testing_counter

#define CHECK(str, x) \
    { \
        std::cout << "[" << __FUNCTION__ << ":" << __LINE__ << "] "; \
        ++TOTAL_TESTING_COUNTER_LABEL; \
        if (!(x)) \
            std::cout << "Failed" << std::endl; \
        else { \
            std::cout << "OK" << std::endl; \
            ++TESTING_COUNTER_LABEL; \
        } \
    } \

#define INIT_TEST() \
    INIT_COUNTER(TESTING_COUNTER_LABEL, 0) \
    INIT_COUNTER(TOTAL_TESTING_COUNTER_LABEL, 0) \
    std::cout << "[" << __FUNCTION__ << "] Init testing" << std::endl; \

#define TERMINATE_TEST() \
    std::cout << "[" << __FUNCTION__ << "] Terminate testing: " << \
        TESTING_COUNTER_LABEL << "/" << TOTAL_TESTING_COUNTER_LABEL << std::endl;
