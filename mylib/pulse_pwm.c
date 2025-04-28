#include "pulse_pwm.h"

#define PWM_PIN 7
#define PWM_FREQ_HZ 40000
#define NUM_CYCLES 8

volatile uint32_t pulse_count = 0;
uint slice_num;

void pwm_wrap_handler() {
    pwm_clear_irq(slice_num);

    pulse_count++;
    if (pulse_count >= NUM_CYCLES) {
        pwm_set_enabled(slice_num, false);              // zatrzymaj PWM
        gpio_set_function(PWM_PIN, GPIO_FUNC_SIO);      // przełącz pin na GPIO
        gpio_set_dir(PWM_PIN, GPIO_OUT);
        gpio_put(PWM_PIN, 1);                           // ustaw na HIGH (idle)
    }
}

void pulse_gen_start_pwm() {
    gpio_put(PWM_PIN, 1);                         // stan idle wysoki
    gpio_set_dir(PWM_PIN, GPIO_OUT);              // ustaw jako wyjście
    sleep_us(1);                                   // krótka pauza na propagację
    gpio_set_function(PWM_PIN, GPIO_FUNC_PWM);    // dopiero teraz przełącz na PWM

    slice_num = pwm_gpio_to_slice_num(PWM_PIN);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 125.0f);  // 125MHz / 125 = 1MHz
    pwm_config_set_wrap(&config, 25);        // 1MHz / 25 = 40kHz

    pwm_init(slice_num, &config, false);
    pwm_set_gpio_level(PWM_PIN, 13); // ~50% duty cycle

    pwm_clear_irq(slice_num);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_wrap_handler);
    irq_set_enabled(PWM_IRQ_WRAP, true);
    pwm_set_irq_enabled(slice_num, true);

    pulse_count = 0;
    pwm_set_enabled(slice_num, true);
}