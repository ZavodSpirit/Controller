#pragma once

#include "target_signal.h"
#include "tel_signal.h"
#include "control_signal.h"

namespace regulator {
    /// @brief Контроллер с обратной одной связью
    class Regulator {
    public:
        /// @brief Настроечные коэффициенты контроллера
        struct Settings {
            float K{};
        };

    public:
        Regulator(Settings, const targetsinal::TargetSignal&,
            const telemetrysignal::TelemetrySignal&,
            controlsignal::ControlSignal&) noexcept;
        Regulator(const Regulator&) = delete;
        Regulator(Regulator&&) = delete;

        virtual ~Regulator() noexcept;

        Regulator& operator=(const Regulator&) = delete;
        Regulator& operator=(Regulator&&) = delete;

        /// @brief Такт вычисления управляющего воздействия
        virtual void operator()() const;
    protected:
        Settings _config{};

        /// @brief Целевой сигнал регулирования
        const targetsinal::TargetSignal& _target;
        /// @brief Обратная связь
        const telemetrysignal::TelemetrySignal& _feedback;
        /// @brief Выходное управляющее воздействие
        controlsignal::ControlSignal& _control_influence;
    private:
    };
}