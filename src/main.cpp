#include <Arduino.h>
#include <U8g2lib.h>
#include <vector>
#include <string>
#include <iostream>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include "Encoder.h"

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

Encoder encoder(GPIO_NUM_14, GPIO_NUM_27);

void setup(void) {
	encoder.init();
	u8g2.begin();
	//u8g2.setDisplayRotation(U8G2_R1);
}

class Messages {
public:
	void addValue(const char * title, const long & value) {
		char text[100];
		sprintf(text, "%s : %ld", title, value);
		this->messages.emplace_back(std::string(text));
	}

		void addValue(const char * title, const unsigned long & value) {
		char text[100];
		sprintf(text, "%s : %lu", title, value);
		this->messages.emplace_back(std::string(text));
	}

	void addValue(const char * title, const bool & value) {
		char text[100];
		sprintf(text, "%s : %s", title, value ? "true" : "false");
		this->messages.emplace_back(std::string(text));
	}

	void addValue(const char * title, const int & value) {
		char text[100];
		sprintf(text, "%s : %d", title, value);
		this->messages.emplace_back(std::string(text));
	}

	void addBits(const unsigned long & bits, int length) {
		char text[length];
		for(int i=0; i<length; i++) {
			auto value = (bits >> i) & 1UL;
			text[i] = value == 0 ? '0' : '1';
		}
		text[length] = '\0';
		this->messages.emplace_back(std::string(text));
	}

	void printToScreen() {
		int y = 10;
		for(const auto & message : this->messages) {
			u8g2.drawStr(0, y, message.c_str());
			y += 10;
		}
	}
protected:
	std::vector<std::string> messages;
};

void loop(void) {
	// u8g2.clearBuffer();					// clear the internal memory
	// u8g2.sendBuffer();					// transfer internal memory to the display
	// delay(1);

	u8g2.clearBuffer();					// clear the internal memory
	u8g2.setFont(u8g2_font_profont12_tf);	// choose a suitable font

	{
		Messages messages;
		messages.addValue("value", encoder.getValue());
		//messages.addValue("stateCode", (int) stateCode);
		//messages.addBits((unsigned long) gpioBits, 31);
		messages.printToScreen();
	}

	u8g2.sendBuffer();					// transfer internal memory to the display
}
