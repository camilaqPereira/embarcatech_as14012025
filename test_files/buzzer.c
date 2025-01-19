#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define BUZZER_PIN 21

// Funcao de Inicializacao de LEDs e Buzzer
void init_buzzer()
{

    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_put(BUZZER_PIN, 0);
}

// Funcao de Configura o PWM no pino do buzzer com a frequência especificada
void set_buzzer_frequency(uint pin, uint frequency)
{
    // Obter o slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Configurar o pino como saída de PWM
    gpio_set_function(pin, GPIO_FUNC_PWM);

    // Configurar o PWM com frequência desejada
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (frequency * 4096)); // Calcula divisor do clock

    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, 0); // Inicializa com duty cycle 0 (sem som)
}

// Função para tocar o buzzer por um tempo especificado (em milissegundos) poderia ser em outras unidades
void play_buzzer()
{

    set_buzzer_frequency(BUZZER_PIN, 3350);
    pwm_set_gpio_level(BUZZER_PIN, 2048);
    sleep_ms(700);
    pwm_set_gpio_level(BUZZER_PIN, 0);
}

// init_buzzer(); Chame esta funcao no inicio da main para que inicialize o buzzer
