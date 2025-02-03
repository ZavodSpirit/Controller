/**
 * @file life.cpp
 * @brief Исполняемый файл
 */

#include "life.h"

using namespace mavsdk;

#pragma region
/// @brief Проверка готовности к боевому взводу
/// @param telemetry 
static void telemetry_check_ARM(Telemetry& telemetry) {
    if (telemetry.armed())
        return;

    /// Проверяем состояние дрона на готовность ко взлету
    size_t count{};
    while (count < life::READY_TIMEOUT) {
        if (telemetry.health_all_ok())
            return;
        ++count;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    throw std::runtime_error("Timeout for ready");
}

/// @brief Боевое взведение (ARM if DISARM)
/// @param action 
/// @param telemetry 
static void ARM(Action& action, Telemetry& telemetry) {
    if (telemetry.armed())
        return;

    auto prom{ std::make_shared<std::promise<Action::Result>>() };
    auto future{ prom->get_future() };
    
    /// Ассинхронное взведение
    action.arm_async([prom](Action::Result res) {
        prom->set_value(res);
    });
    
    Action::Result res{ future.get() };
    if (res != Action::Result::Success)
        throw std::runtime_error("ARM process failed: " + std::to_string((int)res) + "\n");
}

/// @brief Проверка того, было ли прислано дистанционная команда на завершение управления
/// @param telemetry
/// @warning Единственный режим на остановку управления - Land
static void telemetry_check_stop(Telemetry& telemetry, Telemetry::FlightModeHandle& handle) {
    handle = telemetry.subscribe_flight_mode([](Telemetry::FlightMode flight_mode) {
        /// Если в момент управления дрон пытаются аврийно посадить - завершить программу
        if (flight_mode == Telemetry::FlightMode::Land)
            throw std::runtime_error("Land command were send\n");
    });
}

/// @brief Начало режима offboard
/// @param offboard 
static void start_offboard(Offboard& offboard) {
    /// Крен, твнгаж, рысканье (градусы)
    /// Тяга (доли от максимальной 0...1)
    offboard.set_attitude({0.0f, 0.0f, 0.0f, 0.0f});
    auto res{ offboard.start() };
    if (res != Offboard::Result::Success) {
        throw std::runtime_error("Offboard mode cancelled " + std::to_string((int)res));
    }
}
#pragma endregion

namespace life {
    void Life::startup() {
        /// @todo Дрон в симуляции не подавал максимальные признаки жизни
        /*
        DEBUG_MSG("   Check life_is_ok...")
        telemetry_check_ARM(_telemetry);
        */
        DEBUG_MSG("   ARM...")
        ARM(_action, _telemetry);
        
        DEBUG_MSG("   Begin asynchronous Land verification")
        telemetry_check_stop(_telemetry, _handle);
        /// Если сейчас будет послана команда на автоматическую посадку
        /// - аварийное завершение

        DEBUG_MSG("   Start offboard")
        start_offboard(_offboard);
    }
}

#pragma region
/// @brief Завершение режима дистанционного управления
/// @param Offboard 
static void stop_offboard(Offboard& offboard) {
    offboard.stop();
}

/// @brief Прекращение слежение за аварийным завершением полета
/// @param telemetry 
static void telemetry_uncheck_stop(Telemetry& telemetry, Telemetry::FlightModeHandle& handle) {
    if (handle.valid())
        telemetry.unsubscribe_flight_mode(handle);
}

/// @brief Подача команды на автоматическое приземление
/// @param action
/// @warning Программа пытается послать команду на приземление 10 раз.
static void land(Action& action) {
    auto prom{ std::make_shared<std::promise<Action::Result>>() };
    auto future{ prom->get_future() };

    size_t i{};
    while(true) {
        /// NUM_LAND_SEND раз пытаемся подать команду, если не получается - пошло все лесом.
        action.land_async([prom](Action::Result res) {
            prom->set_value(res);
        });

        Action::Result res{ future.get() };
        if (res == Action::Result::Success)
            return;
        if (i >= life::NUM_SEND_LAND)
            throw std::runtime_error("Land failed: " + std::to_string((int)res) + "\n");
        ++i;
    }
}

/// @brief Проверка на изменение режима полета с Land
/// @param telemetry 
/// @warning Если режим полета сменился с Land - завершение программы
/// (другая утилита пытается управлять дроном)
static void telemetry_check_land(Telemetry& telemetry, Telemetry::FlightModeHandle& handle) {
    handle = telemetry.subscribe_flight_mode([](Telemetry::FlightMode flight_mode) {
        if (flight_mode != Telemetry::FlightMode::Land)
            throw std::runtime_error("The landing was cancelled\n");
    });
}

/// @brief Проверка и сопровождение дрона до конца посадки
/// @param telemetry 
static void check_land(Telemetry& telemetry) {
    // Блокируем поток исполнения до тех пор, пока он не сядет.
    // Частота обновления - 1 с.
    while (telemetry.in_air())
        std::this_thread::sleep_for(std::chrono::seconds(1));
}

/// @brief DISARM + KILL
/// @param action 
/// @param telemetry 
static void DISARM(Action& action, Telemetry& telemetry) {
    if (telemetry.armed())
        action.disarm();

    /// Отключение питания тотально принудительно!!!
    action.kill();
}
#pragma endregion

namespace life {
    void Life::shutdown() {
        DEBUG_MSG("   Stop offboard mode")
        stop_offboard(_offboard);
        
        // Прекращение проверки на аварийное завершение регулирования
        DEBUG_MSG("   Stop land verification")
        telemetry_uncheck_stop(_telemetry, _handle);

        DEBUG_MSG("   Land command send")
        land(_action);

        DEBUG_MSG("   Asynchronous check land mode")
        telemetry_check_land(_telemetry, _handle);

        DEBUG_MSG("   Land...")
        check_land(_telemetry);

        DEBUG_MSG("   DISARM")
        DISARM(_action, _telemetry);
    }
}

namespace life {
    Life::Life(Action& action, Telemetry& telemetry, Offboard& offboard) noexcept :
        _action{ action }, _telemetry{ telemetry }, _offboard{ offboard } {
    }

    Life::~Life() noexcept {
        telemetry_uncheck_stop(_telemetry, _handle);
    }
}
