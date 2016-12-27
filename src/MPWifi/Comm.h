#ifndef COMM_H
#define COMM_H

#include <Arduino.h>

#include "Tcp.h"
#include "SdCard.h"

class WebServer;
class Tcp;
class MpWifi;

class Comm {
  private:
    String local_buffer_;
    int checksum_;
    String SD_command_[20];
    SdCard *sd_card_;
    Tcp *tcp_;
    WebServer *web_server_;
    MpWifi *mpwifi_;

  public:
    Comm();
    void ConnectWebServer(WebServer *server, MpWifi *mpwifi);
    void Setup();
    void Check();
    String CheckAndReturn();
    bool FilterGcode();
    void ChecksumAndSend();
};

#endif // COMM_H
