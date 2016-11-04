#include <Arduino.h>
#include <ESP8266WebServer.h>
//Comm.h

class WebServer{
  private:
    String buffer_;
    int port_;
    ESP8266WebServer *internal_server_;

  public:
    WebServer();
    void Setup();
    void Run();
    void Api();
    void Start();
    void Dashboard();
    String GetBuffer();
};
