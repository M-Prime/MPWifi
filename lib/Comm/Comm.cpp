#include <Comm.h>
#include "../WebServer/WebServer.h"

Comm::Comm(){
  Tcp tcp_t;
  tcp_ = &tcp_t;
}

void Comm::SetWebServer(WebServer *server){
  web_server_ = server;
}

void Comm::Setup(){
  tcp_->Setup();
}

void Comm::Check(){
  tcp_->WaitCommand();
}

String Comm::CheckAndReturn(){
  Check();
  String local_buffer = "";

  local_buffer = web_server_->GetBuffer();
  if(local_buffer != ""){
    Serial.println(local_buffer);
    delay(10);
    String command = "";
    while(Serial.available()) {
      char letra = Serial.read();
      command += letra;
    }
    return command;
  }
}
