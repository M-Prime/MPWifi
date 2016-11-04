#include <Comm.h>
#include "../WebServer/WebServer.h"

Comm::Comm(){
  //Get pointer to server from MpWifi
  //web_server_ = server;
}

String Comm::Run(){
  if(web_server_->GetBuffer() != ""){
    Serial.println(web_server_->GetBuffer());
  }
  return "ok";
}

void Comm::SetWebServer(WebServer *web_server){
  web_server_ = web_server;
}
