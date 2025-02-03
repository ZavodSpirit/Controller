#pragma once

#include <chrono>
#include <memory>
#include <stdexcept>
#include <thread>

#include "mavsdk/mavsdk.h"
#include "mavsdk/plugins/mavlink_passthrough/mavlink_passthrough.h"
#include "mavsdk/plugins/info/info.h"
#include "mavsdk/plugins/action/action.h"
#include "mavsdk/plugins/telemetry/telemetry.h"

namespace initialize {
    /// @brief Время ожидания подключения к полетному контроллеру (c)
    inline size_t CONN_TIMEOUT{ 5 };
}

namespace initialize {
    /// @brief Инициализация необходимых плагинов
    class Initializator final {
    public:
        /// @brief При вызове конструктора происходит подключение к автопилоту
        /// @param mavsdk
        /// @param URL 
        Initializator(mavsdk::Mavsdk&, const std::string&) noexcept;
        Initializator(const Initializator&) = delete;
        Initializator(Initializator&&) noexcept = delete;

        virtual ~Initializator() noexcept;

        Initializator& operator=(const Initializator&) = delete;
        Initializator& operator=(Initializator&&) = delete;

        inline mavsdk::Telemetry& getTelemetry() noexcept {
            return _telemetry;
        }

        inline mavsdk::Action& getAction() noexcept {
            return _action;
        }

        inline mavsdk::MavlinkPassthrough& getMavlink() noexcept {
            return _mavlink;
        }

        inline mavsdk::Info& getInfo() noexcept {
            return _info;
        }
    private:
        std::shared_ptr<mavsdk::System> _system;
        
        /// @brief Плагин телеметрии
        mavsdk::Telemetry _telemetry;
        /// @brief Плагин команд
        mavsdk::Action _action;
        /// @brief Плагин информации о системе
        mavsdk::Info _info;
        /// @brief Плагин отправки сообщений MAVLink 2
        mavsdk::MavlinkPassthrough _mavlink;
    };
}