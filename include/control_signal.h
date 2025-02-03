/**
 * @file control_signal.h
 * @brief Контейнер для отпраки управляющего воздействия на дрон
 * 
 * Содержит метод для отправки воздействия.
 * В данной версии отправка управляющего воздействия происходит с помощью
 * плагина Offboard. Метод set_attitude
 */

#pragma once

/// @brief STL
#pragma region
#include <memory>
#include <vector>
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
#include <mavsdk/plugins/offboard/offboard.h>
#pragma endregion

/// @brief Пространство имен инкапсулятора управляющих воздействий
namespace controlsignal {
    /// @brief Число подряд идущих ошибок в отправке
    inline const size_t& MAX_COLLISIONS{ setting::MAX_CONTRLSEND_FAIL };
}

namespace controlsignal {
    /// @brief Контейнер для отправки управляющего сигнала
    class ControlSignal final {
    public:
        ControlSignal(mavsdk::Offboard&) noexcept;
        ControlSignal(const ControlSignal&) = delete;
        ControlSignal(ControlSignal&&) noexcept = delete;

        virtual ~ControlSignal() noexcept;

        ControlSignal& operator=(const ControlSignal&) = delete;
        ControlSignal& operator=(ControlSignal&&) noexcept = delete;

        /// @brief Обновление контейнера тяги
        /// @param value значение thrust в условных единицах [0, 1]
        void setSignal(float);

        /// @brief Отправка управляющего сигнала дрону
        void sendSignal();
    private:
        mavsdk::Offboard& _offboard;
        
        /// @brief Контейнер
        /// Крен, тангаж, рысканье - градусы. Тяга в долях [0, 1]
        mavsdk::Offboard::Attitude _signal{ 0.0f, 0.0f, 0.0f, 0.0f };

        size_t _count{};
    };
}