/**
 * @file input.h
 * @brief Заголовочный файл парсера входных данных программы
 */

#pragma once

#include <stdexcept>
#include <string>

/// @brief Простанство имен парсера входных данных
namespace input {
    /// @brief Парсинг входных данных программы
    class Input final {
    public:
        /// @brief Аргументы main на вход
        Input(int, char*[]);

        Input(const Input&) = delete;
        Input(Input&&) noexcept = delete;

        virtual ~Input() noexcept;

        Input& operator=(const Input&) = delete;
        Input& operator=(Input&&) noexcept = delete;

        inline float getSetpoint() const noexcept {
            return _setpoint_m;
        };

        inline std::string getURL() const noexcept {
            return _url;
        };
    private:
        /// @brief Уставка (m)
        float _setpoint_m{};
        /// @brief URL соединения
        std::string _url{};
    };
}