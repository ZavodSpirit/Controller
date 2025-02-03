/**
 * @file control_signal.cpp
 * @brief Исполняемый файл
 */

#include "control_signal.h"

using namespace mavsdk;

namespace controlsignal {
    ControlSignal::ControlSignal(Offboard& offboard) noexcept :
        _offboard{ offboard } {
        DEBUG_MSG("   Create container of control actions")
    }

    ControlSignal::~ControlSignal() noexcept {
    }

    void ControlSignal::setSignal(float value) {
        if (value >= 1.0f)
            value = 1.0f;
        if (value <= 0.0f)
            value = 0.0f;

        _signal.thrust_value = value;
    }

    void ControlSignal::sendSignal() {
        /// Считаем подряд идущие отказы в управлении
        auto res{ _offboard.set_attitude(_signal) };
        if (res != Offboard::Result::Success)
            ++_count;
        else
            _count = 0;
        if (_count > MAX_COLLISIONS)
            throw std::runtime_error("Object is uncontrollable");
    }
}