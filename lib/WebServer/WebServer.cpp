#include <WebServer.h>

WebServer::WebServer(){
  port_ = 80;
  ESP8266WebServer internal_server(port_);


  internal_server.on("/api", std::bind(&WebServer::Api, this));
  internal_server.on("/", std::bind(&WebServer::Api, this));
  internal_server.on("/start", std::bind(&WebServer::Api, this));
  internal_server.on("/settings", std::bind(&WebServer::Api, this));
  internal_server.begin();

  //MDNS.addService("http", "tcp", 80);
}

void WebServer::Run(){

}

void WebServer::Pannel(){

}

void WebServer::Api(){

}
