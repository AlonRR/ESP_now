#include "oled.hpp"
#define HEIGHT 64
#define WIDTH 128

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

void print_data_to_OLED(const _datapacket *incomingData)
{
    u8g2.clearBuffer();
    u8g2.drawStr(2, 0, "MASTER");
    u8g2.drawStr(2, 10, "Value: ");
    u8g2.setCursor(45, 10);
    u8g2.print(incomingData->value);
    u8g2.drawStr(2, 20, "Name: ");
    u8g2.setCursor(45, 20);
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