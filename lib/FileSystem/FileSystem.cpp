#include "FileSystem.h"

String FileSystem::GetFile(String name){
  SPIFFS.begin();

  String values = "";
  String file = "/";
  file += name;
  file += ".txt";
   File f = SPIFFS.open(file, "r");
  if (!f) {
    Serial.print("\nFile open failed: ");
    Serial.print(name);
  } else {
    values =  f.readStringUntil('\r');
    values.replace("\n", "");
    f.close();
    delay(100);
    SPIFFS.end();
    return values;
  }
  SPIFFS.end();
}

String FileSystem::SetFile(String name, String content){
  SPIFFS.begin();
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
  SPIFFS.end();
}

bool FileSystem::ExistFile(String name){
  SPIFFS.begin();
  String file = "/";
  file += name;
  file += ".txt";
  return SPIFFS.exists(file);
  delay(100);
  SPIFFS.end();
}

bool FileSystem::DeleteFile(String name){
  SPIFFS.begin();
  String file = "/";
  file += name;
  file += ".txt";
  return SPIFFS.remove(file);
  delay(100);
  SPIFFS.end();
}
