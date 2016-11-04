#include <WebServer.h>
#include "../Comm/Comm.h"

WiFiServer server(80);

WebServer::WebServer(Comm *comm){



  internal_server_ = &server;
  //Get the object pointer
  comm_ = comm;
  port_ = 80;
}

void WebServer::Setup(){
   server.begin();
}

void WebServer::Run(){
  WiFiClient client = server.available();
  // wait for a client (web browser) to connect
  if (client)
  {
    Serial.println("\n[Client connected]");
    while (client.connected())
    {
      // read line by line what the client is requesting
      if (client.available())
      {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        // wait for end of client's request, that is marked with an empty line
        if (line.length() == 1 && line[0] == '\n')
        {
          client.println("Soy un servidor aunque no lo parezca");
          break;
        }
      }
    }
    delay(1); // give the web browser time to receive the data

    // close the connection:
    client.stop();
    Serial.println("[Client disonnected]");
  }
}

void WebServer::Api(){
/*  String html = "You send: ";
  uint8_t i=0;
  html += internal_server_->arg(i);
  buffer_ = internal_server_->arg(i);
  //Call comm
  html += "<br>Response: ";
  html += comm_->Run();
  internal_server_->send(200, "text/html", html);*/
}

void WebServer::Start(){
  //internal_server_->send(200, "text/html", "Welcome to Api");
}

void WebServer::Dashboard(){
//  internal_server_->send(200, "text/html", "Welcome to Dashboard");
}

String WebServer::GetBuffer(){
  return buffer_;
}
