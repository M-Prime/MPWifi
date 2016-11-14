#include <WebServer.h>
#include "../Comm/Comm.h"

WiFiServer server(80);

WebServer::WebServer(){
  internal_server_ = &server;
}

void WebServer::SetObjetcs(Comm *comm, FileSystem *file_system){
  comm_ = comm;
  file_system_ = file_system;
}

void WebServer::SetSettings(String ssid, String pass, String host_name, String ap_ssid, String ap_pass, int work_mode){
  ssid_ = ssid;
  pass_ = pass;
  host_name_ = host_name;
  ap_ssid_ = ap_ssid;
  ap_pass_ = ap_pass;
  work_mode_ = work_mode;
}

void WebServer::Setup(){
  //Start the web server
   server.begin();
}

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

void WebServer::Api(){
  String html_code = "";
  String gcode = GetUrlData("/api/");
  if(gcode != ""){
    buffer_ = gcode;
    html_code += comm_->CheckAndReturn();
  } else {
    html_code += "nothing";
  }
  client_->println(html_code);
  buffer_ = "";
}

void WebServer::Start(){

  String html_code = file_system_->GetFile("start");
  html_code.replace("--ssid--", ssid_);
  html_code.replace("--pass--", pass_);
  html_code.replace("--hostname--", host_name_);
  html_code.replace("--ap-ssid--", ap_ssid_);
  html_code.replace("--ap-pass--", ap_pass_);

  client_->println(html_code);
}

void WebServer::Dashboard(){
  Prepare("dashboard", "MpWifi dashboard content");

  String html_code = file_system_->GetFile("dashboard");

  client_->println(html_code);
}

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

void WebServer::AnalizeURL(){
  if (request_.indexOf("/api/") > 0){Api();}
  else if (request_.indexOf("/start/") > 0){Start();}
  else if (request_.indexOf("/") > 0){Dashboard();}
}

String WebServer::GetBuffer(){
  return buffer_;
}

void WebServer::Prepare(String page, String content){
  String file = "/";
  file += page;
  file += ".txt";

  SPIFFS.begin();
  File f = SPIFFS.open(file, "w");
  if (!f) {
      Serial.print("\nFile open for write failed ");
  } else {
    f.println(content);
  }
  f.close();
  delay(100);
  SPIFFS.end();


  delay(100);

}
