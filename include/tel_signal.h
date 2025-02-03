/**
 * @file tel_signal.h
 * @brief Заголовочный файл класса инкапсулятора телметрических данных
 * 
 * Телеметрические данные обновляются в ассинхронном режиме.
 * Контейнер содержит самые свежие пришедшие данные.
*/
#pragma once

/// @brief STL
#pragma region
#include <future>
#include <memory>
#include <stdexcept>
#pragma endregion

/// @brief include
#pragma region
#include "debug.h"
#include "setting.h"
#pragma endregion

/// @brief MAVSDK
#pragma region
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#pragma endregion

/// @brief Пространство имен контейнера телеметрических данных
namespace telemetrysignal {
    /// @brief Частота посыланий телеметрических данных
    inline const double& FREQ{ setting::FREQUENCY };
}

namespace telemetrysignal {
    /// @brief Контейнер телеметрических данных с датчиков дрона
    class TelemetrySignal final {
    public:
        TelemetrySignal(mavsdk::Telemetry&);
        TelemetrySignal(const TelemetrySignal&) = delete;
        TelemetrySignal(TelemetrySignal&&) noexcept = delete;

        virtual ~TelemetrySignal() noexcept;

        TelemetrySignal& operator=(const TelemetrySignal&) = delete;
        TelemetrySignal& operator=(TelemetrySignal&&) noexcept = delete;

        /// @brief Высота относительно точки старта
        /// @return h (m)
        inline float getSignal() const noexcept {
            return *_altitude_m;
        }
    private:
        mavsdk::Telemetry& _telemetry;

        /// @brief Высота (m) относительно стартовой точки
        std::shared_ptr<float> _altitude_m{ std::make_shared<float>() };

        /// @brief Для отписки
        mavsdk::Telemetry::PositionHandle _handle;
    };
}