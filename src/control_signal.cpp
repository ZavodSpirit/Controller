#include "control_signal.h"

using namespace mavsdk;

namespace controlsignal {
    ControlSignal::ControlSignal(MavlinkPassthrough& mavlink, Info& info) noexcept :
        _mavlink{ mavlink }, _info{ info } {
    }

    ControlSignal::~ControlSignal() noexcept {
    }

    void ControlSignal::setSignal(float value) {
        uint8_t this_sys_id{ _mavlink.get_our_sysid() };
        uint8_t this_comp_id{ _mavlink.get_our_compid() };
        uint8_t target_sys_id{ _mavlink.get_target_sysid() };
        uint8_t target_comp_id{ _mavlink.get_target_compid() };
        auto flight_info{ _info.get_flight_information() };
        if (flight_info.first != Info::Result::Success)
            throw std::runtime_error("Error with drone information\n");
        uint32_t time_boot_ms{ flight_info.second.time_boot_ms };
        uint8_t mask{ 0 };
        const float quarter[] { 1.0f, 0.0f, 0.0f, 0.0f };
        float body_roll_rate{ 0.0f };
        float body_pitch_rate{ 0.0f };
        float body_yaw_rate{ 0.0f };
        float thrust{ value };
        if (value >= 1)
            thrust = 1.0f;
        if (value <= -1)
            thrust = -1.0f;
        const float NED_thrust[] {0.0f, 0.0f, -1.0f};
        
        mavlink_msg_set_attitude_target_pack(this_sys_id, this_comp_id, &_msg,
            time_boot_ms, target_sys_id, target_comp_id, mask,
            quarter, body_roll_rate, body_pitch_rate,
            body_yaw_rate, thrust, NED_thrust);
    }

    void ControlSignal::sendSignal() {
        MavlinkPassthrough::Result res{ _mavlink.send_message(_msg) };
        if (res != MavlinkPassthrough::Result::Success) {
            ++_num_collisions;
        }
        else _num_collisions = 0;
        // Если подряд идущих ошибок в отправке MAX_COLLISIONS,
        // то система неуправляема
        if (_num_collisions >= MAX_COLLISIONS)
            throw std::runtime_error("The system is is out of control\n");
    }
}