#include "WebServer.h"
#include "Comm.h"
#include "MpWifi.h"

WiFiServer server(80);

WebServer::WebServer(){
  internal_server_ = &server;
}

/**
* Connect WebServer with comm, file_system and mpwifi
* @param comm A pointer to a Comm object
* @param file_system A pointer to a FileSystem object
* @param mpwifi A pointer to a MpWifi object
*/
void WebServer::SetObjetcs(Comm *comm, FileSystem *file_system, MpWifi *mpwifi){
  comm_ = comm;
  file_system_ = file_system;
  mpwifi_ = mpwifi;
}

/**
* Begin the web server and get the HTML from internal file_system
*/
void WebServer::Setup(){
  //Start the web server
  server.begin();

  //Get HTML from file_system
   dashboard_[0] = file_system_->GetFile("dashboard_1.html");
   dashboard_[1] = file_system_->GetFile("dashboard_2.html");
   dashboard_[2] = file_system_->GetFile("dashboard_3.html");

   start_[0] = file_system_->GetFile("start_1.html");
   start_[1] = file_system_->GetFile("start_2.html");
   start_[2] = file_system_->GetFile("start_3.html");
   start_[3] = file_system_->GetFile("start_4.html");

}

/**
* Look for clients
*/
void WebServer::Run(){
  WiFiClient client = server.available();
  client_ = &client;
  // wait for a client (web browser) to connect
  if (client)
  {
    request_ = "";
    while (client.connected())
    {
      //Read the client request line by line
      if (client.available())
      {
        String line = client.readStringUntil('\r');
        //Save the request
        request_ += line;
        //If is the last line
        if (line.length() == 1 && line[0] == '\n')
        {
          AnalizeURL();
          break;
        }
      }
    }
    delay(1); //Give some time
    client.stop();  //Close the connection
  }
}

/**
* Send data data to web server output buffer
*/
void WebServer::Api(){
  String html_code = "";
  //Get the GCODE from URL
  String gcode = this->GetUrlData("/api/");
  if(gcode != ""){
    if(gcode == "firston"){
      file_system_->DeleteFile("first.txt");
      html_code += "first deleted";
    } else if (gcode == "firstoff"){
      file_system_->SetFile("first.txt", "existe");
      html_code = file_system_->GetFile("first.txt");
      html_code += "first created";
    } else {
      buffer_ = gcode;
      api_flag_ = true;
      //Send data to the printer and get response
      html_code += comm_->CheckAndReturn();
    }
  } else {
    html_code += "nothing";
  }
  //Send back the response
  client_->println(html_code);
  buffer_ = "";
}

/**
* First boot page
*/
void WebServer::Start(){
  //If form has been sent
  if(request_.indexOf("change=true") > 0){
    //Get station data
    String ssid = "";
    int station = 0;
    int pos = request_.indexOf("ssid=") + 5;
    if(pos > 0){
      while(request_[pos] != '&'){
        ssid += request_[pos];
        pos++;
      }
      station++;
    }
    String pass = "";
    pos = 0;
    pos = request_.indexOf("pass=") + 5;
    if(pos > 0){
      while(request_[pos] != '&'){
        pass += request_[pos];
        pos++;
      }
      station++;
    }
    String host_name = "";
    pos = 0;
    pos = request_.indexOf("hostname=") + 9;
    if(pos > 0){
      while(request_[pos] != '&'){
        host_name += request_[pos];
        pos++;
      }
      station++;
    }
    //If station data was received -> save data
    if(station > 0)
      mpwifi_->SetStation(ssid, pass, host_name);
    //Get work_mode
    int work_mode = 1;
    pos = 0;
    pos = request_.indexOf("workmode=") + 9;
    //If work_mode was received -> save work_mode
    if(pos > 0){
      work_mode = request_[pos]-48;
      mpwifi_->SetWorkMode(work_mode);
    }
    //Get AP data
    ssid = "";
    int ap = 0;
    pos = 0;
    pos = request_.indexOf("apssid=") + 7;
    if(pos > 0){
      while(request_[pos] != '&'){
        ssid += request_[pos];
        pos++;
      }
      ap++;
    }
    pass = "";
    pos = 0;
    pos = request_.indexOf("appass=") + 7;
    if(pos > 0){
      while(request_[pos] != '&'){
        pass += request_[pos];
        pos++;
      }
      ap++;
    }
    //If AP data was received -> save data
    if(ap > 0)
      mpwifi_->SetAp(ssid, pass);

    //Create first file
    file_system_->SetFile("first.txt", "existe");
  }

  //Prepare and send html response
  for(int i = 0; i < 4; i++){
    start_[i].replace("{{ ssid }}", mpwifi_->GetStationSsid());
    start_[i].replace("{{ pass }}", mpwifi_->GetStationPass());
    start_[i].replace("{{ hostname }}", mpwifi_->GetStationHostname());
    start_[i].replace("{{ ap-ssid }}", mpwifi_->GetApSsid());
    start_[i].replace("{{ ap-pass }}", mpwifi_->GetApPass());
    client_->println(start_[i]);
  }
}

/**
* Dashboard page
*/
void WebServer::Dashboard(){
  //Prepare and send html response
  for(int i = 0; i < 3; i++){
    String serial_baudrate_string = String(mpwifi_->GetSerialBaudrate());
    dashboard_[i].replace("{{ serial-baudrate }}", serial_baudrate_string);
    client_->println(dashboard_[i]);
  }
}

/**
* Get data from the URL
* @param name Word you are looking for
* @return Data if it was found
*/
String WebServer::GetUrlData(String name){
  //Get the position od the data
  int position = request_.indexOf(name) + name.length();
  String return_string = "";
  //Find the data, until find '/'
  while(request_[position] != '/'){
    return_string += request_[position];
    position++;
  }
  return return_string;
}

/**
* Analize URL, look for the page
*/
void WebServer::AnalizeURL(){
  if (request_.indexOf("/api/") > 0){Api();}
  else if (request_.indexOf("/start/") > 0){Start();}
  else if (request_.indexOf("/") > 0){Dashboard();}
}

String WebServer::GetBuffer(){
  return buffer_;
}

bool WebServer::GetFlag(){
  return api_flag_;
}

void WebServer::ClearFlag(){
  api_flag_ = false;
}
