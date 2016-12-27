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
    } else if (gcode == "resetMPWifi"){
      client_->println("Reset done");
      delay(200);
      ESP.restart();
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
  String message = "";
  //If form has been sent
  if(request_.indexOf("change=true") > 0){
    //Get station data
    String ssid = this->GetUrlParameter("ssid=");
    String pass = this->GetUrlParameter("pass=");
    String host_name = this->GetUrlParameter("hostname=");
    mpwifi_->SetStation(ssid, pass, host_name);

    //Get work_mode
    int work_mode = 1;
    work_mode = this->GetUrlParameter("workmode=")[0] -48;
    Serial.println(work_mode);
    mpwifi_->SetWorkMode(work_mode);

    //Get AP data
    String apssid = this->GetUrlParameter("apssid=");
    String appass = this->GetUrlParameter("appass=");
    mpwifi_->SetAp(apssid, appass);

    //Get User data
    String a_c_user = this->GetUrlParameter("acuser=");
    String a_c_pass = this->GetUrlParameter("acpassword=");
    mpwifi_->SetUser(a_c_user, a_c_pass);

    //Create first file
    file_system_->SetFile("first.txt", "existe");
    //Reset message
    message = "<h1>Changes will take place once you <a href='/start/resetMPWifi/'>reset</a> MPWifi.</h1>";
    client_->println(message);
  } else {
    //Prepare and send html response
    for(int i = 0; i < 4; i++){
      start_[i].replace("{{ reset_message }}", message);
      start_[i].replace("{{ ssid }}", mpwifi_->GetStationSsid());
      start_[i].replace("{{ pass }}", mpwifi_->GetStationPass());
      start_[i].replace("{{ hostname }}", mpwifi_->GetStationHostname());
      start_[i].replace("{{ ap-ssid }}", mpwifi_->GetApSsid());
      start_[i].replace("{{ ap-pass }}", mpwifi_->GetApPass());
      start_[i].replace("{{ a-c-user }}", mpwifi_->GetACUser());
      start_[i].replace("{{ a-c-pass }}", mpwifi_->GetACPass());
      client_->println(start_[i]);
    }
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
 * Login page
 */
void WebServer::LogIn(){
  //Chec data and send to MPWIFI
  if(request_.indexOf("change=true") > 0){
    //Get station data
    String user = this->GetUrlParameter("user=");
    String pass = this->GetUrlParameter("pass=");

     if(mpwifi_->CheckUser(user, pass))
        client_->println("<html><head><meta http-equiv=\"Refresh\" content=\"0;url=/\"></head><body><h1>Wait a second</h1></body></html>");
  } else if(request_.indexOf("resetMPWifi") > 0){
      client_->println("Reset done");
      delay(200);
      ESP.restart();
  } 
  
  
  client_->println(file_system_->GetFile("login.html"));
}

/**
 * 404
 */
 void WebServer::NotFound(){
    String content = "<h1>404</h1><br>The content you are asking for are not found on MPWifi";
    client_->println(content);
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
* Get a value like ?name=value
* @param name Parameter's name
* @return The value, a String
*/
String WebServer::GetUrlParameter(String name){
  //Get the position od the data
  int position = request_.indexOf(name) + name.length();
  String return_string = "";
  //Find the data, until find '/'
  while(request_[position] != '&'){
    return_string += request_[position];
    position++;
  }
  return return_string;
}

/**
* Analize URL, look for the page
*/
void WebServer::AnalizeURL(){
  //If first boot, then charge Start
  if(!file_system_->ExistFile("first.txt")){
    this->Start();
  } else if(mpwifi_->GetSession()){
    if (request_.indexOf("/api/") > 0){this->Api();}
    else if (request_.indexOf("/start/") > 0){this->Start();}
    else if (request_.indexOf("/") > 0){this->Dashboard();}
    else {this->NotFound();} 
  } else {
    this->LogIn();
  }
}

/**
 * Look for old login
 */
 bool WebServer::Session(){
  return false;
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
