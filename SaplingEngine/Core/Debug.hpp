//
//  Debug.hpp
//  ECS_Engine
//
//  Created by Tuukka Virtanen on 1/6/24.
//

#pragma once

#include <iostream>

class Debug {
    public:
        static void log(const std::string& message) {
            std::cout << message << std::endl;
        }
};

