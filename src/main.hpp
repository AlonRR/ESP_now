#ifndef MAIN_HPP
#define MAIN_HPP
#define VERBOSE
// #define RECIVER_MODE
// #define SENDER_MODE
#define SEND_RECIVE_MODE
#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

struct _datapacket
{
    String message;
    unsigned char value;
};

enum Mode
{
    MASTER = 0,
    SLAVE,
    OTHER
};

#endif // MAIN_HPP