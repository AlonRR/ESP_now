#include "sender&reciver.hpp"

Mode mode;
// esp_now_peer_info_t brodcastAddress;
esp_now_peer_info_t yigalInfo;
uint8_t yigalAddress[] = {0xC4, 0x5B, 0xBE, 0x8D, 0xBC, 0x8C};
_datapacket s_packet;
_datapacket r_packet;
bool mode_changed = 1;

void setup()
{
    Serial.begin(115200);
    u8g2_prepare();
    WiFi.mode(WIFI_STA);
    Serial.println(WiFi.macAddress());

    if (esp_now_init() != 0)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_register_recv_cb(OnDataRecieve);
    esp_now_register_send_cb(OnDataSend);

    memcpy(yigalInfo.peer_addr, yigalAddress, 6);
    yigalInfo.channel = 1;
    yigalInfo.encrypt = false;
    esp_now_add_peer(&yigalInfo);
    // brodcastAddress.encrypt = false;
    // brodcastAddress.peer_addr = [0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF];

    pinMode(HUMIDITY_SENSOR_PIN, INPUT);
    pinMode(PUSH_BUTTON_PIN, INPUT_PULLUP);
    pinMode(SWITCH_PIN, INPUT_PULLUP);

    // attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), changeMode, CHANGE);
    attachInterruptArg(digitalPinToInterrupt(PUSH_BUTTON_PIN), changeMode, self, CHANGE);
    mode = digitalRead(SWITCH_PIN) == HIGH ? MASTER : SLAVE;
}

void loop()
{
    char *message;
    int val;
    if (mode_changed)
    {
        mode_changed = 0;
        u8g2.clearBuffer();
        if (mode == SLAVE)
        {
            message = (char *)"SLAVE";
        }
        if (mode == MASTER)
        {
            message = (char *)"MASTER";
        }
        u8g2.drawStr(2, 0, message);
        u8g2.sendBuffer();
    }
    if (mode == MASTER)
        sendSensorRequest(yigalAddress);
    delay(1000);
}

int readSensor()
{
    static unsigned char value = 0;
    // value = analogRead(LIGHT_SENSOR_PIN);
    value += 1;
    return value;
}

void sendSensorRequest(const uint8_t *mac_addr)
{
    r_packet.value = 1;
    // strcpy(r_packet.message, "Request");
    r_packet.message = "Request";
#ifdef VERBOSE
    verbosePrintMessage(&r_packet);
#endif
    esp_now_send(mac_addr, (uint8_t *)&r_packet, (sizeof(r_packet) + sizeof(r_packet.message) * sizeof(char)));
}

void sendSensorData(int value, const uint8_t *mac_addr)
{
    s_packet.value = value;
    s_packet.message = (char *)"Alon A.";
    esp_now_send(mac_addr, (uint8_t *)&s_packet, (sizeof(s_packet) + sizeof(s_packet.message) * sizeof(char)));
}

// Callback when data is recieved
void OnDataRecieve(const uint8_t *mac_addr, const uint8_t *incomingData, int len)
{
    int value;
#ifdef VERBOSE
    verbosePrint(mac_addr, incomingData, len);
#endif
    if (mode == SLAVE)
    {
        value = readSensor();
        sendSensorData(value, mac_addr);
        return;
    }
    if (mode == MASTER)
    {
        print_data_to_OLED((struct _datapacket *)incomingData);
        return;
    }
}

// Callback when data is sent
void OnDataSend(const uint8_t *mac_addr, esp_now_send_status_t sendStatus)
{
#ifdef VERBOSE
    if (sendStatus == 0)
        Serial.print("Delivery success");
    else
        Serial.print("Delivery fail");
    Serial.printf("\tMAC: %x:%x:%x:%x:%x:%x\n",
                  mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
#endif
}

void verbosePrint(const uint8_t *mac_addr, const uint8_t *incomingData, int len)
{
    struct _datapacket packet = *(struct _datapacket *)incomingData;
    Serial.printf("MAC: %x:%x:%x:%x:%x:%x\t",
                  mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    Serial.printf("Value: %d\n", packet.value);
    Serial.printf("message: %s\n", packet.message.c_str());
}

void verbosePrintMessage(_datapacket *incomingData)
{
    Serial.printf("Value: %d\n", incomingData->value);
    Serial.printf("message: %s\n", incomingData->message.c_str());
}

void changeMode(void *_read)
{
    int read = (int)_read;
    mode_changed = 1;
    mode = read == HIGH ? MASTER : SLAVE;
}