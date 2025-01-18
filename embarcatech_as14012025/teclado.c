#include <stdio.h>
#include "pico/stdlib.h"

// Pinos configurados com base no diagrama JSON
const uint8_t row_pins[4] = {8, 7, 6, 5};  // Pinos das linhas do teclado
const uint8_t col_pins[4] = {4, 3, 2, 28}; // Pinos das colunas do teclado

// Matriz de teclas
const char keys[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Inicializa os pinos do teclado matricial
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

// Detecta a tecla pressionada
char keypad_get_key() {
    for (int row = 0; row < 4; row++) {
        gpio_put(row_pins[row], 0); // Ativar linha
        for (int col = 0; col < 4; col++) {
            if (gpio_get(col_pins[col]) == 0) { // Verifica se a coluna está ativa
                gpio_put(row_pins[row], 1); // Desativar linha
                return keys[row][col]; // Retorna a tecla pressionada
            }
        }
        gpio_put(row_pins[row], 1); // Desativar linha
    }
    return 0; // Nenhuma tecla pressionada
}

int main() {
    stdio_init_all(); // Inicializa a comunicação serial
    keypad_init();    // Inicializa o teclado matricial

    printf("Teclado Matricial 4x4 Iniciado\n");

    while (true) {
        char key = keypad_get_key();
        if (key) {
            printf("Tecla pressionada: %c\n", key);
            sleep_ms(200); // Debounce
        }
    }

    return 0;
}