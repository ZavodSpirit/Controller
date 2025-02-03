#pragma once

#include <stdexcept>
#include <string>

namespace input {
    /// @brief Парсинг входных данных программы
    class Input final {
    public:
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