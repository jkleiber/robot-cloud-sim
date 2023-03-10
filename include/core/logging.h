
#pragma once

#include <iomanip>
#include <iostream>
#include <limits>

#define LOG(msg) std::cout << msg << std::endl;

#define VERIFY(check)                                                          \
    if (!(check))                                                              \
    {                                                                          \
        std::cout << "[" << __FILE__ << ":" << __LINE__ << "] VERIFY("         \
                  << #check << ")\n";                                          \
        return false;                                                          \
    }
