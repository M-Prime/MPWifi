#include <Arduino.h>
#include <FS.h>

class FileSystem{
  public:
    String GetFile(String name);
    String SetFile(String name, String content);
    bool ExistFile(String name);
    bool DeleteFile(String name);

};
