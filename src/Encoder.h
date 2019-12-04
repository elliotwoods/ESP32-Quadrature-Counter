#pragma once

#include "driver/pcnt.h"
#include <stdint.h>

class Encoder {
public:
	Encoder(gpio_num_t gpioPinA, gpio_num_t gpioPinB, pcnt_unit_t pcntUnit = PCNT_UNIT_0);
	void init();
	int16_t getValue();
protected:
	gpio_num_t gpioPinA, gpioPinB;
	pcnt_unit_t pcntUnit;
};