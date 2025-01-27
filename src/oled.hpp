#include "main.hpp"
#ifdef SEND_RECIVE_MODE
#ifndef OLED_H
#define OLED_H
#include <Arduino.h>
#include <U8g2lib.h>

// // Define OLED pins - using hardware SPI
// #define OLED_SCL 22  // Clock pin
// #define OLED_SDA 21  // Data pin
// #define OLED_RST 16  // Reset pin

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

void print_data_to_OLED(const _datapacket *incomingData);
void u8g2_prepare();

#endif // OLED_H
#endif // SEND_RECIVE_MODE