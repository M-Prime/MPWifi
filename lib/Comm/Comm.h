#include <Arduino.h>
#include <WebServer.h>

class Comm {
  private:
    //Sd *sd_;
    //Tcp *tcp_;
    WebServer *server_;
  public:
    Comm(WebServer *server);
    void Run();
};
