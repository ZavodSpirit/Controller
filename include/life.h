/**
 * @file life.h
 * @brief Траектория дрона
 */

#pragma once

/// @brief STL
#pragma region
#include <chrono>
#include <future>
#include <stdexcept>
#include <thread>
#include <vector>
#pragma endregion

/// @brief include
#pragma region
#include "debug.h"
#include "setting.h"
#pragma endregion

/// @brief MAVSDK
#pragma region
#include "mavsdk/mavsdk.h"
#include "mavsdk/plugin_base.h"
#include "mavsdk/plugins/action/action.h"
#include "mavsdk/plugins/telemetry/telemetry.h"
#include "mavsdk/plugins/offboard/offboard.h"
#pragma endregion

/// @brief Пространтсво имен траектории
namespace life {
    /// @brief Число попыток приземлить дрон
    inline const size_t& NUM_SEND_LAND{ setting::MAX_ATTEMP_SEND_LEND };
    /// @brief Время ожидания готовности дрона
    inline const size_t& READY_TIMEOUT{ setting::READY_TIMEOUT };
}

namespace life {
    /// @brief Жизненный цикл дрона
    class Life final {
    public:
        Life(mavsdk::Action&, mavsdk::Telemetry&, mavsdk::Offboard&) noexcept;
        Life(const Life&) = delete;
        Life(Life&&) = delete;

        virtual ~Life() noexcept;

        Life& operator=(const Life&) = delete;
        Life& operator=(Life&&) = delete;

        /// @brief Действия перед началом управления
        void startup();

        /// @brief Действия после управления
        void shutdown();
    private:
        mavsdk::Action& _action;
        mavsdk::Telemetry& _telemetry;
        mavsdk::Offboard& _offboard;

        /// @brief Для отписки
        mavsdk::Telemetry::FlightModeHandle _handle{};
    };
}