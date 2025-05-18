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
  Serial.println("Memory first initialization: "+printBool(aE.firstInitialization));
  if(aE.firstInitialization==true){
    aE.setWIFIMode(turOffM);
    aE.setAlarm(0, al0);
    aE.setTimeZone(2);
    char NTP[] = "tempus1.gum.gov.pl";
    aE.setNTPName(NTP, 18);
    aE.endConnection(true);
    aE.startConnection();
  }
  Serial.println("WIFI mode: "+String(aE.readWIFIMode()));
  Serial.println("Alarm count: "+String(aE.readAlarmCount()));
  Serial.println("WIFI count: "+String(aE.readWIFICount()));
  Serial.println("Time zone: "+String(aE.readTimeZone(), 1));
  
  Serial.println("NTP server length: "+String(aE.readNTPLength()));
  Serial.println("NTP server: "+String(aE.readNTPName()));
  
  aE.endConnection(true);


  aE.startConnection();
  alarm tAl0 = aE.readAlarm(0);
  Serial.println(String(tAl0.hour)+":"+String(tAl0.minute)+" "+String(tAl0.schedule));
  aE.endConnection(true);
}

String input = "";
bool aEState = aE.initialized();

void loop() {
  if(Serial.available()){
    input = Serial.readStringUntil('\n');
  }
  if(input=="clr"){
    input = "";
    aE.startConnection();
    aE.deInitialize();
    aE.clearMemory();
    aE.endConnection();
  }
  aE.startConnection();
  if(aEState!=aE.initialized()){
    Serial.println("Serial status changed: "+printBool(aE.initialized()));
    aEState = aE.initialized();
  }
  aE.endConnection();
}
