/**
 * @file pch.h
 * @brief Задумывался как файл предкомпилляции
 */

#pragma once

/// @brief STL
#pragma region
#include <chrono>
#include <iostream>
#include <exception>
#include <future>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#pragma endregion

/// @brief include
#pragma region
#include "control_signal.h"
#include "debug.h"
#include "initialize.h"
#include "input.h"
#include "life.h"
#include "regulator.h"
#include "setting.h"
#include "target_signal.h"
#include "tel_signal.h"
#pragma endregion

/// @brief MAVSDK
#pragma region
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <mavsdk/plugins/offboard/offboard.h>
#pragma endregion