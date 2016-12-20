#include "Tcp.h"

WiFiServer tcp_server(6000);
WiFiClient client;

/**
* The constructor initialize the TCP flag
* tcp_flag_ is true when buffer has content
*/
Tcp::Tcp(){
  tcp_flag_ = false;
  buffer_ = "";
}

/**
* Open a client when needed and get the data from TCP communication
*/
void Tcp::WaitCommand(){
  //Wait until client try to connect
  if (!client) {
     client = tcp_server.available();
 }
 else { //When client
     if (client.status() == CLOSED) {
         client.stop();
         alreadyConnected_ = false;
     } else { //If client is already connected
         if(!alreadyConnected_) {
             // Clean out the input buffer:
             client.flush();
             alreadyConnected_ = true;
         }
         //If the client has sent data
         if (client.available()) {
             //Read incoming data from the client
             buffer_ = client.readString();
             //Set the flag up
             tcp_flag_ = true;
         }
     }
 }
}

/**
* Send data to a TCP client
* @param command The data we will send to the client
*/
void Tcp::SendCommand(String command){
  client.print(command);
  delay(100);
}

/**
* Read the buffer
* @return The TCP buffer
*/
String Tcp::GetBuffer(){
  return buffer_;
}

/**
* Get the tcp_flag, with indicates if there is content in the buffer
* @return The flag state, a boolean
*/
bool Tcp::GetFlag(){
  return tcp_flag_;
}

/**
* Set the TCP flag down
*/
void Tcp::ClearFlag(){
  tcp_flag_ = false;
}

/**
* This method is used to begin the TCP server,
* it may be called once at the boot.
*/
void Tcp::Setup(){
  tcp_server.begin();
}
