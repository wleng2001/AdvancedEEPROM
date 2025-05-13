#include <stdint.h>
#ifndef AdvancedEEPROM_h
#define AdvancedEEPROM_h
#include <EEPROM.h>

/*
1. byte usage (LSBF)
  7 - memory initialized 1 = true
  6 to 5 - WI-FI mode 0 = STA, 1 = AP, 2 = AP+STA 3 = turn off
  4 to 2 - alarms quantity //it can't be bigger than 7
  1 to 0 - network quantity + AP //it can't be bigger than 3
2. byte time zone in format
  7 - 1 = + 0 = -
  6 to 1 - entire value
  0 - 1 = +0.5 
3.-32. NTP server address
*/
#define AlarmPath

typedef struct{
  uint8_t hour;
  uint8_t minute;
  uint8_t schedule; //MSB = 1 -> alarm turn on LSB = monday
} alarm;

typedef struct{
  char ssid[32];
  char password[63];
}APData;

class AdvancedEEPROM{
  private:
  uint16_t EEPROMSize;
  bool init = 0;
  uint8_t alarmCount = 0;
  uint8_t WIFICount = 0;

  void setAlarmCount(uint8_t count);
  void setWiFICount(uint8_t count);
  
  public:
  AdvancedEEPROM(uint16_t EEPROMSize, uint8_t alarmCount, uint8_t WIFIcount);
  void startConnection();
  void endConnection();
  bool initialized();
  
  uint8_t readAlarmCount();
  uint8_t readWIFICount();

  alarm readAlarm(uint8_t alarmNumber);
  void setAlarm(uint8_t alarmNumber, alarm al);
  APData readWIFI(uint8_t WIFINumber);
  void setWIFI(uint8_t WIFINumber, APData AP);

};

#endif