# Advanced EEPROM library for ESP8266
## Description
It's library for ESP8266, which allows to save alarms, wifi data, user password, time zone and NTP server name in memory.
# Introduction
If you want use the library first you must define object.
```cpp
AdvancedMemory aE(uint16_t EEPROMSize, uint8_t alarmCount, uint8_t WIFIcount);
```
`EEPROMSize` It tells for constructor length of the memory. If you give other value than in previous initialization data can't be read correctly. Max value is 4096 and min is 32.<br>
`alarmCount` If memory is initialized first time it defines how much alarms can be stored in memory. Max value is 7 min is 0.<br>
`WIFIcount` If memory is initialized first time it defines how much `APData` can be stored in memory. Max value is 3 min is 0.<br>
Class to create `AdvancedMemory` object has public methods, which can divide to some group.
## Start and end methods
```cpp
void startConnection(); //It must be used before read or write data to memory.
void endConnection(); //It confirms writing data to memory and free memory.
bool initialized(); // if memory is initialized return true else false.
```
When object is created it reads from memory first bit in first byte. If it's set to 1, memory is initialized else it isn't initialized. If memory isn't initialized object constructor gives space for amount of alarms and APData defined in constructor. Else reads it from memory. Method described below clear this bit.
```cpp
void deInitialize();
```
## Memory information and clear
```cpp
uint16_t usedMemory(); //Returns information about amount of used memory in bytes.
void clearMemory(); //It clears memory by write 0 to it.
uint8_t readAlarmCount(); //It returns how much alarms can be written to memory.
uint8_t readWIFICount(); //It returns how much APData can be write to memory.
```

## Time zone

```cpp
void setTimeZone(float tZ); //You can define time zone in range from -63 to 63 with 0.5 step.
float readTimeZone();
```

## Alarms
```cpp
alarm readAlarm(uint8_t alarmNumber); //Number from 0 to length-1 defined in constructor (alarmCount) or read from memory.
void setAlarm(uint8_t alarmNumber, alarm al);
```
Alarm structure constitution.
```cpp
typedef struct{
  uint8_t hour;
  uint8_t minute;
  uint8_t schedule; //MSB = 1 -> alarm turn on LSB = monday
} alarm;
```

## WIFI

```cpp
APData readWIFI(uint8_t WIFINumber); //Number from 0 to length defined in constructor (WIFIcount) or read from memory.
void setWIFI(uint8_t WIFINumber, APData AP);
void setWIFIMode(WIFIMode WM);
WIFIMode readWIFIMode();
```
APData structure constitution.
```cpp
typedef struct{
  char ssid[32];
  char password[63];
  uint8_t priority;
}APData;
```
WIFIMode constitution.
```cpp
enum WIFIMode{
  STAM = 0,
  APM = 1,
  APpSTAM = 2,
  turOffM = 3,
};
```

## Password

```cpp
char* readPassword(); // it has '\0' on the end
void setPassword(char* pass, uint8_t length); //Password can't be wider than 30 chars. Second parameter tells methods how long is pass.
```

## NTP server name

```cpp
void setNTPName(char* name, uint8_t length); //Second parameter tells how long is name.
uint8_t readNTPLength(); //NTP name length + 1. 
char* readNTPName(); // It has '\0' on the end.
```
## Public variable
The class has one public variable.
```cpp
bool firstInitialization; //It informs that memory was initialized first time (memory doesn't have data in right format).
```