#include "EEPROM.h"
#include <stdint.h>
#include "AdvancedEEPROM.h"

AdvancedEEPROM::AdvancedEEPROM(uint16_t EEPROMSize, uint8_t alarmCount, uint8_t WIFIcount){
  this->EEPROMSize=EEPROMSize;
  EEPROM.begin(EEPROMSize);
  this->init = this->initialized();
  if(!this->init){
    this->setAlarmCount(alarmCount);
    this->setWiFICount(WIFIcount);
    uint8_t reading = EEPROM.read(0);
    EEPROM.write(0, reading|0b10000000);
    this->init = true;
  }else{
    this->firstInitialization = false;
    this->alarmCount = this->readAlarmCount();
    this->WIFICount = this->readWIFICount();
  }
  this->NTPServerPath = this->alarmCount*3+this->WIFICount*96 + 2;
  this->endConnection();
}

void AdvancedEEPROM::startConnection(){
  EEPROM.begin(this->EEPROMSize);
}

void AdvancedEEPROM::endConnection(){
  EEPROM.commit();
  EEPROM.end();
}

bool AdvancedEEPROM::initialized(){
  uint8_t reading = EEPROM.read(0);
  if(reading>>7==1){
    return true;
  }else{
    return false;
  }
}

void AdvancedEEPROM::deInitialize(){
  uint8_t reading = EEPROM.read(0);
  EEPROM.write(0, reading&0b01111111);
  this->init = 0;
}

uint16_t AdvancedEEPROM::usedMemory(){
  return this->NTPServerPath+NTPServerLength;
}

void AdvancedEEPROM::clearMemory(){
  for(unsigned int i = 0; i<this->EEPROMSize;i++){
    EEPROM.write(i,0);
  } 
}

void AdvancedEEPROM::setWIFIMode(WIFIMode WM){
  uint8_t writing = 3;
  switch(WM){
    case STAM:
      writing = 0;
      break;
    case APM:
      writing = 1;
      break;
    case APpSTAM:
      writing = 2;
      break;
    default:
      writing = 3;
      break;
  }
  uint8_t reading = EEPROM.read(0);
  EEPROM.write(0, writing<<5|reading);
}

WIFIMode AdvancedEEPROM::readWIFIMode(){
  uint8_t reading = EEPROM.read(0);
  reading = (reading&0b01100000)>>5;
  switch(reading){
    case 0:
      return STAM;
    case 1:
      return APM;
    case 2:
      return APpSTAM;
    default:
      return turOffM;
  }
}

uint8_t AdvancedEEPROM::readAlarmCount(){
  return (EEPROM.read(0)&0b00011100)>>2;

}

uint8_t AdvancedEEPROM::readWIFICount(){
  return ((EEPROM.read(0))&0b00000011)+1;

}

void AdvancedEEPROM::setAlarmCount(uint8_t count){
  if(!init && count<8){
    uint8_t reading = EEPROM.read(0);
    reading = (reading&0b11100011)|(count<<2);
    EEPROM.write(0, reading);
    this->alarmCount = count;
  }
}

void AdvancedEEPROM::setWiFICount(uint8_t count){
  if(!init && count<4){
    uint8_t reading = EEPROM.read(0);
    reading = (reading&0b11111100)|(count);
    EEPROM.write(0, reading);
    this->WIFICount = count+1;
  }
}

void AdvancedEEPROM::writeCharArray(uint16_t path, char* s, uint8_t length){
  EEPROM.write(path, length+1);
  for(unsigned int i = 1; i<length+2; i++){
    EEPROM.write(path+i, s[i-1]);
  }
}

char* AdvancedEEPROM::readCharArray(uint16_t path){
  uint8_t length = EEPROM.read(path);
  this->name = new char[length];
  for(uint8_t i = 1; i< length; i++){
    name[i-1] = EEPROM.read(path+i);
  }
  name[length-1] = '\0';
  delete [] name;
  return name;
}

void AdvancedEEPROM::setTimeZone(float tZ){
  if(tZ>63 || tZ<-63){
    return;
  }
  uint8_t writing = 0;
  if(tZ<0){
    writing = 128;
  }

  if(abs(tZ)>(int)abs(tZ)){
    writing = writing|1;
  }
  uint8_t var = abs(tZ);
  writing = (var<<1) | writing;
  EEPROM.write(timeZonePath, writing);
}

float AdvancedEEPROM::readTimeZone(){
  uint8_t reading = EEPROM.read(timeZonePath);
  float var = (reading>>1)&0b00111111;
  if(reading&0b00000001){
    var+=0.5;
  }
  if(reading>>7){
    var=var*-1;
  }
  return var;
}

char* AdvancedEEPROM::readPassword(){
  return this->readCharArray(passPath);
}

void AdvancedEEPROM::setPassword(char* pass, uint8_t length){
  if(length<=30){
    this->writeCharArray(passPath, pass, length);
  }
}

alarm AdvancedEEPROM::readAlarm(uint8_t alarmNumber){
  alarm al;
  if(alarmNumber < this->alarmCount){
    al.hour = EEPROM.read(alarmPath+alarmNumber*3);
    al.minute = EEPROM.read(alarmPath+alarmNumber*3+1);
    al.schedule = EEPROM.read(alarmPath+alarmNumber*3+2);
  }
  return al;
}

void AdvancedEEPROM::setAlarm(uint8_t alarmNumber, alarm al){
  if(alarmNumber<this->alarmCount){
    EEPROM.write(alarmPath+alarmNumber*3, al.hour);
    EEPROM.write(alarmPath+alarmNumber*3+1, al.minute);
    EEPROM.write(alarmPath+alarmNumber*3+2, al.schedule);
  }
}

APData AdvancedEEPROM::readWIFI(uint8_t WIFINumber){
  APData AD;
  if(WIFINumber<this->WIFICount){
    EEPROM.get(WIFINumber*96+alarmPath+this->alarmCount*3, AD);
  }
  return AD;
}

void AdvancedEEPROM::setWIFI(uint8_t WIFINumber, APData AP){
  if(WIFINumber<this->WIFICount){
    EEPROM.put(WIFINumber*96+alarmPath+this->alarmCount*3, AP);
  }
}

void AdvancedEEPROM::setNTPName(char *name, uint8_t length){
  this->NTPServerLength = length+1;
  this->writeCharArray(this->NTPServerPath, name, length);
}

uint8_t AdvancedEEPROM::readNTPLength(){
  this->NTPServerLength = EEPROM.read(this->NTPServerPath);
  return this->NTPServerLength;
}

char* AdvancedEEPROM::readNTPName(){
  return this->readCharArray(this->NTPServerPath);
}
