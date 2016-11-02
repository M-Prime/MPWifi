#include "FileSystem.h"


String FileSystem::GetFile(String name){
  String value = "";
  String file = "/";
  file += name;
  file += ".txt";
   File f = SPIFFS.open(file, "r");
  if (!f) {
    Serial.print("\nFile open failed: ");
    Serial.print(name);
  } else {
    value =  f.readStringUntil('\r');
    value.replace("\n", "");
    f.close();
    delay(100);
    return value;
  }
}

String FileSystem::SetFile(String name, String content){
  String file = "/";
  file += name;
  file += ".txt";
  File f = SPIFFS.open(file, "w");
  if (!f) {
      Serial.print("\nFile open for write failed ");
      Serial.println(name);
  } else {
    f.println(content);
  }
  f.close();
    delay(100);
}

bool FileSystem::ExistFile(String name){
  String file = "/";
  file += name;
  file += ".txt";
  return SPIFFS.exists(file);
}

bool FileSystem::DeleteFile(String name){
  String file = "/";
  file += name;
  file += ".txt";
  return SPIFFS.remove(file);
}
