#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"



/* Definicao de constantes*/
#define LED_RED_PIN 13    //Pino do LED vermelho (Red)
#define LED_BLUE_PIN 12   //Pino do LED azul (Blue)
#define LED_GREEN_PIN 11  //Pino do LED verde (Green)
#define BUZZER_PIN 21     // Pino do buzzer A

//Mapeamento das teclas do teclado
const char keys_mapping[4][4] = {
{ '1', '2', '3', 'A' },
{ '4', '5', '6', 'B' },
{ '7', '8', '9', 'C' },
{ '*', '0', '#', 'D' }
};

// Pinos do teclado matricial 4x4
const uint8_t row_pins[4] = {8, 7, 6, 5};  // Pinos das linhas do teclado
const uint8_t col_pins[4] = {4, 3, 2, 28}; // Pinos das colunas do teclado


/* Prototipo de funcoes*/
void leds_configure_output(bool, bool, bool);
void leds_init();
int leds_blink(uint);
void buzzer_init();
void buzzer_set_frequency(uint, uint);
void buzzer_play();
void keypad_init();
void keypad_get_pressed_key(char*);


/*
*   @brief Funcao para inicializacao do pinos do LED RGB
*/
void leds_init(){
    // Inicializacao LED vermelho
    gpio_init(LED_RED_PIN);
    gpio_set_dir(LED_RED_PIN, GPIO_OUT);
    gpio_put(LED_RED_PIN, 0);

    // Inicializacao LED azul
    gpio_init(LED_BLUE_PIN);
    gpio_set_dir(LED_BLUE_PIN, GPIO_OUT);
    gpio_put(LED_BLUE_PIN, 0);

    //Inicializacao LED verde
    gpio_init(LED_GREEN_PIN);
    gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);
    gpio_put(LED_GREEN_PIN, 0);
}

/*
*   @brief Funcao para ativar/desativar o LED RGB
*   @param red_active - indica se o LED vermelho deve ser ativado
*   @param green_active - indica se o LED verde deve ser ativado
*   @param blue_active - indica se o LED azul deve ser ativado
*/
void leds_configure_output(bool red_active, bool green_active, bool blue_active){
    gpio_put(LED_RED_PIN, red_active);
    gpio_put(LED_GREEN_PIN, green_active);
    gpio_put(LED_BLUE_PIN, blue_active);
}

/*
*   @brief Funcao utilizada par piscar o LED RGB cinco vezes de acordo com a cor selecionada
*   no padrão binário
*   @param led_sequency - sequencia binaria da cor selecionada
*/
int leds_blink(uint led_sequency){
    
    if (led_sequency > 7) return -1;

    for (int i = 0; i < 5; i++){
        leds_configure_output((led_sequency & 0x4) >> 2, (led_sequency & 0x2) >> 1, (led_sequency & 0x1));
        sleep_ms(200);
        leds_configure_output(0,0,0);
        sleep_ms(200);
    }
    return 0;
}

/*
*   @brief Funcao para inicializar o pino do buzzer A
*/
void buzzer_init(){
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_put(BUZZER_PIN, 0);
}

/*
*   @brief Funcao de configuracao do PWM no pino do buzzer com a frequência especificada
*   @param pin - pino do componente a ser modulado
*   @param frequency - frequencia de modulacao
*/
void buzzer_set_frequency(uint pin, uint frequency){
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

/*
*   @brief // Função para tocar o buzzer por um tempo especificado (em milissegundos)
*/
void buzzer_play(uint32_t duration){
    buzzer_set_frequency(BUZZER_PIN, 3350);
    pwm_set_gpio_level(BUZZER_PIN, 2048);
    sleep_ms(duration);
    pwm_set_gpio_level(BUZZER_PIN, 0);
}
/*
*   @brief Funcao para inicializacao dos pinos do teclado matricial
*/
void keypad_init() {
    for (int i = 0; i < 4; i++) {
        // Configura os pinos das linhas como saída
        gpio_init(row_pins[i]);
        gpio_set_dir(row_pins[i], GPIO_OUT);
        gpio_put(row_pins[i], 1); // Desativar linha

        // Configura os pinos das colunas como entrada 
        gpio_init(col_pins[i]);
        gpio_set_dir(col_pins[i], GPIO_IN);
        gpio_pull_up(col_pins[i]);
    }
}

/*
*   @brief Funcao para leitura do teclado matricial e deteccao do pressionamento de teclas
*   @param pressed_key ponteiro char para o retorno da tecla pressionada
*/

void keypad_get_pressed_key(char *pressed_key) {
    for (int row = 0; row < 4; row++) {
        gpio_put(row_pins[row], 0); // Ativar linha
        for (int col = 0; col < 4; col++) {
            if (gpio_get(col_pins[col]) == 0) { // Verifica se a coluna está ativa
                gpio_put(row_pins[row], 1); // Desativar linha
                *pressed_key = keys_mapping[row][col]; // Retorna a tecla pressionada
                return;
            }
        }
        gpio_put(row_pins[row], 1); // Desativar linha
    }
    *pressed_key = ' ';
    // Nenhuma tecla pressionada
}


int main()
{
    /*Incializacao de componentes*/
    leds_init();
    buzzer_init();
    keypad_init();

    stdio_init_all();

    /* Inicializacao de variaveis*/
    
    char pressed_key = ' ';
    uint key_ascii = 0;

    /*Loop principal*/
    while (true) {
        keypad_get_pressed_key(&pressed_key);
        
        if(pressed_key != ' '){
            printf("Pressed key: %c \n", pressed_key);

            switch (pressed_key){
            case '#':
                /*Acionamento do buzzer por 700ms*/
                buzzer_play(700);
                break;
            case '*':
            /* 2 ciclos de 700ms de acionamento do buzzer*/
                buzzer_play(700);
                sleep_ms(200);
                buzzer_play(700);
                break;
            case 'A':
                /* Led pisca 5 vezes na cor branca*/
                leds_blink(7);
                break;
            case 'B':
                /* Led pisca 5 vezes na cor amarela*/
                leds_blink(6);
                break;
            case 'C':
                /* Led pisca 5 vezes na cor magenta*/
                leds_blink(5);
                break;
            case 'D':
                /* Led pisca 5 vezes na cor ciano*/
                leds_blink(3);
                break;
            case '8':
                /* 3 ciclos de 500ms de acionamento do buzzer*/
                for (int i = 0; i < 3; i++){
                    buzzer_play(500);
                    sleep_ms(200);
                }
                break;
            case '9':
                break; 
            default:
                /*Acionamento as cores vermelho, verde e azul respectivo à contagem binária com 3 bits*/
                key_ascii = pressed_key - '0';
                leds_configure_output((key_ascii & 0x4) >> 2, (key_ascii & 0x2) >> 1, (key_ascii & 0x1));
                sleep_ms(700);
                break;
            }
        }else{
            leds_configure_output(0,0,0);
        }    
        
        sleep_ms(100); //Debounce
    }
}
