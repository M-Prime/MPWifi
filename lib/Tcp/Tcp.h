#ifndef TCP_H
#define TCP_H

#include <Arduino.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
//#include <ESP8266WebServer.h>

class Tcp{
  private:
    String buffer_;
    boolean alreadyConnected_[2];
    int status_;
    WiFiServer *server_;
    WiFiClient client_[2];
  public:
    Tcp();
    void WaitCommand();
    void SendCommand(String command);
    String GetBuffer();
};

#endif // TCP_H
