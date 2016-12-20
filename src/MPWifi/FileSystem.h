#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <Arduino.h>
#define FS_NO_GLOBALS
#include <FS.h>

class FileSystem{
  public:
    String GetFile(String name);
    void SetFile(String name, String content);
    bool ExistFile(String name);
    bool DeleteFile(String name);

};

#endif // FILESYSTEM_H
