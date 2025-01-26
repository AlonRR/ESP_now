#include "main.hpp"
#ifdef SEND_RECIVE_MODE
#ifndef OLED_H
#define OLED_H
#include <Arduino.h>
#include <U8g2lib.h>
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

void print_data_to_OLED(_datapacket *incomingData);
void u8g2_prepare();

#endif // OLED_H
#endif // SEND_RECIVE_MODE