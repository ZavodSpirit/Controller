#pragma once

#include <memory>
#include <stdexcept>

#include "mavsdk/mavsdk.h"
#include "mavsdk/plugins/info/info.h"
#include "mavsdk/plugins/mavlink_passthrough/mavlink_passthrough.h"
#include "mavsdk/mavlink/common/mavlink.h"
#include "mavsdk/mavlink/common/mavlink_msg_attitude_target.h"

namespace controlsignal {
    /// @brief Число подряд идущих ошибок в отправке
    inline size_t MAX_COLLISIONS{ 10 };
}

namespace controlsignal {
    /// @brief Контейнер для отправки управляющего сигнала
    class ControlSignal final {
    public:
        ControlSignal(mavsdk::MavlinkPassthrough&, mavsdk::Info&) noexcept;
        ControlSignal(const ControlSignal&) = delete;
        ControlSignal(ControlSignal&&) noexcept = delete;

        virtual ~ControlSignal() noexcept;

        ControlSignal& operator=(const ControlSignal&) = delete;
        ControlSignal& operator=(ControlSignal&&) noexcept = delete;

        /// @brief Обновление контейнера тяги
        /// @param value значение thrust в условных единицах [-1, 1]
        void setSignal(float);

        /// @brief Отправка управляющего сигнала дрону
        void sendSignal();
    private:
        mavsdk::MavlinkPassthrough& _mavlink;
        mavsdk::Info& _info;
    
        /// @brief Сообщение для отправки
        mavlink_message_t _msg{};

        size_t _num_collisions{};
    };
}