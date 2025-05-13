#include "EEPROM.h"
#include <stdint.h>
#include "AdvancedEEPROM.h"

AdvancedEEPROM::~AdvancedEEPROM(uint16_t EEPROMSize, uint8_t alarmCount, uint8_t WIFIcount){
  this->EEPROMSize=EEPROMSize;
  EEPROM.begin(EEPROMSize);
  this->init = this->initialized();
  if(!this->init){
    this->setAlarmCount(alarmCount);
    this->setWiFICount(WIFIcount);
    uint8_t reading = EEPROM.read(0);
    EEPROM.write(0, reading|0b10000000);
    this->init = true;
  }
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

uint8_t AdvancedEEPROM::readAlarmCount(){
  return reading = (EEPROM.reaad(0)>>2)&0b00000111;

}

uint8_t AdvancedEEPROM::setAlarmCount(uint8_t count){
  if(!init && count<8){
    uint8_t reading = EEPROM.read(0);
    reading = (reading&0b11100011)|(count<<2);
    EEPROM.write(0, reading);
  }
}

uint8_t AdvancedEEPROM::setWiFICount(uint8_t count){
  if(!init && count<4){
    uint8_t reading = EEPROM.read(0);
    reading = (reading&0b11111100)|(count);
    EEPROM.write(0, reading);
  }