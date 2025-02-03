/**
 * @file main
 * @brief Главный исполняемый файл
 */
#include "pch.h"

/// @brief Интервал времени для такта контролера (mc)
#define INTERVAL 1 / setting::FREQUENCY * 1000

using namespace mavsdk;

/// @brief Функция обработки исключений
/// @param _err Исключение
void error(const std::exception& _err) noexcept {
    std::cerr << "Error while runtime:\n" << _err.what() << std::endl;
}

/// @brief Вывод информации о необходимом входе
void usage() {
    std::cout << "The first variable is the URL.\n" << 
        "The second is the relative setpoint in meters relative to the ground.\n";
}

/// @brief Конвейер выполнения регулирования
/// @param float Установившееся значение 
/// @param Regulator& Созданный контроллер
/// @param TelemetrySignal& Данные по высоте
/// @param ControlSignal& Управляющее воздействие 
void pipeline(float, const regulator::Regulator&,
    const telemetrysignal::TelemetrySignal&, controlsignal::ControlSignal&);

int main(int argc, char* argv[]) {
    DEBUG_MSG("Start program");
    float setpoint_m{};
    std::string url{};
    /// Обработка входных данных
    DEBUG_MSG("Input data parsing")
    try {
        input::Input input_parser{ argc, argv };
        setpoint_m = input_parser.getSetpoint();
        url = input_parser.getURL();
    }
    catch (const std::exception& err) {
        error(err);
        usage();
        return 1;
    }

    /// Полет
    Mavsdk mavsdk{ setting::API_CONF };
    DEBUG_MSG("Begin drone control...")
    try {
        /// Инициализация необходимых плагинов
        initialize::Initializator ini_system{ mavsdk, url };
        /// Создание сигналов для контроллера
        targetsinal::TargetSignal target{ setpoint_m };
        telemetrysignal::TelemetrySignal feedback{ ini_system.getTelemetry() };
        controlsignal::ControlSignal control_influence{ ini_system.getOffboard() };
        /// Контроллер
        regulator::Regulator regulator({ setting::K }, target, feedback,
            control_influence);
        
        /// Жизненный цикл дрона
        life::Life traectory{ ini_system.getAction(), ini_system.getTelemetry(),
            ini_system.getOffboard() };
        /// Предварительные операции перед регулированием
        traectory.startup();

        /// Регулирование
        DEBUG_MSG("Work of the controller...")
        pipeline(setpoint_m, regulator, feedback, control_influence);
        DEBUG_MSG("Work stopted")

        /// Посадка, DISARM
        traectory.shutdown();
    }
    catch (const std::exception& err) {
        error(err);
        return 1;
    }

    DEBUG_MSG("Successful completion of drone control")
    return 0;
}

void pipeline(float setpoint, const regulator::Regulator& regulator,
    const telemetrysignal::TelemetrySignal& altitude_m,
    controlsignal::ControlSignal& signal) {
    /// Верхняя граница зоны регулирования
    float upp_limit{ setpoint * (1 + setting::REGULATION_AREA) };
    /// Нижняя граница зоны регулирования
    float down_limit{ setpoint * (1 - setting::REGULATION_AREA) };
    /// Время вхождения в зону регулирования
    auto stop { std::clock() };
    
    size_t times{};
    while(true) {
        auto start{ std::clock() };
        /// Вычисление управляющего воздействия
        regulator.calcControl();
        /// Отправка управляющего воздействия
        signal.sendSignal();

        /// Проверка того, что высота вошла в зону регулирования
        float alt_m{ altitude_m.getSignal() };
        DEBUG_ALTITUDE(times, (size_t)setting::FREQUENCY, alt_m)
        if ((alt_m > upp_limit) || (alt_m < down_limit))
            stop = std::clock();
        else if ((std::clock() - stop) > setting::TIME * 1000)
            break;
        
        /// Настройка частоты отправления данных
        long delta{ INTERVAL - (std::clock() - start) };
        if (delta > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(delta));

        ++times;
    }
}