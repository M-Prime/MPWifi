#include "MpWifi.h"

WebServer server_t;

/**
* Constructor, creates the objects and connect one to each other
*/
MpWifi::MpWifi(){
  //Create FyleSystem
  FileSystem file_system;
  file_system_ = file_system;
  //Create Comm
  Comm comm_t;
  comm_ = comm_t;
  //Create WebServer for HTTP
  server_ = &server_t;
  server_->SetObjetcs(&comm_, &file_system_, this);

  comm_.ConnectWebServer(server_, this);

}

/**
* Boot the sistem, set the network and the servers
*/
void MpWifi::Boot(){

  //For development purposes
      //file_system_.DeleteFile("first.txt");

  //Set the LED for blink in case needed
  pinMode(2, OUTPUT);
  digitalWrite(2,HIGH);
  
  //Setting up serial port baudrate
  SetAutoBaudrate();

  //Set network info
  if(file_system_.ExistFile("first.txt")){
    ssid_ = file_system_.GetFile("ssid.txt");
    pass_ = file_system_.GetFile("pass.txt");
    ap_ssid_ = file_system_.GetFile("ap_ssid.txt");
    ap_pass_ = file_system_.GetFile("ap_pass.txt");
    host_name_ = file_system_.GetFile("host_name.txt");
    work_mode_ = file_system_.GetFile("work_mode.txt").toInt();
    a_c_user_ = file_system_.GetFile("a_c_user.txt");
    a_c_pass_ = file_system_.GetFile("a_c_pass.txt");
  } else {
    this->SetStation("", "", "mpwifi");
    this->SetAp("MPWifi", "mpwifipass");
    this->SetUser("mpwifi", "mpwifipass");
    this->SetWorkMode(2);
  }

  session_ = false;


  //Select and set up network mode
  int now = 0;
  int start = 0;

  if(work_mode_ == 1){
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid_.c_str(), pass_.c_str());
    start = millis();
    now = 0;
    while(WiFi.status() != WL_CONNECTED && now<5000){
        delay(500);
        now = millis() - start;
    }
    if(now < 5000){
      this->Blink(2, 250);
      //DNS setting
      if (!MDNS.begin(host_name_.c_str())) {
        while(1) {
          this->Blink(1, 1000);
        }
      }
    } else {
      work_mode_ = 2;
      this->Blink(5, 1000);
    }
  } if(work_mode_ == 2){


    WiFi.softAP("MPWifi", ap_pass_.c_str());
    this->Blink(4, 250);
  }
  //Begin the Web Server
  server_->Setup();
  //Communications setup
  comm_.Setup();

  MDNS.addService("http", "tcp", 80);
}

/**
 * Look for clients
 */
void MpWifi::Run(){
  comm_.Check();
  server_->Run();
}

/**
 * Try to get the printer baudrate
 */
void MpWifi::SetAutoBaudrate(){

  int baudrate[8] = {250000, 230400, 115200, 74880, 57600, 38400, 19200, 9600};
  bool found = false;
  int i = 0;
  while(!found && i < 8){
    Serial.begin(baudrate[i]);
    Serial.println("M105");
    delay(100);
    while(Serial.available() > 0){
      if(Serial.read() == 'o'){
        serial_baudrate_ = baudrate[i];
        found = true;
      }
    }
    i++;
  }
  
  //For development purposes
  if(!found){
    Serial.begin(115200);
    serial_baudrate_ = 115200;
  }
}

/**
 * Set the station parameters up
 *  @param ssid Your router's WiFi SSID
 *  @param pass Your router's WiFi password
 *  @param host_name Domain name you will use on the web browser
 */
void MpWifi::SetStation(String ssid, String pass, String host_name){
  ssid.replace("%20", " ");
  ssid.replace("+", " ");
  ssid_ = ssid;
  file_system_.SetFile("ssid.txt", ssid);
  pass_ = pass;
  file_system_.SetFile("pass.txt", pass);
  host_name_ = host_name;
  file_system_.SetFile("host_name.txt", host_name);
}

/**
* Set the Access Point parameters up
* @Param ssid SSID for MPWifi Access Point
* @Param pass Password for MPWifi Access Point
*/
void MpWifi::SetAp(String ssid, String pass){
  ap_ssid_ = ssid;
  file_system_.SetFile("ap_ssid.txt", ssid);
  ap_pass_ = pass;
  file_system_.SetFile("ap_pass.txt", pass);
}

/**
* Set work mode up
* @Param work_mode 1 -> Station mode; 2 -> Access Point mode
*/
void MpWifi::SetWorkMode(int work_mode){
  work_mode_ = work_mode;
  String work_mode_string = String(work_mode);
  file_system_.SetFile("work_mode.txt", work_mode_string);
}

/**
 * Set user access control data
 */

 void MpWifi::SetUser(String user, String pass){
    file_system_.SetFile("a_c_user.txt", user);
    a_c_user_ = user;
    file_system_.SetFile("a_c_pass.txt", pass);
    a_c_pass_ = pass;    
 }

/**
 * Check if the user is login
 */
 bool MpWifi::CheckUser(String user, String pass){
    if(user == a_c_user_ && pass == a_c_pass_){
      session_ = true;      
      return true;
    } else {
      session_ = false;
      return false;
    }
 }

/**
* Blink the LED
* @Param times How many times the LED must Blink
* @Param delay_time Both HIGH time and LOW time
*/
void MpWifi::Blink(int times, int delay_time){
  for(int i = 0; i < times; i++){
    digitalWrite(2,LOW);
    delay(delay_time);
    digitalWrite(2,HIGH);
    delay(delay_time);
  }
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

bool MpWifi::GetSession(){
  return session_;
}

String MpWifi::GetACUser(){
  return a_c_user_;
}

String MpWifi::GetACPass(){
  return a_c_pass_;
}

