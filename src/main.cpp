#include "pch.h"

/// @brief Передаточное число регулятора
#define K 0.0f

/// @brief Время удержания высоты (с)
#define TIME 10

using namespace mavsdk;

// Программа исполняется на бортовом устройстве управления
#define CONFIGURATION Mavsdk::Configuration{ ComponentType::CompanionComputer }

/// @brief Функция обработки исключений
/// @param _err исключение
void error(const std::exception& _err) noexcept {
    std::cerr << "Error while runtime:\n" << _err.what() << std::endl;
}

/// @brief Вывод информации о необходимом входе
void usage() {
    std::cout << "The first variable is the URL.\n" << 
        "The second is the relative setpoint in meters relative to the ground.\n";
}

/// @brief 
/// @param float Установившееся значение 
/// @param Regulator& Созданный контроллер
/// @param TelemetrySignal& Данные по высоте
/// @param ControlSignal& Управляющее воздействие 
void pipeline(float, const regulator::Regulator&,
    const telemetrysignal::TelemetrySignal&, controlsignal::ControlSignal&);

int main(int argc, char* argv[]) {
    float setpoint_m{};
    std::string url{};
    // Обработка входных данных
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

    // Полет
    Mavsdk mavsdk{ CONFIGURATION };
    try {
        // Инициализация необходимых плагинов
        initialize::Initializator ini_system{ mavsdk, url };
        // Создание сигналов для контроллера
        targetsinal::TargetSignal target{ setpoint_m };
        telemetrysignal::TelemetrySignal feedback{ ini_system.getTelemetry() };
        controlsignal::ControlSignal control_influence{ ini_system.getMavlink(),
            ini_system.getInfo() };
        // Контроллер
        regulator::Regulator regulator({ K }, target, feedback,
            control_influence);
        
        // Жизненный цикл дрона
        life::Life traectory{ ini_system.getAction(), ini_system.getTelemetry() };
        // Предварительные операции перед регулированием
        traectory.startup();

        // Регулирование
        pipeline(setpoint_m, regulator, feedback, control_influence);

        // Посадка, DISARM
        traectory.shutdown();
    }
    catch (const std::exception& err) {
        error(err);
        return 1;
    }
    return 0;
}

void pipeline(float setpoint, const regulator::Regulator& regulator,
    const telemetrysignal::TelemetrySignal& altitude_m,
    controlsignal::ControlSignal& signal) {
    // Верхняя граница зоны регулирования
    float upp_limit{ setpoint * 1.05f };
    // Нижняя граница зоны регулирования
    float down_limit{ setpoint * 0.95f };
    auto start { std::clock() };

    while(true) {
        regulator();
        signal.sendSignal();
        float alt_m{ altitude_m.getSignal() };
        // Проверка того, что высота вошла в зону регулирования
        if ((alt_m > upp_limit) || (alt_m < down_limit))
            start = std::clock();
        else if ((std::clock() - start) > TIME * 1000)
            break;
    }
}