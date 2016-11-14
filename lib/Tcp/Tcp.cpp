#include <Tcp.h>

WiFiServer tcp_server(6000);
WiFiClient tcp_client[2];

Tcp::Tcp(){
  server_ = &tcp_server;
  status_ = WL_IDLE_STATUS;
}

void Tcp::WaitCommand(){
    //TCP conection control
    unsigned int i = 0;
    // wait for a new client:
    if (!tcp_client[i]) {
        tcp_client[i] = tcp_server.available();
    }
    else {
        if (tcp_client[i].status() == CLOSED) {
            tcp_client[i].stop();
            alreadyConnected_[i] = false;
        } else {
            if(!alreadyConnected_[i]) {
                // Clean out the input buffer:
                tcp_client[i].flush();
                alreadyConnected_[i] = true;
            }

            if (tcp_client[i].available()) {
                // read the bytes incoming from the client:
                String data = tcp_client[i].readString();
                // Send data to Marlin
                Serial.print(data);
                //Read and send response
                String command = "";
                while(Serial.available()) {
                  char letra = Serial.read();
                  command += letra;
                }
                tcp_client[i].print(command);
            }
        }
    }
}
