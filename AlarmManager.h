#ifndef _TIME_MANAGER_H
#define _TIME_MANAGER_H

#include <NTPtimeESP.h>
#include "AlarmTime.h"

extern NTPtime NTPch;

class AlarmManager{
    private:
    AlarmTime tToAlarm;
    public:
    /*
    * The structure contains following fields:
    * struct strDateTime
        {
        byte hour;
        byte minute;
        byte second;
        int year;
        byte month;
        byte day;
        byte dayofWeek;
        boolean valid;
        };
    */
    strDateTime tmpTime;
    strDateTime dateTime;
    bool alarmRung = false;

    AlarmTime timeToAlarm(){
        tToAlarm.hour = 0;
        tToAlarm.minute = 0;
        if(dateTime.hour > alarmState.hour && !alarmRung){ // if the alarm is scheduled for tomorrow
            tToAlarm.hour += 23 - dateTime.hour;
        }
        tToAlarm.hour += alarmState.hour - dateTime.hour;
        tToAlarm.minute = alarmState.minute - dateTime.minute;

        if(tToAlarm.minute < 0 && !alarmRung){
            tToAlarm.hour--;
            tToAlarm.minute += 60;
        }
        return tToAlarm;
    }

public:

    String strTimeToAlarm(){
        AlarmTime t = timeToAlarm();
        return (String(t.hour)+":"+ String(t.minute));
    }

 bool checkTime(){
    this->tmpTime = NTPch.getNTPtime(2.0, 1);
    if(this->tmpTime.valid){
        this->dateTime = tmpTime;
        NTPch.printDateTime(this->dateTime);
        return true;
        // byte actualHour = dateTime.hour;
        // byte actualMinute = dateTime.minute;
        // byte actualsecond = dateTime.second;
        // int actualyear = dateTime.year;
        // byte actualMonth = dateTime.month;
        // byte actualday = dateTime.day;
        // byte actualdayofWeek = dateTime.dayofWeek;
    }else{
        return false;
    }
 }



 bool shouldAlarmRing(){
     AlarmTime t = timeToAlarm();
     return (t.hour <= 0 && t.minute <= 0 && !alarmRung);
 }

 void setAlarmRung(bool rung){
     this->alarmRung = rung;
 }

};

extern AlarmManager alarmManager;

#endif