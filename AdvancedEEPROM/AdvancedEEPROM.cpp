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
    this->alarmCount = this->readAlarmCount();
    this->WIFICount = this->readWIFICount();
  }
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
  return (EEPROM.read(0))&0b00000011;

}

void AdvancedEEPROM::setAlarmCount(uint8_t count){
  if(!init && count<8){
    uint8_t reading = EEPROM.read(0);
    reading = (reading&0b11100011)|(count<<2);
    EEPROM.write(0, reading);
  }
}

void AdvancedEEPROM::setWiFICount(uint8_t count){
  if(!init && count<4){
    uint8_t reading = EEPROM.read(0);
    reading = (reading&0b11111100)|(count);
    EEPROM.write(0, reading);
  }
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
  writing = var<<1 | writing;
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

alarm AdvancedEEPROM::readAlarm(uint8_t alarmNumber){
  alarm al;
  if(alarmNumber < this->alarmCount){
    EEPROM.get(alarmPath+alarmNumber, al);
  }
  return al;
}

void AdvancedEEPROM::setAlarm(uint8_t alarmNumber, alarm al){
  if(alarmNumber<this->alarmCount){
    EEPROM.put(alarmPath+alarmNumber, al);
  }
}
