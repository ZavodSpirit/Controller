# Описание программы
Программа предназначена для исполнения на сопровождающем бортовом компьютере Ubuntu мультироторного дрона с целевым контролером PX4, ArduPilot.\Программа выполняет регулирование целевой высоты подъема дрона относительно точки старта.

Целевая высота удерживается 10с.\После этого происходит посадка в автоматическом режиме.

## Входные данные
На вход программа получает url адрес порта целевого контролера и уставку целевой высоты в метрах.\Входные данные передаются через аргументы командной строки.\Первый аргумент - url, второй - уставка.

## Сборка программы из исходных файлов
Сборка программы происходит с помощью утилиты CMake.\Перейдите в вашу папку временных файлов сборки и запустите команду: cmake "path_to_project".

Далее запустите вашу предпочтительную систему сборки в папке временных файлов, например выполните в консоле команду: make.

После этого запустите программу, выполните в консоле команду: ./controller "url" "setpoint_m".

## Включение режима отладки
Для включения режима "отладки" в папке include в файле debug.h включите определение: #include DEBUG.

Режим "отладки" выводит на консоль отладочную информацию и высоту дрона в метрах в процессе управления.\Отладочная информация отражает процесс исполнения программы.

## Описание версии
В данной версии тестового задания для автоматического управления дроном применяется библиотека MAVSDK и плагин Offboard с методом set_attitude.\В документациях на PX4 и Ardupilot написано, что они поддерживают в режиме Offboard управление тягой.

В прошлой версии тестового задания применялся плагин MavlinkPassthrow и команда SET_ATTITUDE_RATE.\Однако тестовые запуски выявили, что модель дрона не реагирует на отправку данных сообщений, а точнее MAVSDK их не отправляет на дрон.

## Симуляция работы
Работа утилиты верифицировалась на симуляции PX4 SITL с программой моделирования Gazebo.\В качестве модели выбран дрон 4 роторный gz_x500.

Также в процессе тестирования ПО использован QGroundControl.

Пример запуска симуляции: в отдельном терминале запустите QGroundControl.\В другом терминале перейдите в католог Firmware и выполните следующию кманду: make px4_sitl gz_x500

px4_sitl автоматически подключится к QGroundControl.\Есть возможность запустить Gazebo без графического отображения, смотри документацию.

## Файл настроек
Глобальные константы, определющие работу программы описаны в файле setting.h

## @todo
В документации на MAVSDK написано, что перед командой ARM необходимо проверить готовность всех систем дрона к полету методом health_all_ok.\В программе данная проверка реализована, но не подключена, так как симуляция при изначальном запуске не настроена.\Для ее настройки требуется колибровка датчиков, двигателей, акселерометров и прочее... (что долго).\Для подключения этой проверки смотри файл life.cpp метод startup.

Также в момент написания первой версии ПО автор не знал, что исключения из ассинхронных функций не ловятся.\Данный баг может вызвать системное прерывание.

Из за недостатка времени и опыта разработки алгоритмов управления нелинейными системами не удалось разработать адекватный алгоритм управления.\Поэтому при любом запуске на любом дроне он уйдет в автоколебания.

# Траектория
В данном разделе описана траектория нормального функционирования дрона под управлением данной утилиты.

1. Подключение соединения
2. Начало ассинхронной проверки наличия соединения.\Если соединение отсутсвует - системное прерывание.
3. Установка частоты приема телеметрических данных.
4. Начало в ассинхронном режиме приема телметрических данных.
5. @todo Проверка готовности к взлету.
6. ARM if DISARM.
7. Начало ассинхронной проверки изменения режима полета на Land.\Если далее режим полета изменится на Land - системное прерывание.
8. Установка режима Offboard.
9. Начало работы контроллера.\При работе контроллера если происходит отказ отправки управляющего воздействия подряд определенное количество раз - завершение работы.
10. Отмена пункта 7.
11. Отправка команды на Land.\Если команда не отправляется определенное количество раз - системное прерывание.
12. Начало ассинхронной проверки режима полета.\Если далее режим изменится с Land - системное прерывание.
13. Сопровождение дрона во время посадки.
14. После посадки - DISARM and KILL.

# Алгоритм управления
Используемый алгоритм управления описан в pdf файле репазитория algorithm.pdf.\В репазитории также находятся файлы MATLAB со скрптами разработки.