/**
 * @file regulator.cpp
 * @brief Исполняемый файл
 */

#include "regulator.h"

using namespace targetsinal;
using namespace telemetrysignal;
using namespace controlsignal;

namespace regulator {
    Regulator::Regulator(Settings settings, const TargetSignal& target,
        const TelemetrySignal& feedback, ControlSignal& influence) noexcept :
        _config{ settings }, _target{ target }, _feedback{ feedback },
        _control_influence{ influence } {
        DEBUG_MSG(" Сreating controller")
        DEBUG_MSG(" ~Сreating controller was successful")
    }

    Regulator::~Regulator() noexcept {
    }

    /// @todo Переделать алгоритм управления
    void Regulator::calcControl() const {
        _control_influence.setSignal(_config.K *
            (_target.getSignal() - _feedback.getSignal()));
    }
}