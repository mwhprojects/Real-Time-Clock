# About
Code for my Real Time Clock PCBs that displays the current time on four 7-segment digits.

Controled by Atmega328p. Time is kept with DS3231 IC. Seven-segment display controlled with 74HC595 shift registers.

Requires Sodaq_DS3231 and Wire libraries.

# Files
## RTClock_SetTime_RevA.ino
Stripped down "adjust" sample code from Sodaq_DS3231 library to set the date and time. The original example sketch can be used instead.

## RTClock_RevA.ino
Main code.