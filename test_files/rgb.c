#include "pico/stdlib.h"

#define LED_PIN_R 13 //Pino do LED vermelho (Red)
#define LED_PIN_B 12 //Pino do LED azul (Blue)
#define LED_PIN_G 11 //Pino do LED verde (Green)

//Mapeamento das teclas do teclado
const teclas[4][4] = {
{ '1', '2', '3', 'A' },
{ '4', '5', '6', 'B' },
{ '7', '8', '9', 'C' },
{ '*', '0', '#', 'D' }
};

//Função usada para definir o estado dos LEDs (ligado ou desligado)
void ativaLeds(bool verme, bool verde, bool azul) {
    gpio_put(LED_PIN_R, verme);
    gpio_put(LED_PIN_G, verde);
    gpio_put(LED_PIN_B, azul);
}


void iniciaLeds() { // iniciaLeds ou Setup (para iniciar leds, teclado e buzzer)
    //inicializa os pinos dos três LEDs como saída
    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);
    gpio_init(LED_PIN_G);
    gpio_set_dir(LED_PIN_G, GPIO_OUT);
    gpio_init(LED_PIN_B);
    gpio_set_dir(LED_PIN_B, GPIO_OUT);
    /* inicializa o teclado (assim ou em função separa)
    gpio_init(--);
    gpio_set_dir(--, GPIO_OUT);   
    */
}

char entradaTeclado(){
    // função para ler a tecla pressionada que vai enviar o endereço da tecla ()
}

int main() {
    iniciaLeds();

    while (true) {
        // Recebe a tecla que foi pressionada no teclado
        char tecla = entradaTeclado();

        // Switch usado para mudar os 8 estados dos LEDs de acordo com a tecla que foi pressionada
        switch (tecla) {
            case '1':
                ativaLeds(1, 0, 0); // Apenas o LED vermelho ligado
                break;
            case '2':
                ativaLeds(0, 1, 0); // Apenas o LED verde ligado
                break;
            case '3':
                ativaLeds(0, 0, 1); // Apenas o LED azul ligado
                break;
            case '4':
                ativaLeds(1, 1, 0); // LEDs vermelho e verde ligados
                break;
            case '5':
                ativaLeds(1, 0, 1); // LEDs vermelho e azul ligados
                break;
            case '6':
                ativaLeds(0, 1, 1); // LEDs verde e azul ligados
                break;
            case '7':
                ativaLeds(1, 1, 1); // Todos os LEDs ligados
                break;
            default:
                ativaLeds(0, 0, 0); // Estado padrão: todos os LEDs estão desligados
                break;
        }

    }
}
