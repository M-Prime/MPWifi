#include <WebServer.h>

WebServer::WebServer(){
  port_ = 80;
  ESP8266WebServer internal_server(port_);

  //internal_server.on("/", webRoot);
  internal_server.on("/g", std::bind(&WebServer::Api, this));
  //internal_server.on("/config", webConfig);
  //internal_server.on("/network", webNetwork);
  //internal_server.begin();

  //MDNS.addService("http", "tcp", 80);
}

void WebServer::Run(){

}

void WebServer::Pannel(){

}

void WebServer::Api(){

}

void handler()
    {
        // do something
    }
