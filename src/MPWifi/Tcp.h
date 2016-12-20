#ifndef TCP_H
#define TCP_H

#include <Arduino.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>

class Tcp{
  private:
    String buffer_;
    bool tcp_flag_;
    bool alreadyConnected_;
    WiFiServer *server_;
    WiFiClient client_;
  public:
    Tcp();
    void WaitCommand();
    void SendCommand(String command);
    String GetBuffer();
    bool GetFlag();
    void ClearFlag();
    void Setup();
};

#endif // TCP_H
