//
//  Debug.hpp
//  ECS_Engine
//
//  Created by Tuukka Virtanen on 1/6/24.
//

#ifndef Debug_h
#define Debug_h

#include <iostream>

class Debug {
    public:
        static void log(const std::string& message) {
            std::cout << message << std::endl;
        }
};

#endif /* Debug_h */
