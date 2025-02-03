/**
 * @file initialize.cpp
 * @brief Исполняемый файл
 */

#include "initialize.h"

using namespace mavsdk;

#pragma region
/// @brief Поиск и подключение необходимого бортового контролера
/// @param mavsdk 
/// @param url URL соединения
/// @return Идентификатор подключенной системы
static std::shared_ptr<mavsdk::System> connect_system(Mavsdk& mavsdk,
    const std::string& url) {
    DEBUG_MSG("   Connecting to autopilot")
    /// Добавление соединения в список прослушиваемых
    ConnectionResult conn_res{ mavsdk.add_any_connection(url) };
    if (conn_res != ConnectionResult::Success)
        throw std::runtime_error("Connection failed: " + std::to_string((int)conn_res) + "\n");
    
    /// Поиск первого попавшегося автопилота в течении CONN_TIMEOUT
    size_t count{};
    while (count < initialize::CONN_TIMEOUT) {
        for (auto system : mavsdk.systems()) {
            if (!system->is_connected())
                continue;
            if (system->autopilot_type() == Autopilot::Px4)
                return system;
            if (system->autopilot_type() == Autopilot::ArduPilot)
                return system;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ++count;
    }
    throw std::runtime_error("Timeout waiting for system\n");
}
#pragma endregion

namespace initialize {
    /// @brief + запуск ассинхронной проверки наличия соединения
    /// @param mavsdk 
    /// @param url 
    Initializator::Initializator(Mavsdk& mavsdk,
        const std::string& url) noexcept :
        _system{ connect_system(mavsdk, url) },
        _telemetry{ _system }, _action{ _system },
        _offboard{ _system } {
        DEBUG_MSG("   Plugin initialization was successful")
        /// Проверка в асинхронном режиме, разорвалось ли соединение
        DEBUG_MSG("   Asinch. subscribe is_connected")
        _handle = _system->subscribe_is_connected([](bool conn_res) {
        if (!conn_res)
            throw std::runtime_error("System has disconnected\n");
        });
    }

    Initializator::~Initializator() noexcept {
        /// Отписка!
        if (_handle.valid())
            _system->unsubscribe_is_connected(_handle);
    }
}