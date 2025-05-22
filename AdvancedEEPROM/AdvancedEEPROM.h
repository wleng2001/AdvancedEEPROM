#include <stdint.h>
#ifndef AdvancedEEPROM_h
#define AdvancedEEPROM_h
#include <EEPROM.h>
#include <math.h>
/*
0. byte usage (LSBF)
  7 - memory initialized 1 = true
  6 to 5 - WI-FI mode 0 = STA, 1 = AP, 2 = AP+STA 3 = turn off
  4 to 2 - alarms quantity //it can't be bigger than 7
  1 to 0 - network quantity + AP //it can't be bigger than 3
1. byte time zone in format
  7 - 1 = - 0 = +
  6 to 1 - entire value
  0 - 1 = +0.5
2.-32. user password
33. alarm data start
  0. - hour
  1. - minute
  2. - schedule 
    MSB 1 = turn on
    LSB - monday

alarms+WIFI length+3.-32. NTP server address
*/

const uint8_t passPath = 2;
const uint8_t alarmPath = 33;
const uint8_t timeZonePath = 1;

enum WIFIMode{
  STAM = 0,
  APM = 1,
  APpSTAM = 2,
  turOffM = 3,
};

typedef struct{
  uint8_t hour;
  uint8_t minute;
  uint8_t schedule; //MSB = 1 -> alarm turn on LSB = monday
} alarm;

typedef struct{
  char ssid[32];
  char password[63];
  uint8_t priority;
}APData;

class AdvancedEEPROM{
  private:
  uint16_t EEPROMSize;
  bool init = 0;
  uint8_t alarmCount = 0;
  uint8_t WIFICount = 0;
  unsigned int NTPServerPath = 0;
  uint8_t NTPServerLength = 0;
  char* name;

  void setAlarmCount(uint8_t count);
  void setWiFICount(uint8_t count);

  void writeCharArray(uint16_t path, char* s, uint8_t length);
  char* readCharArray(uint16_t path);
  
  public:
  AdvancedEEPROM(uint16_t EEPROMSize, uint8_t alarmCount, uint8_t WIFIcount);
  void startConnection();
  void endConnection();
  bool initialized();
  bool firstInitialization = true;
  void deInitialize();

  uint16_t usedMemory();
  void clearMemory();
  
  void setWIFIMode(WIFIMode WM);
  WIFIMode readWIFIMode();
  uint8_t readAlarmCount();
  uint8_t readWIFICount();

  void setTimeZone(float tZ);
  float readTimeZone();

  char* readPassword();
  void setPassword(char* pass, uint8_t length); //can't be wider than 30 chars

  alarm readAlarm(uint8_t alarmNumber);
  void setAlarm(uint8_t alarmNumber, alarm al);
  APData readWIFI(uint8_t WIFINumber);
  void setWIFI(uint8_t WIFINumber, APData AP);

  void setNTPName(char* name, uint8_t length);
  uint8_t readNTPLength();
  char* readNTPName();
  

};

#endif