/**
 * @file initialize.h
 * @brief Заголовочный файл класса инициализации подключения
 */

#pragma once

/// @brief STL
#pragma region
#include <chrono>
#include <memory>
#include <stdexcept>
#include <thread>
#pragma endregion

/// @brief include
#pragma region
#include "debug.h"
#include "setting.h"
#pragma endregion

/// @brief MAVSDK
#pragma region
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <mavsdk/plugins/offboard/offboard.h>
#pragma endregion

/// @brief Пространство имен инициализатора подключения
namespace initialize {
    /// @brief Время ожидания подключения к полетному контроллеру (c)
    inline const size_t& CONN_TIMEOUT{ setting::CONNECTION_TIMEOUT };
}

namespace initialize {
    /// @brief Инициализация необходимых плагинов, подключения
    class Initializator final {
    public:
        /// @brief При вызове конструктора происходит подключение к автопилоту
        /// @param mavsdk
        /// @param URL URL порта полетного контролера
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

        inline mavsdk::Offboard& getOffboard() noexcept {
            return _offboard;
        }

    private:
        std::shared_ptr<mavsdk::System> _system;
        
        /// @brief Плагин телеметрии
        mavsdk::Telemetry _telemetry;
        /// @brief Плагин команд
        mavsdk::Action _action;
        /// @brief Плагин дистанционного управления
        mavsdk::Offboard _offboard;

        /// @brief Для отписки
        mavsdk::System::IsConnectedHandle _handle{};
    };
}