#include "regulator.h"

using namespace targetsinal;
using namespace telemetrysignal;
using namespace controlsignal;

namespace regulator {
    Regulator::Regulator(Settings settings, const TargetSignal& target,
        const TelemetrySignal& feedback, ControlSignal& influence) noexcept :
        _target{ target }, _feedback{ feedback },
        _control_influence{ influence }, _config{ settings } {
    }

    Regulator::~Regulator() noexcept {
    }

    void Regulator::operator()() const {
        _control_influence.setSignal(_config.K *
            (_target.getSignal() - _feedback.getSignal()));
    }
}