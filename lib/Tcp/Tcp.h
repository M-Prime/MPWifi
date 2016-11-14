#ifndef TCP_H
#define TCP_H

#include <Arduino.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>

class Tcp{
  private:
    String buffer_;
    bool alreadyConnected_;
    WiFiServer *server_;
    WiFiClient client_;
  public:
    void WaitCommand();
    void SendCommand(String command);
    String GetBuffer();
    void Setup();
};

#endif // TCP_H
