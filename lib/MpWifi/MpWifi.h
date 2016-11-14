#ifndef MPWIFI_H
#define MPWIFI_H

#include <Arduino.h>
#include <FileSystem.h>
#include <WebServer.h>
#include <Comm.h>
#include <ESP8266mDNS.h>

class MpWifi{
  private:
    String ssid_;
    String pass_;
    String ap_ssid_;
    String ap_pass_;
    String host_name_;
    int work_mode_;
    bool first_;
    int serial_baudrate_;
    FileSystem file_system_;
    WebServer server_;
    //SD *sd_;
    Comm comm_;
  public:
    MpWifi();
    void Run();
    void Boot();
    void SetStation(String ssid, String pass, String host_name);
    void SetAp(String ssid, String pass);
    void SetWorkMode(int work_mode);
    String GetStationSsid();
    String GetStationPass();
    String GetStationHostname();
    String GetApSsid();
    String GetApPass();
    int GetWorkMode();
    int GetSerialBaudrate();
    int FindBaudrate();
};

#endif // MPWIFI_H
