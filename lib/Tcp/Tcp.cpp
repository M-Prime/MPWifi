#include <Tcp.h>

WiFiServer tcp_server(6000);
WiFiClient client;

void Tcp::WaitCommand(){
  if (!client) {
     client = tcp_server.available();
 }
 else {
     if (client.status() == CLOSED) {
         client.stop();
         alreadyConnected_ = false;
     } else {
         if(!alreadyConnected_) {
             // Clean out the input buffer:
             client.flush();
             alreadyConnected_ = true;
         }

         if (client.available()) {
             // read incoming data from the client:
             String data = client.readString();
             Serial.println(data);
             //delay(10);
             String command = "";
             while(Serial.available()) {
               char letra = Serial.read();
               command += letra;
             }
             client.print(command);
         }
     }
 }
}

void Tcp::SendCommand(String command){
  client.print(command);
}

String Tcp::GetBuffer(){
  return buffer_;
}

void Tcp::Setup(){
  tcp_server.begin();
}
