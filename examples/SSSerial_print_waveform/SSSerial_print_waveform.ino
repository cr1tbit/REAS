#include <aREAS.h>

SSSerial s(3);

uint8_t test = 0;

void setup() {
  s.begin(20);
  Serial.begin(9600);
}

void loop() {
  s.send((char)test);
  Serial.print("int ");
  Serial.print(test);
  Serial.print(": ");
  Serial.println(getWaveformFromChar((char)test));
  Serial.println();
  test++;
  delay(4000);
}

String getWaveformFromChar(char c){
  String out = "~_____===|";
  uint8_t bitmask = 0b10000000;
  for(int i=0;i<8;i++){
    if(c&bitmask){
      out +="==|";
    }
    else{
      out +="__|";
    }
    bitmask >>= 1;
  }
  out +="===_____~";
  return out;
}

