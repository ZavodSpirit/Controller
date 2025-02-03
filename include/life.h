#pragma once

#include <future>
#include <stdexcept>
#include <thread>

#include "mavsdk/mavsdk.h"
#include "mavsdk/plugin_base.h"
#include "mavsdk/plugins/action/action.h"
#include "mavsdk/plugins/telemetry/telemetry.h"

namespace life {
    /// @brief Число попыток приземлить дрон
    inline size_t NUM_SEND_LAND{ 10 };
}

namespace life {
    /// @brief Жизненный цикл дрона
    class Life final {
    public:
        Life(mavsdk::Action&, mavsdk::Telemetry&) noexcept;
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
    };
}