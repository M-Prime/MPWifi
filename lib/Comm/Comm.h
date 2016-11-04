#ifndef COMM_H
#define COMM_H

#include <Arduino.h>
//#include <WebServer.h>

class WebServer;

class Comm {
  private:
    //Sd *sd_;
    //Tcp *tcp_;
    WebServer *web_server_;
  public:
    Comm();
    String Run();
    void SetWebServer(WebServer *web_server);
};

#endif // COMM_H
