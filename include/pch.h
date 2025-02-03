#pragma once

#include <chrono>
#include <iostream>
#include <exception>
#include <future>
#include <memory>
#include <stdexcept>
#include <thread>

#include "control_signal.h"
#include "initialize.h"
#include "input.h"
#include "life.h"
#include "regulator.h"
#include "target_signal.h"
#include "tel_signal.h"

#include "mavsdk/mavsdk.h"
#include "mavsdk/mavlink/common/mavlink.h"
#include "mavsdk/mavlink/common/mavlink_msg_set_attitude_target.h"
#include "mavsdk/plugins/action/action.h"
#include "mavsdk/plugins/mavlink_passthrough/mavlink_passthrough.h"
#include "mavsdk/plugins/telemetry/telemetry.h"