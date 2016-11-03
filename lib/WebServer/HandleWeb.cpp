#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

void Dashboard(){
  server.send(200, "text/html", "Welcome to dashboard");
}
