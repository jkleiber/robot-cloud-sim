
#pragma once

#include <iostream>

#define LOG(msg) std::cout << msg << std::endl;

#define VERIFY(check)                                                          \
    if (!(check))                                                              \
    {                                                                          \
        std::cerr << "[" << __FILE__ << ":" << __LINE__ << "VERIFY(" << #check \
                  << ")\n";                                                    \
        return true;                                                           \
    }
