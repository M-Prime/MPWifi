#ifndef MPWIFI_H
#define MPWIFI_H

#include <Arduino.h>
#include "FileSystem.h"
#include "WebServer.h"
#include "Comm.h"
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
    String a_c_user_;
    String a_c_pass_;
    FileSystem file_system_;
    WebServer *server_;
    //SD *sd_;
    Comm comm_;
    bool session_;
  public:
    MpWifi();
    void Run();
    void Boot();
    void SetStation(String ssid, String pass, String host_name);
    void SetAp(String ssid, String pass);
    void SetWorkMode(int work_mode);
    void SetUser(String user, String pass);
    bool CheckUser(String user, String pass);
    String GetStationSsid();
    String GetStationPass();
    String GetStationHostname();
    String GetApSsid();
    String GetApPass();
    int GetWorkMode();
    bool GetSession();
    String GetACUser();
    String GetACPass();
    int GetSerialBaudrate();
    void SetAutoBaudrate();
    void Blink(int times, int delay_time);
};

#endif // MPWIFI_H
