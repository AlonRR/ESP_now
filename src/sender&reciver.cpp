#include "main.h"
#ifdef SEND_RECIVE_MODE
#include <Arduino.h>
#include <WiFi.h>
struct _datapacket
{
    int value;
    String message;
};

void OnDataRecieve(const uint8_t *mac_addr, const uint8_t *incomingData, int len);
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t sendStatus);
void sendSensorData(int value, const uint8_t *mac_addr);
void sendSensorRequest(const uint8_t *mac_addr);
int readSensor();
// void sender(_datapacket *s_packet, const uint8_t *mac_addr);
// void verbosePrint(const uint8_t *mac_addr, String message, int val = 0);
void doOledStuff(_datapacket *incomingData);

enum Mode
{
    MASTER = 0,
    SLAVE
};
Mode mode;

esp_now_peer_info_t yigalInfo;
uint8_t yigalAddress[] = {0xC4, 0x5B, 0xBE, 0x8D, 0xBC, 0x8C};

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
    esp_now_register_send_cb(OnDataSent);
    memcpy(yigalInfo.peer_addr, yigalAddress, 6);
    yigalInfo.channel = 1;
    yigalInfo.encrypt = false;
    esp_now_add_peer(&yigalInfo);
    mode = SLAVE;
}

void loop()
{
    if (mode == MASTER)
    {
        sendSensorRequest(yigalAddress);
        delay(5000);
    }
}

int readSensor()
{
    static int value = 0;
    value += 1;
    return value;
}

void setupTempSensor(){
#ifdef TEMP_SENSOR_PIN
    #define TEMP_SENSOR_PIN
    pinMode(TEMP_SENSOR_PIN, INPUT);
#endif
}

int readTemp(){

}

void doOledStuff(_datapacket *incomingData)
{
    Serial.printf("Value: %d\n", incomingData->value);
    Serial.printf("Message: %s\n", incomingData->message);
}

void sendSensorRequest(const uint8_t *mac_addr)
{
    _datapacket s_packet;
    s_packet.value = 1;
    s_packet.message = "Request";
    // sender(&s_packet, mac_addr);
    esp_now_send(mac_addr, (uint8_t *)&s_packet, (sizeof(s_packet) + s_packet.message.length() * sizeof(char)));
    // Serial.printf("MAC: %x:%x:%x:%x:%x:%x\t",
    //               mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    // Serial.printf("Sent %d -", s_packet.value);
    // Serial.printf("Message: %s\n", s_packet.message);
}

//

void sendSensorData(int value, const uint8_t *mac_addr)
{
    _datapacket s_packet;
    s_packet.value = value;
    s_packet.message = "Sensor data";
    for (int i = 0; i < value; i++)
        s_packet.message += value;
    // sender(&s_packet, mac_addr);
    esp_now_send(mac_addr, (uint8_t *)&s_packet,
                 (sizeof(s_packet) + s_packet.message.length() * sizeof(char)));
}

// Callback when data is recieved
void OnDataRecieve(const uint8_t *mac_addr, const uint8_t *incomingData, int len)
{
    int value;
    String buffer;
    Serial.printf("Recieved from %x:%x:%x:%x:%x:%x %d bytes ...",
                  mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5], len);

    Serial.printf("Value: %d\n", ((struct _datapacket *)incomingData)->value);
    buffer = ((struct _datapacket *)incomingData)->message;
    Serial.printf("Message: %s\n", buffer);
    if (mode == SLAVE && ((struct _datapacket *)incomingData)->value == 1)
    {
        value = readSensor();
        sendSensorData(value, mac_addr);
        return;
    }
    if (mode == MASTER && ((struct _datapacket *)incomingData)->value == 69)
    {
        doOledStuff((struct _datapacket *)incomingData);
        return;
    }
}

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t sendStatus)
{
    if (sendStatus == 0)
        Serial.print("Delivery success");
    else
        Serial.print("Delivery fail");
    Serial.printf("MAC: %x:%x:%x:%x:%x:%x\n",
                  mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
}

// void verbosePrint(const uint8_t *mac_addr, String message, int val = 0)
// {
//     Serial.printf("MAC: %x:%x:%x:%x:%x:%x\t",
//                   mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
//     if (val == 0)
//         return;
//     Serial.printf("Value: %d\n", val);
//     Serial.printf("Message: %s\n", message);
// }

// void sender(_datapacket *s_packet, const uint8_t *mac_addr)
// {
//     esp_now_send(mac_addr, (uint8_t *)&s_packet, (sizeof(struct _datapacket) + sizeof(s_packet->message)));
//     Serial.printf("MAC: %x:%x:%x:%x:%x:%x\t",
//                   mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
//     Serial.printf("Sent %d -", s_packet->value);
//     Serial.printf("Message: %s\n", s_packet->message);
// }
#endif // SEND_RECIVE_MODE