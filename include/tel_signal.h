#pragma once

#include <future>
#include <memory>
#include <stdexcept>

#include "mavsdk/mavsdk.h"
#include "mavsdk/plugins/telemetry/telemetry.h"

namespace telemetrysignal {
    /// @brief Частота посыланий телеметрических данных
    inline double FREQ{ 100.0 };
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
    };
}