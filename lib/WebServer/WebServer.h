#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino.h>
#include <FileSystem.h>
#include <ESP8266WiFi.h>

class Comm;

class WebServer{
  private:
    String buffer_;
    String request_;
    String ssid_, pass_, host_name_, ap_ssid_, ap_pass_;
    int work_mode_;
    WiFiServer *internal_server_;
    WiFiClient *client_;
    Comm *comm_;
    FileSystem *file_system_;

  public:
    WebServer();
    void SetObjetcs(Comm *comm, FileSystem *file_system);
    void SetSettings(String ssid, String pass, String host_name, String ap_ssid, String ap_pass, int work_mode);
    void Setup();
    void Run();
    String GetUrlData(String name);
    void AnalizeURL();
    void Api();
    void Start();
    void Dashboard();
    String GetBuffer();
    void Prepare(String page, String content);
};


#endif // WEBSERVER_H
