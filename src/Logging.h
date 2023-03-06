#pragma once
#include <iostream>
#include <sstream>
#include <iomanip>

#define LOG(msg) (std::cout << __FILE__ << ":" << __LINE__ << "|" << __func__ << "|" <<  msg << std::endl)

template<typename NumType>
std::string print_num(NumType n, std::ios_base& (&fmt)(std::ios_base&)) {
    std::stringstream ss;
    ss << "0x" << fmt << n;
    return ss.str();
}