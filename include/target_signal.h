/**
 * @file target_signal.h
 * @brief Класс инкапсулятор целевой уставки
 */

#pragma once

/// STL
#include <memory>

/// include
#include "debug.h"

/// @brief Пространство имен целевой уставки регулировани
namespace targetsinal {
    /// @brief Контейнер для целевого сигнала регулирования
    class TargetSignal final {
    public:
        TargetSignal(float) noexcept;
        TargetSignal(const TargetSignal&) = delete;
        TargetSignal(TargetSignal&&) noexcept = delete;

        virtual ~TargetSignal() noexcept;

        TargetSignal& operator=(const TargetSignal&) = delete;
        TargetSignal& operator=(TargetSignal&&) noexcept = delete;

        inline void setSignal(float value) noexcept {
            *_signal = value;
        }

        inline float getSignal() const noexcept {
            return *_signal;
        }
    private:
        /// @brief Контейнер сигнала
        std::shared_ptr<float> _signal{ std::make_shared<float>() };
    };
}