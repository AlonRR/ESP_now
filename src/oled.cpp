#include "oled.hpp"
#define HEIGHT 64
#define WIDTH 128

void print_data_to_OLED(_datapacket *incomingData)
{
    u8g2.clearBuffer();
    u8g2.drawStr(0, 0, "MASTER");
    u8g2.drawStr(0, 10, "Value: ");
    u8g2.setCursor(0, 20);
    u8g2.print(incomingData->value);
    u8g2.drawStr(0, 30, "Name: ");
    u8g2.setCursor(0, 40);
    u8g2.print(incomingData->message);
    u8g2.sendBuffer();
}

void u8g2_prepare(void)
{
    u8g2.begin();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.setFontRefHeightExtendedText();
    u8g2.setDrawColor(1);
    u8g2.setFontPosTop();
    u8g2.setFontDirection(0);
}