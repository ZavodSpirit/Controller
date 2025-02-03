/**
 * @file Debuh.h
 * @brief Заголовочный файл для включения режима отладки
 */
#pragma once

#include <iostream>

/// @brief Включи это для вывода отладочных сообщений
#define DEBUG

#ifdef DEBUG
/// @brief Вывод отладочной информации
#define DEBUG_MSG(str) std::cout << "[APP DEBUG]:   " << str << "\n";
#else
#define DEBUG_MSG(str) { }
#endif

#ifdef DEBUG
/// @brief Вывод информации о высоте каждую секунду
#define DEBUG_ALTITUDE(times, freq, altitude) { if (!(times % freq)) std::cout << "   " << altitude << " m\n"; }
#else
#define DEBUG_ALTITUDE(times, freq, altitude) { }
#endif