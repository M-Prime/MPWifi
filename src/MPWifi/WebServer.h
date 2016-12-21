#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino.h>
#include "FileSystem.h"
#include <ESP8266WiFi.h>

class Comm;
class MpWifi;

class WebServer{
  private:
    String buffer_;
    bool api_flag_;
    String request_;
    int work_mode_;
    WiFiServer *internal_server_;
    WiFiClient *client_;
    Comm *comm_;
    FileSystem *file_system_;
    MpWifi *mpwifi_;
    String dashboard_[3];
    String start_[4];

  public:
    WebServer();
    void SetObjetcs(Comm *comm, FileSystem *file_system, MpWifi *mpwifi);
    void Setup();
    void Run();
    String GetUrlData(String name);
    String GetUrlParameter(String name);
    void AnalizeURL();
    void Api();
    void Start();
    void Dashboard();
    String GetBuffer();
    void Prepare(String page, String content);
    bool GetFlag();
    void ClearFlag();
};


#endif // WEBSERVER_H
