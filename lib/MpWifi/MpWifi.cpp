#include "MpWifi.h"

MpWifi::MpWifi(){
  //Create FyleSystem object
  FileSystem file_system;
  file_system_ = &file_system;
  //Create WebServer for HTTP
  WebServer server_t;
  server_ = &server_t;

}

void MpWifi::Boot(){
  //file_system_->ExistFile("first")
  if(false){
    ssid_ = file_system_->GetFile("ssid");
    pass_ = file_system_->GetFile("pass");
    ap_ssid_ = file_system_->GetFile("ap_ssid");
    ap_pass_ = file_system_->GetFile("ap_pass");
    host_name_ = file_system_->GetFile("host_name");
    work_mode_ = (file_system_->GetFile("work_mode").toInt()-48);
  } else {
    ssid_ = "";
    pass_ = "";
    ap_ssid_ = "MPWifi";
    ap_pass_ = "mpwifipass";
    host_name_ = "mpwifi";
    work_mode_ = 2;
    serial_baudrate_ = FindBaudrate();
    //Set first file
  }

  //Setting up serial port baudrate
  Serial.begin(serial_baudrate_);

  //Select and set up network mode
  int now = 0;
  int start = 0;
  switch(work_mode_){
    case 3:
      WiFi.mode(WIFI_AP_STA);
    case 1:
      WiFi.begin(ssid_.c_str(), pass_.c_str());
      start = millis();
      now = 0;
      while(WiFi.status() != WL_CONNECTED && now<5000){
          delay(500);
          Serial.print(".");
          now = millis() - start;
      }
      if(now < 5000){
        Serial.print("Local IP:");
        Serial.println(WiFi.localIP());
        //DNS setting
        if (!MDNS.begin(host_name_.c_str())) {
          Serial.println("Error setting up DNS");
          while(1) {
            delay(1000);
          }
        } else {
          MDNS.addService("http", "tcp", 80);
        }
      } else {
        work_mode_ = 2;
        Serial.println("Error setting up Station mode");
      }
      if(work_mode_ == 1) break;
    case 2:
      WiFi.softAP(ap_ssid_.c_str(), ap_pass_.c_str());
      Serial.println("AP mode enable.");
      break;
  }
  //Setting URL and begin the Web Server
  server_->Setup();
}

void MpWifi::Run(){
  //Look for clients
  server_->Run();
}

int MpWifi::FindBaudrate(){
  return 115200;
}

void MpWifi::SetStation(String ssid, String pass, String host_name){
  ssid_ = ssid;
  pass_ = pass;
  host_name_ = host_name;
}

void MpWifi::SetAp(String ssid, String pass){
  ap_ssid_ = ssid;
  ap_pass_ = pass;
}

void MpWifi::SetWorkMode(int work_mode){
  work_mode_ = work_mode;
}

String MpWifi::GetStationSsid(){
  return ssid_;
}

String MpWifi::GetStationPass(){
  return pass_;
}

String MpWifi::GetStationHostname(){
  return host_name_;
}

String MpWifi::GetApSsid(){
  return ap_ssid_;
}

String MpWifi::GetApPass(){
  return ap_pass_;
}

int MpWifi::GetSerialBaudrate(){
  return serial_baudrate_;
}
