#include "Encoder.h"
// from https://github.com/espressif/esp-idf/blob/93a8603c545fb8e54741d6685146e2f3b874378d/examples/peripherals/pcnt/main/pcnt_example_main.c

#include "driver/periph_ctrl.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "driver/pcnt.h"
#include "esp_attr.h"
#include "esp_log.h"

#include <limits>

Encoder::Encoder(gpio_num_t gpioPinA, gpio_num_t gpioPinB, pcnt_unit_t pcntUnit) {
	this->gpioPinA = gpioPinA;
	this->gpioPinB = gpioPinB;
	this->pcntUnit = pcntUnit;
}

void Encoder::init() {
	//gpio_set_pull_mode(this->gpioPinA, GPIO_PULLUP_ONLY);
	//gpio_set_pull_mode(this->gpioPinB, GPIO_PULLUP_ONLY);

	/* Prepare configuration for the PCNT unit */
	pcnt_config_t pcnt_config;

	pcnt_config.pulse_gpio_num = this->gpioPinA;
	pcnt_config.ctrl_gpio_num = this->gpioPinB;
	pcnt_config.channel = PCNT_CHANNEL_0;
	pcnt_config.unit = pcntUnit;

	// What to do on the positive / negative edge of pulse input?
	pcnt_config.pos_mode = PCNT_COUNT_INC;   // Count up on the positive edge
	pcnt_config.neg_mode = PCNT_COUNT_DIS;   // Keep the counter value on the negative edge

	// What to do when control input is low or high?
	pcnt_config.lctrl_mode = PCNT_MODE_REVERSE; // Reverse counting direction if low
	pcnt_config.hctrl_mode = PCNT_MODE_KEEP;    // Keep the primary counter mode if high

	// Set the maximum and minimum limit values to watch
	pcnt_config.counter_h_lim = std::numeric_limits<int16_t>::max();
	pcnt_config.counter_l_lim = std::numeric_limits<int16_t>::min();  

	
	/* Initialize PCNT unit */
	pcnt_unit_config(&pcnt_config);

	/* Configure and enable the input filter */
	pcnt_set_filter_value(this->pcntUnit, 1023);
	pcnt_filter_enable(this->pcntUnit);

	/* Enable events on zero, maximum and minimum limit values */
	pcnt_event_enable(this->pcntUnit, PCNT_EVT_ZERO);
	pcnt_event_enable(this->pcntUnit, PCNT_EVT_H_LIM);
	pcnt_event_enable(this->pcntUnit, PCNT_EVT_L_LIM);

	/* Initialize PCNT's counter */
	pcnt_counter_pause(this->pcntUnit);
	pcnt_counter_clear(this->pcntUnit);

	/* Register ISR handler and enable interrupts for PCNT unit */
	//pcnt_isr_register(pcnt_example_intr_handler, NULL, 0, &user_isr_handle);
	//pcnt_intr_enable(PCNT_TEST_UNIT);

	/* Everything is set up, now go to counting */
	pcnt_counter_resume(this->pcntUnit);
}

int16_t Encoder::getValue() {
	int16_t value;
	return pcnt_get_counter_value(this->pcntUnit, &value);

	auto A = gpio_get_level(this->gpioPinA);
	auto B = gpio_get_level(this->gpioPinB);
	int16_t addition = (B << 1) + (A ^ B);

	return value * 4 + addition;
}