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
String SdCard::Begin(){

  //SD.begin();
  String back = "echo:SD card ok\r\n";
	back += "ok";

  return back;
}

/**
* Get filelist
* @return The list, a String
*/
String SdCard::GetFileList(){

  String list = "Begin file list\r\n";
	list += "COG_TR~1.GCO\r\n";
	list += "PP_YZ_~1.GCO\r\n";
	list += "LITOGR~1.GCO\r\n";
	list += "/MPRIME~2/PP_BIG~1.GCO\r\n";
	list += "/MPRIME~2/PP_SMA~1.GCO\r\n";
	list += "End file list\r\n";
	list += "ok\r\n";
  //String list = "Begin file list\r\nCOG_TR~1.GCO\r\nEnd file list\r\nok\r\n";

  return list;
}

/**
* Open a file
* @param name File's name and extension
*/
void SdCard::OpenFile(String name){
  //Create a file
/*  File file;
  file = SD.open(name, FILE_READ);
  file_lenght_ = file.size();
  my_file_ = &file;*/
  //Set the flag up
  file_open_ = true;
}

/**
* Start printing from file
*/
String SdCard::StartPrinting(){
  //Set the flag up
  printing_ = true;
  String back = "ok";
  return back;
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

  byte_pos_ = 0;//my_file_->position();

  line_++;
  return true;
}

/**
* Close file
*/
void SdCard::CloseFile(){
  //my_file_->close();
  file_open_ = false;
  printing_ = false;
}

/**
* Status when print from file is enable
*/
String SdCard::Status(){
  //SD printing byte 0/478901
  String back = "SD printing byte ";
  back += byte_pos_;
  back += "/";
  back += file_lenght_;
  back += "\r\n";

  return back;
}

/**
* Start writting on the microSD
* @param name File's name and extension
*/
String SdCard::BeginWrite(String name){
  //Create a file
  /*File file;
  file = SD.open(name, FILE_WRITE);
  my_file_ = &file;
  //Set the flag up
  */
  write_open_ = true;
  String back = "echo:Now fresh file: pruebaEscritura.gco";
	back += "open failed, File: 1641.";
	back += "ok";
   return back;
}

/**
* Stop writting on the microSD
* @param name File's name and extension
*/
String SdCard::StopWrite(String name){
  //SD.close();
  //Set the flag down
  write_open_ = false;
  String back = "ok";
  return back;
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
  return false;//SD.remove(name);
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

unsigned long SdCard::GetFileLenght(){
  return file_lenght_;
}

String SdCard::Close(){
  return "ok";
}
