#include "target_signal.h"

namespace targetsinal {
    TargetSignal::TargetSignal(float value) noexcept {
        *_signal = value;
    };

    TargetSignal::~TargetSignal() noexcept {
    }
}