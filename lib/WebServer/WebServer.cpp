#include <WebServer.h>
#include "HandleWeb.cpp"

WebServer::WebServer(){
  internal_server_ = &server;
  port_ = 80;


  internal_server_->on("/", Dashboard);
  internal_server_->begin();
}

void WebServer::Run(){
  internal_server_->handleClient();
  delay(10);
}
