%% Параметры модели
m = 1;
g = 10;
Fmax = 3*m*g;

% Параметры saturation
upper = Fmax - m*g;
lover = -m*g;

%% Параметры контролера
K = 1;
T = 0.026;
%% Моделирование
out = sim("sim_test.slx");

%% Анализ данных
figure;
plot(out.setpoint, out.control);
title('Орган управления');
xlabel('input');
ylabel('output');
ylim([lover - 1, upper + 1]);

figure;
plot(out.h(:, 2), out.velocity);
title('Фазовый портрет');
xlabel('h');
ylabel("h'");

figure;
plot(out.h(:, 1), out.h(:, 2));
title('Переходная характеристика');
xlabel('t')
ylabel('h')