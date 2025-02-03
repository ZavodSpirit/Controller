/**
 * @file regulator.h
 * @brief Заголовочный файл регулятора
 * 
 * calcControl() - метод вычисления управляющего воздействия
 */

#pragma once

/// @brief include
#include "debug.h"
#include "target_signal.h"
#include "tel_signal.h"
#include "control_signal.h"

/// @brief Пространство имен регулятора
namespace regulator {
    /// @brief Контроллер
    class Regulator {
    public:
        /// @brief Настроечные коэффициенты контроллера
        struct Settings {
            float K{};
        };

    public:
        /// @brief Конструктор
        /// @param Settings Настроечные коэффициенты
        /// @param TargetSignal& Цель регулирования 
        /// @param TelemetrySignal& Данные с датчиков
        /// @param ControlSignal& Управляющее воздействие (оно изменяется) 
        Regulator(Settings, const targetsinal::TargetSignal&,
            const telemetrysignal::TelemetrySignal&,
            controlsignal::ControlSignal&) noexcept;
        Regulator(const Regulator&) = delete;
        Regulator(Regulator&&) = delete;

        virtual ~Regulator() noexcept;

        Regulator& operator=(const Regulator&) = delete;
        Regulator& operator=(Regulator&&) = delete;

        /// @brief Такт вычисления управляющего воздействия
        /// Обновляется контейнер ControlSignal
        virtual void calcControl() const;
    protected:
        Settings _config{};

        /// @brief Целевой сигнал регулирования
        const targetsinal::TargetSignal& _target;
        /// @brief Данные с датчиков
        const telemetrysignal::TelemetrySignal& _feedback;
        /// @brief Выходное управляющее воздействие
        controlsignal::ControlSignal& _control_influence;
    private:
    };
}