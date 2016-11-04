#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino.h>
//#include <Comm.h>
//#include <ESP8266WebServer.h>

#include <ESP8266WiFi.h>
//https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/server-examples.md#the-page

class Comm;

class WebServer{
  private:
    String buffer_;
    int port_;
  //  ESP8266WebServer *internal_server_;
    WiFiServer *internal_server_;
    Comm *comm_;

  public:
    WebServer(Comm *comm);
    void Setup();
    void Run();
    void Api();
    void Start();
    void Dashboard();
    String GetBuffer();
};


#endif // WEBSERVER_H
