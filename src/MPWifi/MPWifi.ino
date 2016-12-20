#include "MpWifi.h"

MpWifi mpwifi;

void setup(){
  mpwifi.Boot();
}

void loop(){
  mpwifi.Run();
}
