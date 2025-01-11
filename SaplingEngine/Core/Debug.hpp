//
//  Debug.hpp
//  SaplingEngine
//

#pragma once

#include <iostream>

class Debug {
    public:
    
        /*
            * Logs a message to the console
            * @param message The message to print.
        */
        template<typename T>
        static void log(const T& message) 
        {
            std::cout << message << std::endl;
        }
};

