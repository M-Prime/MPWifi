#include <Comm.h>
#include "../WebServer/WebServer.h"

Comm::Comm(){
  Tcp tcp_t;
  tcp_ = &tcp_t;
}

void Comm::SetWebServer(WebServer *server){
  web_server_ = server;
}

void Comm::Check(){
  String local_buffer = "";

  tcp_->WaitCommand();
  
}

String Comm::CheckAndReturn(){
  String local_buffer = "";

  local_buffer = "";
  local_buffer = web_server_->GetBuffer();
  if(local_buffer != ""){
    Serial.println(local_buffer);
    String command = "";
    while(Serial.available()) {
      char letra = Serial.read();
      command += letra;
    }
    return command;
  }
}
