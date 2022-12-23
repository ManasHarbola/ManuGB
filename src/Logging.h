#pragma once
#include <iostream>

#define LOG(msg) (std::cout << __FILE__ << ":" << __LINE__ << "|" << __func__ << "|" <<  msg << std::endl)
