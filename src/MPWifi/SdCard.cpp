#include "SdCard.h"


/**
* Initialize
*/
SdCard::SdCard(){
  buffer_ = "";
  file_ = "";
  file_open_ = false;
  printing_ = false;
  write_open_ = false;
  line_ = 0;
  my_file_ = NULL;
}

/**
* Begin the microSD
*/
void SdCard::Begin(){
  SD.begin();
}

/**
* Get filelist
* @return The list, a String
*/
String SdCard::GetFileList(){
  String list = "Begin file list\r\nCOG_TR~1.GCO\r\nEnd file list\r\nok\r\n";

  return list;
}

/**
* Open a file
* @param name File's name and extension
*/
void SdCard::OpenFile(String name){
  //Create a file
  File file;
  file = SD.open(name, FILE_READ);
  file_lenght_ = file.size();
  my_file_ = &file;
  //Set the flag up
  file_open_ = true;
}

/**
* Start printing from file
*/
void SdCard::StartPrinting(){
  //Set the flag up
  printing_ = true;
}

/**
* Pause printing from file
*/
void SdCard::PausePrinting(){
  //Set the flag down
  printing_ = false;
}

/**
* Prepare the line, it will be sent to the printer
*/
bool SdCard::PrepareLine(){
  if(!file_open_) return false;
  if(!printing_) return false;

  String serial_buffer;
  while(Serial.available() > 0){
    serial_buffer += Serial.read();
  }
/*  if(serial_buffer.indexOf("ok") < 0)
    return false;
*/
  buffer_ = "mil linea";

  byte_pos_ = my_file_->position();

  line_++;
  return true;
}

/**
* Close file
*/
void SdCard::CloseFile(){
  my_file_->close();
  file_open_ = false;
  printing_ = false;
}

/**
* Status when print from file is enable
*/
void SdCard::Status(){
  //SD printing byte 0/478901
  //ok
  String send = "SD printing byte ";
  send += byte_pos_;
  send += "/";
  send += file_lenght_;
  //Serial.println(send);
  //Serial.println("ok");

}

/**
* Start writting on the microSD
* @param name File's name and extension
*/
void SdCard::BeginWrite(String name){
  //Create a file
  File file;
  file = SD.open(name, FILE_WRITE);
  my_file_ = &file;
  //Set the flag up
  write_open_ = true;
}

/**
* Stop writting on the microSD
* @param name File's name and extension
*/
void SdCard::StopWrite(String name){
  //SD.close();
  //Set the flag down
  write_open_ = false;
}

/**
* Write a line
* @param line The line, a String
*/
void SdCard::SetFileLine(String line){
  //Send content to SD
}

/**
* Look for a file
* @param name File's name and extension
* @return True if file exists
*/
bool SdCard::ExistFile(String name){
  return SD.exists(name);
}

/**
* Delete a  file
* @param name File's name and extension
*/
bool SdCard::DeleteFile(String name){
  return SD.remove(name);
}

String SdCard::GetBuffer(){
  return buffer_;
}

void SdCard::SetFileOpen(){
  file_open_ = true;
}

bool SdCard::GetFileOpen(){
  return file_open_;
}

void SdCard::SetPrinting(){
  printing_ = false;
}

bool SdCard::GetPrinting(){
  return printing_;
}

bool SdCard::GetWriteOpen(){
  return write_open_;
}

void SdCard::SetWriteOpen(){
  write_open_ = false;
}

void SdCard::Close(){
  //Serial.println("SD close");
}
