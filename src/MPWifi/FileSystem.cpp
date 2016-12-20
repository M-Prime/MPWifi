#include "FileSystem.h"

/**
* Get the file's content
* @param name File's name and extension
* @return The file's content, a String
*/
String FileSystem::GetFile(String name){
  SPIFFS.begin();
  String values = "";
  String file = "/";
  file += name;
   fs::File f = SPIFFS.open(file, "r");
  if (!f) {
    Serial.print("\nFile open failed: ");
    Serial.print(name);
  } else {
    values =  f.readStringUntil('\r');
    f.close();
    delay(100);
    SPIFFS.end();
    return values;
  }
  SPIFFS.end();
  delay(100);
}

/**
* Write content in a file, if the file already exists it will be trucated
* @param file_name File's name and extension
* @param content File's content
*/
void FileSystem::SetFile(String file_name, String content){
  String file = "/";
  file += file_name;

  SPIFFS.begin();
  fs::File f = SPIFFS.open(file, "w");
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

/**
* Look for a file
* @param name File's name and extension
* @return True if file was found and false if file wasn't found
*/
bool FileSystem::ExistFile(String name){
  SPIFFS.begin();
  String file = "/";
  file += name;
  return SPIFFS.exists(file);
  delay(100);
  SPIFFS.end();
  delay(100);
}

/**
* Delete a file if exists
* @param name File's name and extension
* @return True if file was deleted
*/
bool FileSystem::DeleteFile(String name){
  SPIFFS.begin();
  String file = "/";
  file += name;
  return SPIFFS.remove(file);
  delay(100);
  SPIFFS.end();
  delay(100);
}
