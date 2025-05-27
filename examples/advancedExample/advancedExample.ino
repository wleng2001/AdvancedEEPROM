#include <AdvancedEEPROM.h>
AdvancedEEPROM aE(1024, 4, 3);

String printBool(bool value){ //fucntion to print value boolean variable
  if(value){
    return "true";
  }else{
    return "false";
  }
}

void setup() {
  alarm al0; //create alarm struct, which will be template for data to save in memory
  al0.hour = 12;
  al0.minute = 21;
  al0.schedule = 0b10000001;
  alarm al[4];
  for(uint8_t i = 0; i<4; i++){ //create array of alarm struct
    al[i].hour = al0.hour+i;
    al[i].minute = al0.minute+i;
    al[i].schedule = al0.schedule+i; 
  }

  APData AP[4]; //create array of APData struct
  for(uint8_t i = 0; i<4; i++){
    ("T3sting"+String(i)).toCharArray(AP[i].ssid, 32);
    ("Str0ngPa33w0rd"+String(i)).toCharArray(AP[i].password, 63);
    AP[i].priority = 1+i;
  }

  aE.startConnection(); //You must do it if you want read and write something to memory. It can be used once before block of these functions.
  Serial.println("Memory first initialization: "+printBool(aE.firstInitialization));
  if(aE.firstInitialization==true){ //If memory is first time initialized save data to memory
    aE.setWIFIMode(turOffM);
    aE.setTimeZone(2);
    aE.setPassword("Very 3tr0ng pa33w0rd", 20);
    for(uint8_t i = 0; i<aE.readAlarmCount(); i++){
      aE.setAlarm(i, al[i]);
    }
    for(uint8_t i = 0; i<aE.readWIFICount(); i++){
      aE.setWIFI(i, AP[i]);
    }
    
    char NTP[] = "tempus1.gum.gov.pl";
    aE.setNTPName(NTP, 18);
    aE.endConnection(); //end connection - save changes in memory
    aE.startConnection();
  }
  //write data from memory
  Serial.println("WIFI mode: "+String(aE.readWIFIMode()));
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
  Serial.println("Used memory: "+String(aE.usedMemory()));
  aE.endConnection();
}

String input = "";
bool aEState = aE.initialized();

void loop() {
  if(Serial.available()){
    input = Serial.readStringUntil('\n');
  }
  if(input=="clr"){ //if write clr in terminal then esp clear memory and deinitializes memory
    input = "";
    aE.startConnection();
    aE.deInitialize();
    aE.clearMemory();
    aE.endConnection();
  }else{
    if(input!=""){
      Serial.println(input);
      input = "";
    }
  }
  aE.startConnection();
  if(aEState!=aE.initialized()){
    Serial.println("Serial status changed: "+printBool(aE.initialized()));
    aEState = aE.initialized();
  }
  aE.endConnection();
}
