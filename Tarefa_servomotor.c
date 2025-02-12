#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <stdio.h>

#define PINO_SERVO 22        
#define PINO_LED_VERMELHO 13 
#define PINO_LED_VERDE 11     
#define PINO_LED_AZUL 12      

#define MIN_PULSE_WIDTH 500    // 0 graus (500µs)
#define MID_PULSE_WIDTH 1470   // 90 graus (1470µs)
#define MAX_PULSE_WIDTH 2400   // 180 graus (2400µs)
#define PULSE_INCREMENT 5      // Incremento de 5µs
#define DELAY_MS 10            // Atraso entre mudanças

// Configura o PWM do servo
void set_servo_pulse_width(uint pulse_width_us) {
    uint16_t wrap = 39062;  // Período de 20ms (50Hz) com clock de 125MHz e divisor de 64
    uint16_t level = (pulse_width_us * wrap) / 20000; // Converte microssegundos para nível PWM
    pwm_set_gpio_level(PINO_SERVO, level);
}

// Move o servo suavemente entre dois pontos
void move_servo_smoothly(uint start_pulse, uint end_pulse) {
    if (start_pulse < end_pulse) {
        for (uint pulse = start_pulse; pulse <= end_pulse; pulse += PULSE_INCREMENT) {
            set_servo_pulse_width(pulse);
            sleep_ms(DELAY_MS);
        }
    } else {
        for (uint pulse = start_pulse; pulse >= end_pulse; pulse -= PULSE_INCREMENT) {
            set_servo_pulse_width(pulse);
            sleep_ms(DELAY_MS);
        }
    }
}

// Configura cor do LED RGB usando PWM (valores de 0 a 255)
void set_led_color(uint8_t vermelho, uint8_t verde, uint8_t azul) {
    pwm_set_gpio_level(PINO_LED_VERMELHO, vermelho);
    pwm_set_gpio_level(PINO_LED_VERDE, verde);
    pwm_set_gpio_level(PINO_LED_AZUL, azul);
}

int main() {
    // Inicializa o GPIO do servo como PWM
    gpio_set_function(PINO_SERVO, GPIO_FUNC_PWM);
    uint slice_num_servo = pwm_gpio_to_slice_num(PINO_SERVO);

    // Configuração do PWM do servo
    pwm_config config_servo = pwm_get_default_config();
    pwm_config_set_clkdiv(&config_servo, 64.f); // Divisor para 50Hz
    pwm_config_set_wrap(&config_servo, 39062);  // Período de 20ms
    pwm_init(slice_num_servo, &config_servo, true);

    // Inicializa os pinos do LED RGB como PWM
    gpio_set_function(PINO_LED_VERMELHO, GPIO_FUNC_PWM);
    gpio_set_function(PINO_LED_VERDE, GPIO_FUNC_PWM);
    gpio_set_function(PINO_LED_AZUL, GPIO_FUNC_PWM);

    // Configuração do PWM dos LEDs RGB (máxima resolução de 8 bits)
    pwm_config config_led = pwm_get_default_config();
    pwm_config_set_clkdiv(&config_led, 4.f);
    pwm_config_set_wrap(&config_led, 255);
    pwm_init(pwm_gpio_to_slice_num(PINO_LED_VERMELHO), &config_led, true);
    pwm_init(pwm_gpio_to_slice_num(PINO_LED_VERDE), &config_led, true);
    pwm_init(pwm_gpio_to_slice_num(PINO_LED_AZUL), &config_led, true);

    // Movimento inicial do servo e LED RGB
    set_servo_pulse_width(MAX_PULSE_WIDTH);
    set_led_color(255, 0, 0); // LED Vermelho
    sleep_ms(5000);

    set_servo_pulse_width(MID_PULSE_WIDTH);
    set_led_color(0, 255, 0); // LED Verde
    sleep_ms(5000);

    set_servo_pulse_width(MIN_PULSE_WIDTH);
    set_led_color(0, 0, 255); // LED Azul
    sleep_ms(5000);

    // Movimento contínuo do servo e transição suave de cores no LED RGB
    while (true) {
        // Movimento do servo de 0° a 180° - LED de Azul para Vermelho
        move_servo_smoothly(MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
        for (int i = 0; i <= 255; i++) {
            set_led_color(i, 0, 255 - i); // Azul → Vermelho
            sleep_ms(DELAY_MS);
        }

        // Movimento do servo de 180° a 0° - LED de Vermelho para Azul
        move_servo_smoothly(MAX_PULSE_WIDTH, MIN_PULSE_WIDTH);
        for (int i = 0; i <= 255; i++) {
            set_led_color(255 - i, 0, i); // Vermelho → Azul
            sleep_ms(DELAY_MS);
        }
    }
}
