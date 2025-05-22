#include "AdvancedEEPROM.h"
AdvancedEEPROM aE(1024, 4, 3); //initialize memory with 1024 bytes length, 4 alarm and 4 wifi (3 STA + 1 AP)
/*
max memory length is 4096 b
max amounts of alamrs is 7 and min is 0
max amounts of wifi is 4 and min is 1
*/

void setup() {
  alarm al0;
  al0.hour = 12;
  al0.minute = 21;
  al0.schedule = 0b10000001; //turn on and activate in monday

  alarm al[4];
  for(uint8_t i = 0; i<4; i++){
    al[i].hour = al0.hour+i;
    al[i].minute = al0.minute+i;
    al[i].schedule = al0.schedule+i; 
  }

  APData AP[4];
  for(uint8_t i = 0; i<4; i++){
    ("T3sting"+String(i)).toCharArray(AP[i].ssid, 32);
    ("Str0ngPa33w0rd"+String(i)).toCharArray(AP[i].password, 63);
    AP[i].priority = 1+i;
  }

  Serial.begin(115200);

  aE.startConnection(); //you must use it if before you want read something or write to memory
  Serial.println("Memory first initialization: "+String(aE.firstInitialization)); //firstInitialization give information that memory was initialized in the boot up (doesn't have write data in memory)

  //save data to memory
  aE.setWIFIMode(turOffM);
  aE.setTimeZone(2); //max value -63 or 63 with accuracy to 0.5
  aE.setPassword("Very 3tr0ng pa33w0rd", 20); //Max length is 30. Second parameter informs about length of the password

  for(uint8_t i = 0; i<aE.readAlarmCount(); i++){
    aE.setAlarm(i, al[i]);
  }

  for(uint8_t i = 0; i<aE.readWIFICount(); i++){
    aE.setWIFI(i, AP[i]);
  }
    
  char NTP[] = "tempus1.gum.gov.pl";
  aE.setNTPName(NTP, 18); //length limited by memory lengths and lengths of rest of the data
  aE.endConnection(); //it confirms writing data to memory

  //read data
  aE.startConnection();
    
  Serial.println("WIFI mode: "+String(aE.readWIFIMode()));
  /*
  Available modes:
  STAM - STA mode
  APM - AP mode
  APpSTAM - AP + STA mode
  turnOffM - turn off wifi 
  */
  Serial.println("Alarm count: "+String(aE.readAlarmCount()));
  Serial.println("WIFI count: "+String(aE.readWIFICount()));
  Serial.println("Time zone: "+String(aE.readTimeZone(), 1));

  Serial.println("Password: "+String(aE.readPassword()));
  
  Serial.println("NTP server length: "+String(aE.readNTPLength()));
  Serial.println("NTP server: "+String(aE.readNTPName()));
  
  aE.endConnection();


  aE.startConnection();
  for(uint8_t i = 0; i<aE.readAlarmCount(); i++){
    alarm tAl0 = aE.readAlarm(i);
    Serial.println(String(i)+".\t"+String(tAl0.hour)+":"+String(tAl0.minute)+" "+String(tAl0.schedule));
  }
  for(uint8_t i = 0; i<aE.readWIFICount(); i++){
    APData AP = aE.readWIFI(i);
    Serial.println(String(i)+".\t"+AP.ssid+"\t"+AP.password+"\t"+String(AP.priority));
  }
  Serial.println("Used memory: "+String(aE.usedMemory())); //write information about used memory in bytes
  aE.endConnection();
}

String napis;

void loop() {
}
