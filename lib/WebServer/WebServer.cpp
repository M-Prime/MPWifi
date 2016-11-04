#include <WebServer.h>

ESP8266WebServer server(80);

WebServer::WebServer(){
  //Get the objet pointer
  internal_server_ = &server;
  port_ = 80;
}

void WebServer::Setup(){
  //Prepare the server's URL
  internal_server_->on("/", std::bind(&WebServer::Dashboard, this));
  internal_server_->on("/start", std::bind(&WebServer::Start, this));
  internal_server_->on("/api", std::bind(&WebServer::Api, this));
  //Begin the web server
  internal_server_->begin();
}

void WebServer::Run(){
  internal_server_->handleClient();
  delay(10);
}

void WebServer::Api(){
  String html = "You send: ";
  uint8_t i=0;
  html += internal_server_->arg(i);
  String buffer = internal_server_->arg(i);
  buffer_ = buffer;
  internal_server_->send(200, "text/html", html);
}

void WebServer::Start(){
  internal_server_->send(200, "text/html", "Welcome to Api");
}

void WebServer::Dashboard(){
  internal_server_->send(200, "text/html", "Welcome to Dashboard");
}

String WebServer::GetBuffer(){
  return buffer_;
}
