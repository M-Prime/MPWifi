#ifndef COMM_H
#define COMM_H

#include <Arduino.h>

#include "../Tcp/Tcp.h"

class WebServer;
class Tcp;

class Comm {
  private:
    //Sd *sd_;
    Tcp *tcp_;
    WebServer *web_server_;

  public:
    Comm();
    void Check();
    String CheckAndReturn();
    void SetWebServer(WebServer *server);
    void Setup();
};

#endif // COMM_H
