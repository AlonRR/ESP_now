#include "main.hpp"
#ifdef SENDER_MODE

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <String.h>

// my mac 08:3A:F2:3B:51:D8
// Yigal mac C4:5B:BE:8D:BC:8C

uint8_t peerAddress[] = {0xC4, 0x5B, 0xBE, 0x8D, 0xBC, 0x8C};
esp_now_peer_info_t peerInfo;


// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t sendStatus)
{
    if (sendStatus == 0)
        Serial.println("Delivery success");
    else
        Serial.println("Delivery fail");
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

    esp_now_register_send_cb(OnDataSent);
    memcpy(peerInfo.peer_addr, peerAddress, 6);
    peerInfo.channel = 1;
    peerInfo.encrypt = false;
    esp_now_add_peer(&peerInfo);
}

void loop()
{
    esp_now_send(peerAddress, (uint8_t *)&packet, sizeof(packet));
    Serial.printf("Sent %d -", packet.value++);
    delay(5000);
    strcpy(packet.message, "Hello");
}

#endif // SENDER_MODE