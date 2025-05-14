#include "AdvancedEEPROM.h"
AdvancedEEPROM aE(1024, 4, 3);

String printBool(bool value){
  if(value){
    return "true";
  }else{
    return "false";
  }
}

void setup() {
  alarm al0;
  al0.hour = 12;
  al0.minute = 21;
  al0.schedule = 0b10000001;

  Serial.begin(115200);

  aE.startConnection();

  Serial.println("Memory initialization: "+printBool(aE.initialized()));
  if(!aE.initialized()){
    aE.setWIFIMode(turOffM);
    aE.setAlarm(0, al0);
  }
  aE.setTimeZone(2);
  Serial.println("WIFI mode: "+String(aE.readWIFIMode()));
  Serial.println("Alarm count: "+String(aE.readAlarmCount()));
  Serial.println("WIFI count: "+String(aE.readWIFICount()));
  Serial.println("Time zone: "+String(aE.readTimeZone(), 1));

  aE.endConnection();


  aE.startConnection();
  alarm tAl0 = aE.readAlarm(0);
  Serial.println(String(tAl0.hour)+":"+String(tAl0.minute)+" "+String(tAl0.schedule));
  aE.endConnection();
}

void loop() {
  // put your main code here, to run repeatedly:

}
