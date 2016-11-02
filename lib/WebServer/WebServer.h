#include <Arduino.h>
#include <ESP8266WebServer.h>

class WebServer{
  private:
    String buffer_;
    int port_;

  public:
    WebServer();
    void Run();
    void Pannel();
    void Api();
    void handler();
};
