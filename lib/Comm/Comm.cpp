#include <Comm.h>

Comm::Comm(WebServer *server){
  server_ = server;
}

void Comm::Run(){
  if(server_->GetBuffer() != ""){
    Serial.println(server_->GetBuffer());
  }
}
