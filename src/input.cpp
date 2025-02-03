/**
 * @file input.cpp
 * @brief Исполняемый файл парсера входных данных
 */

#include "input.h"

namespace input {
    Input::Input(int argc, char* argv[]) {
        if (argc < 3)
            throw std::invalid_argument("Wrong inputs\n");
        
        _url = std::string{ argv[1] };
        std::string setp = std::string{ argv[2] };
        _setpoint_m = std::stof(setp);
    }

    Input::~Input() noexcept {
    }
}