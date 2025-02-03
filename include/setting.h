/**
 * @file setting.h
 * @brief Файл глобальных констант, определяющих работу приложения
 */

#pragma once

#include <mavsdk/mavsdk.h>

/// @brief Пространство имен глобальных констант
namespace setting {
    /// @brief Время удержания целевой высоты (непрерывное) (с)
    inline const double TIME{ 10 };
    /// @brief Величина зоны регулирования в долях
    inline const float REGULATION_AREA{ 0.05 };
}

namespace setting {
    /// @brief Частота обновления телеметрических данных и посыла управляющих сигналов (Гц)
    inline const double FREQUENCY{ 100.0 };
}

namespace setting {
    /// @brief Время ожидания подключения к полетному контролеру (с)
    inline const size_t CONNECTION_TIMEOUT{ 10 };
    /// @brief Время ожидания дрона к готовности (с) (health is ok)
    inline const size_t READY_TIMEOUT{ 60 };
    /// @brief Максимальное число подряд идущих коллизий отправки упр. сигнала
    inline const size_t MAX_CONTRLSEND_FAIL{ 10 };
    /// @brief Максимальное число попыток приземлить дрон
    inline const size_t MAX_ATTEMP_SEND_LEND{ 10 };
}

namespace setting {
    /// @brief Исполняемая программа предназначена для выполнения на сопровождающем компьютере
    inline const mavsdk::Mavsdk::Configuration
        API_CONF{ mavsdk::ComponentType::CompanionComputer };
}

namespace setting {
    /// @brief Конфигурация контролера высоты
    inline const double K{ 5.0 };
}