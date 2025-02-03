#include "initialize.h"

using namespace mavsdk;

#pragma region
/// @brief Поиск и подключение необходимого бортового контролера
/// @param mavsdk 
/// @param url 
/// @return Идентификатор подключенной системы
static std::shared_ptr<mavsdk::System> connect_system(Mavsdk& mavsdk,
    const std::string& url) {
    // Добавление соединения в список прослушиваемых
    ConnectionResult conn_res{ mavsdk.add_any_connection(url) };
    if (conn_res != ConnectionResult::Success)
        throw std::runtime_error("Connection failed: " + std::to_string((int)conn_res) + "\n");
    
    // Поиск первого попавшегося автопилота PX4 или Ardupilot
    //в течении CONN_TIMEOUT
    size_t count{};
    while (count < initialize::CONN_TIMEOUT) {
        auto it = mavsdk.systems().begin();
        auto end = mavsdk.systems().end();
        for (; it != end; ++it)
            switch ((*it)->autopilot_type()) {
                case Autopilot::Px4:
                case Autopilot::ArduPilot:
                    return *it;
                default:
                    break;
            }
        // Блокируем поток на 1 с, вдруг что то подключится.
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ++count;
    }
    throw std::runtime_error("Timeout for autopilote search\n");
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
        _info{ _system }, _mavlink { _system } {
        // Проверка в асинхронном режиме, разорвалось ли соединение
        _system->subscribe_is_connected([](bool conn_res) {
        if (!conn_res)
            throw std::runtime_error("System has disconnected\n");
        });
    }

    Initializator::~Initializator() noexcept {
        // Отписка!
        _system->subscribe_is_connected(nullptr);
    }
}