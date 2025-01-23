#include "main.hpp"
#ifdef RECIVER_MODE

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <String.h>

// Callback when data is recieved
void OnDataRecieve(const uint8_t *mac_addr, const uint8_t *incomingData, int len)
{
    String buffer;
    Serial.printf("Recieved from %x:%x:%x:%x:%x:%x %d bytes ...",
                  mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5], len);
    Serial.printf("Value: %d\n", ((struct _datapacket *)incomingData)->value);
    buffer = ((struct _datapacket *)incomingData)->message;
    Serial.printf("Message: %s\n", buffer);
}

void setup()
{
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    // Init ESP-NOW
    if (esp_now_init() != 0)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_register_recv_cb(OnDataRecieve);
}
void loop() {}

#endif // RECIVER_MODE