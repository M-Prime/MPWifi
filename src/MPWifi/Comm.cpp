#include "Comm.h"
#include "WebServer.h"


Tcp tcp_t;
SdCard sd_card_t;

/**
* The constructor is used to initialize the objects
* and the commands for the SD control
*/
Comm::Comm(){
  tcp_ = &tcp_t;
  sd_card_ = &sd_card_t;

  String SD_command[20] = {"M20", "M21", "M22", "M23", "M24", "M25", "M26", "M27", "M28", "M29", "M30", "M31", "M32", "M33", "M34", "M36"};
  for(int i = 0; i < 20; i++)
    SD_command_[i] = SD_command[i];
}

/**
* Comm need to have access to the WebServer
* in order to read flags and buffer
* @param server A pointer to a WebServer object
*/
void Comm::ConnectWebServer(WebServer *server){
  web_server_ = server;
}

/**
* Setup method to run the TCP's setup
*/
void Comm::Setup(){
  tcp_->Setup();
}

/**
* Look for commands from TCP or microSD card.
* When data is find, we route them to the
* correct destination.
*/
void Comm::Check(){
  /*************************
  * TCP
  *************************/
  local_buffer_ = "";
  tcp_->WaitCommand();
  delay(10);
  //If there are data on the TCP buffer
  if(tcp_->GetFlag()){
    local_buffer_ = tcp_->GetBuffer();
    // Clear the serial buffer out
    while(Serial.available() > 0) char t = Serial.read();
    //If data is not for SD card
    if(!this->FilterGcode()){
      //Send data to Printer
      Serial.print(local_buffer_);
      delay(500);
      //Get a response from the printer
      String command = "";
      while(Serial.available()) {
        char letra = Serial.read();
        command += letra;
      }
      //Send printer's response to the client
      tcp_->SendCommand(command);
    }
    //Clear the TCP flag down
    tcp_->ClearFlag();

  }
  /*************************
  * microSD
  *************************/
  local_buffer_ = "";
  if(sd_card_->PrepareLine()){
    Serial.println("Done");
    local_buffer_ = sd_card_->GetBuffer();
    Serial.println(local_buffer_);
  }

}

/**
* Send data from web server to printer
*/
String Comm::CheckAndReturn(){
  //Check if there is data on both TCP and SD
  this->Check();
  
  if(web_server_->GetFlag()){
    local_buffer_ = web_server_->GetBuffer();
    local_buffer_.replace("%20", " ");
    if(!this->FilterGcode()){
      Serial.println(local_buffer_);
    }
    web_server_->ClearFlag();
    delay(100);
    String command = "";
    while(Serial.available()) {
      char letra = Serial.read();
      command += letra;
    }
    return command;
  }
}

/**
* It checks if the GCODE is for the microSD car
*/
bool Comm::FilterGcode(){
  //IF it is a SD GCODE
  for(int i = 0; i < 20; i++){
    //Look for the GCODE on the list
    int pos = local_buffer_.indexOf(SD_command_[i]);
    if(local_buffer_[pos+3] >= 48 && local_buffer_[pos+3] <= 57){
      return false;
    }
    //If the GCODE is on the list
    if(pos > 0 ){
      int start_pos = 0;
      int end_pos = 0;
      String name = "";
      switch (i) {
        case 0:
          tcp_->SendCommand(sd_card_->GetFileList());
          break;
        case 1:
          sd_card_->Begin();
          tcp_->SendCommand("ok");
          break;
        case 2:
          sd_card_->Close();
          tcp_->SendCommand("ok");
          break;
        case 3:
          //Get both start and end string's position
          start_pos = local_buffer_.indexOf("M23");
          end_pos = local_buffer_.indexOf(".gco");
          if(end_pos < 0)
            end_pos = local_buffer_.indexOf(".GCO");
          //Find file name
          name = "";
          for (int i = start_pos; i < end_pos; i++) {
            name += local_buffer_[i];
          }
          name += ".gcode";
          sd_card_->OpenFile(name);
          tcp_->SendCommand("ok");
          break;
        case 4:
          sd_card_->StartPrinting();
          break;
        case 5:
          sd_card_->PausePrinting();
          break;
        case 7:
          sd_card_->Status();
          break;
        case 8:
          //Get both start and end string's position
          start_pos = local_buffer_.indexOf("M28");
          end_pos = local_buffer_.indexOf(".gco");
          if(end_pos < 0)
            end_pos = local_buffer_.indexOf(".GCO");
          //Find file name
          name = "";
          for (int i = start_pos; i < end_pos; i++) {
            name += local_buffer_[i];
          }
          name += ".gcode";
          sd_card_->BeginWrite(name);
          break;
        case 9:
          //Get both start and end string's position
          start_pos = local_buffer_.indexOf("M29");
          end_pos = local_buffer_.indexOf(".gco");
          if(end_pos < 0)
            end_pos = local_buffer_.indexOf(".GCO");
          //Find file name
          name = "";
          for (int i = start_pos; i < end_pos; i++) {
            name += local_buffer_[i];
          }
          name += ".gcode";
          sd_card_->StopWrite(name);
          break;
        case 10:
          //Get both start and end string's position
          start_pos = local_buffer_.indexOf("M29");
          end_pos = local_buffer_.indexOf(".gco");
          if(end_pos < 0)
            end_pos = local_buffer_.indexOf(".GCO");
          //Find file name
          name = "";
          for (int i = start_pos; i < end_pos; i++) {
            name += local_buffer_[i];
          }
          name += ".gcode";
          sd_card_->DeleteFile(name);
          break;
        case 12:
          //Get both start and end string's position
          start_pos = local_buffer_.indexOf("M32");
          end_pos = local_buffer_.indexOf(".gco");
          if(end_pos < 0)
            end_pos = local_buffer_.indexOf(".GCO");
          //Find file name
          name = "";
          for (int i = start_pos; i < end_pos; i++) {
            name += local_buffer_[i];
          }
          name += ".gcode";
          sd_card_->OpenFile(name);
          delay(100);
          sd_card_->StartPrinting();
          break;
        case 16:
          /***************************
          ****************************
          ****************************
          ***************************/
          break;
      }
      //Send dummy gcode
      this->ChecksumAndSend();
      //Return true, We don't want the GCODE will be sent to the printer
      return true;
    }
  }
  //IF Write to SD is open, send the GCODE to the microSD
  if(sd_card_->GetWriteOpen()){
    delay(10);
    sd_card_->SetFileLine(local_buffer_);
    tcp_->SendCommand("ok");
    return true;
  }
  //IF not SD & not write to SD
  //Return false, The GCODE will be sent to the printer
  return false;
}

/**
* The function calc the CheckSum and it sends a dummy GCODE to the printer
*/
void Comm::ChecksumAndSend(){
  int pos = 0;
  String command = "";
  //Get the last line number sent to the printer
  while(local_buffer_[pos] != ' ')
    command += local_buffer_[pos++];
  //Set the dummy GCODE
  command += " M105";
  //Calc the checksum
  byte checksum = 0;
  for(int i = 0; i < command.length(); i++)
    checksum ^= command[i];

  checksum_ = checksum;
  //Add checksum at the line's end
  command += '*';
  command += checksum_;
  //Send line and clear Serial buffer
  Serial.println(command);
  while(Serial.available()>0)
    char c = Serial.read();
}
