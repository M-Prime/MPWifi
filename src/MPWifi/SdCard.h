#ifndef SDCARD_H
#define SDCARD_H

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

class SdCard{
private:
  String buffer_;
  String file_;
  bool file_open_;
  bool printing_;
  bool write_open_;
  int line_;
  unsigned long byte_pos_;
  unsigned long file_lenght_;
  File *my_file_;

public:
  SdCard();
  void Begin();
  String GetFileList();

  void OpenFile(String name);
  void StartPrinting();
  void PausePrinting();
  bool PrepareLine();
  void CloseFile();

  void Status();

  void BeginWrite(String name);
  void StopWrite(String name);

  void SetFile(String name, String content);
  void SetFileLine(String content);
  bool ExistFile(String name);
  bool DeleteFile(String name);
  void SetFile(String name);

  String GetBuffer();
  void SetFileOpen();
  bool GetFileOpen();
  void SetPrinting();
  bool GetPrinting();
  bool GetWriteOpen();
  void SetWriteOpen();

  void Close();
};

#endif // SDCARD_H
