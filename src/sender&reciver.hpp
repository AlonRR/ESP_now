#include "main.hpp"
#ifdef SEND_RECIVE_MODE
#include "oled.hpp"

// #ifdef U8X8_HAVE_HW_SPI
// #include <SPI.h>
// #endif
// #ifdef U8X8_HAVE_HW_I2C
// #include <Wire.h>
// #endif

#define HUMIDITY_SENSOR_PIN 32
#define PUSH_BUTTON_PIN 18
#define SWITCH_PIN 19

void u8g2_prepare();
void OnDataRecieve(const uint8_t *mac_addr, const uint8_t *incomingData, int len);
void OnDataSend(const uint8_t *mac_addr, esp_now_send_status_t sendStatus);
void sendSensorData(int value, const uint8_t *mac_addr);
void sendSensorRequest(const uint8_t *mac_addr);
int readSensor();
void verbosePrint(const uint8_t *mac_addr, const uint8_t *incomingData, int len);
void verbosePrintMessage(_datapacket *incomingData);
void changeMode(int read);
void setup();
void loop();

#endif // SEND_RECIVE_MODE