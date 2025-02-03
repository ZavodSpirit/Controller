#include "life.h"

using namespace mavsdk;

#pragma region
/// @brief Проверка готовности к боевому взводу
/// @param telemetry 
static void telemetry_check_ARM(Telemetry& telemetry) {
    if (telemetry.armed())
        return;

    auto prom{ std::make_shared<std::promise<void>>() };
    auto future{ prom->get_future() };
    telemetry.subscribe_health_all_ok(
    [prom](bool res) {
        if (res)
            prom->set_value();
    });
    
    future.get();
}

/// @brief Боевое взведение (ARM if DISARM)
/// @param action 
/// @param telemetry 
static void ARM(Action& action, Telemetry& telemetry) {
    if (telemetry.armed())
        return;

    auto prom{ std::make_shared<std::promise<Action::Result>>() };
    auto future{ prom->get_future() };
    
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
static void telemetry_check_stop(Telemetry& telemetry) {
    // 
    telemetry.subscribe_flight_mode([](Telemetry::FlightMode flight_mode) {
        // Если в момент управления дрон пытаются аврийно посадить - завершить программу
        switch(flight_mode) {
            case Telemetry::FlightMode::Land:
                std::runtime_error("Land or Hold commands were send\n");
            default:
                break;
        }
    });
}
#pragma endregion

namespace life {
    void Life::startup() {
        telemetry_check_ARM(_telemetry);

        ARM(_action, _telemetry);
        
        telemetry_check_stop(_telemetry);
        // Если сейчас будет послана команда на автоматическую посадку
        // - аварийное завершение
    }
}

#pragma region
/// @brief Прекращение слежение за аварийным завершением полета
/// @param telemetry 
static void telemetry_uncheck_stop(Telemetry& telemetry) {
    telemetry.subscribe_flight_mode(nullptr);
}

/// @brief Подача команды на автоматическое приземление
/// @param action
/// @warning Программа пытается послать команду на приземление 10 раз.
static void land(Action& action) {
    auto prom{ std::make_shared<std::promise<Action::Result>>() };
    auto future{ prom->get_future() };

    size_t i{};
    while(true) {
        // NUM_LAND_SEND раз пытаемся подать команду, если не получается - пошло все лесом.
        action.land_async([prom](Action::Result res) {
            prom->set_value(res);
        });

        Action::Result res{ future.get() };
        if (res == Action::Result::Success)
            return;
        if (i >= life::NUM_SEND_LAND)
            throw std::runtime_error("Land failed: " + std::to_string((int)res) + "\n");
    }
}

/// @brief Проверка на изменение режима полета с Land
/// @param telemetry 
/// @warning Если режим полета сменился с Land - завершение программы
/// (другая утилита пытается управлять дроном)
static void telemetry_check_land(Telemetry& telemetry) {
    telemetry.subscribe_flight_mode([](Telemetry::FlightMode flight_mode) {
        if (flight_mode == Telemetry::FlightMode::Land)
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

    // Отключение питания тотально принудительно!!!
    action.kill();
}
#pragma endregion

namespace life {
    void Life::shutdown() {
        // Прекращение проверки на аварийное завершение регулирования
        telemetry_uncheck_stop(_telemetry);

        land(_action);

        telemetry_check_land(_telemetry);

        check_land(_telemetry);

        DISARM(_action, _telemetry);
    }
}

namespace life {
    Life::Life(Action& action, Telemetry& telemetry) noexcept :
        _action{ action }, _telemetry{ telemetry } {
    }

    Life::~Life() noexcept {
        telemetry_uncheck_stop(_telemetry);
    }
}
