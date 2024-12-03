//
//  Debug.hpp
//  SaplingEngine
//

#pragma once

#include <iostream>

class Debug {
    public:
        static void log(const std::string& message) {
            std::cout << message << std::endl;
        }
};

