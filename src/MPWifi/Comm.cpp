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

  String SD_command[20] = {"M20*", "M21*", "M22*", "M23 ", "M24*", "M25*", "M26*", "M27", "M28 ", "M29 ", "M30", "M31", "M32 ", "M33", "M34", "M36 "};
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
  * Serie
  *************************/
 /* local_buffer_ = "";
  bool receiveData = false; 
  while(Serial.available() > 0){
    local_buffer_ += Serial.read();
    receiveData = true;
  }
  if(receiveData)
    tcp_->SendCommand(local_buffer_);
*/

  //Get a response from the printer
  String command = "";
  while(Serial.available()) {
    char letra = Serial.read();
    command += letra;
  }
  //Send printer's response to the client
  tcp_->SendCommand(command);
      

  
  /*************************
  * TCP
  *************************/
  local_buffer_ = "";
  tcp_->WaitCommand();
  //If there are data on the TCP buffer
  if(tcp_->GetFlag()){
    local_buffer_ = tcp_->GetBuffer();
    //If data is not for SD card
    if(!this->FilterGcode()){
      //Send data to Printer
      Serial.print(local_buffer_);
      //delay(500);
      
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
    /*if(local_buffer_[pos+3] >= 48 && local_buffer_[pos+3] <= 57){
      return false;
    }*/
    
    //If the GCODE is on the list

    String command_back = "";
    if(pos > 0 ){
      int start_pos = 0;
      int end_pos = 0;
      String name = "";
      switch (i) {
        case 0:
          command_back = sd_card_->GetFileList();
          tcp_->SendCommand(command_back);
          break;
        case 1:
          command_back = sd_card_->Begin();
          tcp_->SendCommand(command_back);
          break;
        case 2:
          command_back = sd_card_->Close();
          tcp_->SendCommand(command_back);
          break;
        case 3:
          //Get FILE NAME
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
          name += ".GCO";


          sd_card_->OpenFile(name);

          command_back = "echo:Now fresh file: ";
          command_back += name;
          command_back += "\r\n";
          command_back += "File opened: 1737 Size: ";
          command_back += String(sd_card_->GetFileLenght());
          command_back += "\r\nFile selected\r\nok\r\n";

          tcp_->SendCommand(command_back);
          break;
        case 4:
          command_back = sd_card_->StartPrinting();
          tcp_->SendCommand(command_back);
          break;
        case 5:
          sd_card_->PausePrinting();
          break;
        case 7:
          command_back = sd_card_->Status();
          tcp_->SendCommand(command_back);
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
          name += ".GCO";

          command_back = sd_card_->BeginWrite(name);
          tcp_->SendCommand(command_back);
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
          name += ".GCO";
          command_back = sd_card_->StopWrite(name);
          tcp_->SendCommand(command_back);
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
          name += ".GCO";
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
          name += ".GCO";
          sd_card_->OpenFile(name);
          delay(100);
          sd_card_->StartPrinting();

          command_back = "echo:Now fresh file: ";
          command_back += name;
          command_back += "\r\nFile opened: 1833 Size: ";
          command_back += String(sd_card_->GetFileLenght());
          command_back += "\r\nFile selected\r\nok\r\n";

          tcp_->SendCommand(command_back);
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
