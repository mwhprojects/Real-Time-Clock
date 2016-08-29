// Date and time functions using a RX8025 RTC connected via I2C and Wire lib

#include <Wire.h>
#include "Sodaq_DS3231.h"

char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

//year, month, date, hour, min, sec and week-day(starts from 0 and goes to 6)
//writing any non-existent time-data may interfere with normal operation of the RTC.
//Take care of week-day also.
DateTime dt(2016, 03, 26, 18, 13, 0, 5);


void setup () 
{
    Wire.begin();
    rtc.begin();
    rtc.setDateTime(dt); //Adjust date-time as defined 'dt' above 
}

void loop () 
{
}
