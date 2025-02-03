/**
 * @file tel_signal.cpp
 * @brief Исполняемый файл
 */

#include "tel_signal.h"

using namespace mavsdk;

#pragma region
/// @brief Установка частоты посылки тел. данных с дрона
/// @param telemetry 
static void set_telemetry_rate(Telemetry& telemetry) {
    auto prom{ std::make_shared<std::promise<Telemetry::Result>>() };
    auto future{ prom->get_future() };

    telemetry.set_rate_position_async(telemetrysignal::FREQ, [prom](Telemetry::Result res) {
        prom->set_value(res);
    });

    Telemetry::Result res{future.get()};
    if (res != Telemetry::Result::Success)
        throw std::runtime_error("Setting rate failed: " + std::to_string((int)res) + "\n");
}

/// @brief Ассинхронное изменение контейнеров соответствующих
/// @param telemetry 
/// @param altitude_m 
static void telemetry_sub_pos(Telemetry& telemetry, Telemetry::PositionHandle& handle, float* altitude_m) noexcept {
    /// Ассинхронное изменение контейнера
    handle = telemetry.subscribe_position([altitude_m](Telemetry::Position position) {
        if (altitude_m)
            *altitude_m = position.relative_altitude_m;
    });
}
#pragma endregion

namespace telemetrysignal {
    TelemetrySignal::TelemetrySignal(Telemetry& telemetry) :
        _telemetry(telemetry) {
        DEBUG_MSG("   Create telemetry data container")

        DEBUG_MSG("   -Setting the frequency of telemetry data")
        set_telemetry_rate(_telemetry);
        
        DEBUG_MSG("   -Begin asynchronous verification of telemetry data")
        telemetry_sub_pos(_telemetry, _handle, _altitude_m.get());
    }

    TelemetrySignal::~TelemetrySignal() noexcept {
        /// Отписка!
        if (_handle.valid())
            _telemetry.unsubscribe_position(_handle);
    }
}