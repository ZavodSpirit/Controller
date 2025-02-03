/**
 * @file target_signal.cpp
 * @brief Исполняемый файл
 */

#include "target_signal.h"

namespace targetsinal {
    TargetSignal::TargetSignal(float value) noexcept {
        DEBUG_MSG("   Creatу setpoint container")
        *_signal = value;
    };

    TargetSignal::~TargetSignal() noexcept {
    }
}