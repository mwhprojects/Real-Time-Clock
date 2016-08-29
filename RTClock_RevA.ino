/*
  Title:        Real-Time Clock Project
  Revision:     A
  Description:  A custom Real-Time Clock based on the DS3231 RTC IC. Shift registers are
                used to control four 7-segment digits.
  
  Author:       Matthew W. (www.mwhprojects.com)
  GitHub Repo:  
 */

#include <Wire.h>
#include "Sodaq_DS3231.h"


///////////////////////////////////////////// Global Variables
int a, b, c, d;
int hr, mn, sec;
String hrS, mnS, secS;
char hrDigits[3], mnDigits[3], secDigits[3];
int lastmn = 99;

///////////////////////////////////////////// Shift Register Code
int latchPin = 4;  // Latch pin
int clockPin = 3;  // Clock pin
int dataPin = 2;  // Data pin
int activeLOW = 0;  // Set to 1 if LEDs are active low.
int numofSRs = 4;  // Number of shift registers. (My SR boards have 2 per board.)
int oePin[] = {10};  // If you are using any Output Enable pins, add them to this array.

// Initialize bytes for each shift register.
byte SR[] = {
  B00000000,B00000000,B00000000,B00000000
};

// The shift register function that will output the SR[] bytes to the shift registers.
void SRout() {
  int aa=0;
  int bb=0;

  // Adjust for active LOW LED setup, if applicable.
  if(activeLOW == 1){
    for(int i=0; i<numofSRs; i++){
      SR[i] = ~SR[i];
    }
  }

  /*
    A shift register works by pulsing a clock while changing the logic level on the
    data pin to high or low. Once all of the data has been clocked onto the
    shift registers, the latch pin is then set high to push the data that was
    clocked in onto the parallel output pins.
  */
  digitalWrite(latchPin, 0);  // Set latch pin low.
  
  // Data is clocked onto the shift register "backwards".
  for(bb = numofSRs-1; bb >= 0; bb--){
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);

    for (aa=0; aa<=7; aa++)  {
      digitalWrite(clockPin, 0);  // Set clock pin to low.
      
      /*
        Shift b00000001 to the left and mask it (and it) with the byte on each loop
        of the current for-loop.
        
        If the checked bit is low, the result of the masking will be zero. Otherwise,
        it will be greater than zero, which is a true condition to the if-statement.
      */
      if (SR[bb] & (1<<aa)){
        // If the result of the mask is not/greater than zero, this bit is high.
        digitalWrite(dataPin, 1);
      }
      else{  
        // If the result of the mask is zero, this bit is low.
        digitalWrite(dataPin, 0);
      }

      digitalWrite(clockPin, 1);  // Set clock pin to high to shift the data bit on.
    }

    digitalWrite(clockPin, 0);  // Set the clock pin to low.
  }
  
  // When all bits have been shifted in, set latch pin to high.
  digitalWrite(latchPin, 1);
}

// Function to set brightness using Output Enable pin.
void setBrightness(int SRselect, int SRsetting){
  analogWrite(oePin[SRselect], 255-SRsetting);
}

// Print digits
// '0'
void MaskNumber0(int digit){
  if(digit == 1){
    SR[0] &= B00000001;
    SR[1] = 0;
    SR[2] = 0;
    SR[3] = 0;
    SR[0] ^= B11111100;
    SRout();
  }else if(digit == 2){
    SR[0] &= B11111110;
    SR[1] &= B00000011;
    SR[2] = 0;
    SR[3] = 0;
    SR[0] ^= B00000001;
    SR[1] ^= B11111000;
    SRout();
  }else if(digit == 3){
    SR[0] = 0;
    SR[1] &= B11111100;
    SR[2] &= B00000111;
    SR[3] = 0;
    SR[1] ^= B00000011;
    SR[2] ^= B11110000;
    SRout();
  }else if(digit == 4){
    SR[0] = 0;
    SR[1] = 0;
    SR[2] &= B11111000;
    SR[3] &= B00000000;
    SR[2] ^= B00000111;
    SR[3] ^= B11100000;
    SRout();
  }
}
// '1'
void MaskNumber1(int digit){
  if(digit == 1){
    SR[0] &= B00000001;
    SR[1] = 0;
    SR[2] = 0;
    SR[3] = 0;
    SR[0] ^= B01100000;
    SRout();
  }else if(digit == 2){
    SR[0] &= B11111110;
    SR[1] &= B00000011;
    SR[2] = 0;
    SR[3] = 0;
    SR[0] ^= B00000000;
    SR[1] ^= B11000000;
    SRout();
  }else if(digit == 3){
    SR[0] = 0;
    SR[1] &= B11111100;
    SR[2] &= B00000111;
    SR[3] = 0;
    SR[1] ^= B00000001;
    SR[2] ^= B10000000;
    SRout();
  }else if(digit == 4){
    SR[0] = 0;
    SR[1] = 0;
    SR[2] &= B11111000;
    SR[3] &= B00000000;
    SR[2] ^= B00000011;
    SR[3] ^= B00000000;
    SRout();
  }
}
// '2'
void MaskNumber2(int digit){
  if(digit == 1){
    SR[0] &= B00000001;
    SR[1] = 0;
    SR[2] = 0;
    SR[3] = 0;
    SR[0] ^= B11011010;
    SRout();
  }else if(digit == 2){
    SR[0] &= B11111110;
    SR[1] &= B00000011;
    SR[2] = 0;
    SR[3] = 0;
    SR[0] ^= B00000001;
    SR[1] ^= B10110100;
    SRout();
  }else if(digit == 3){
    SR[0] = 0;
    SR[1] &= B11111100;
    SR[2] &= B00000111;
    SR[3] = 0;
    SR[1] ^= B00000011;
    SR[2] ^= B01101000;
    SRout();
  }else if(digit == 4){
    SR[0] = 0;
    SR[1] = 0;
    SR[2] &= B11111000;
    SR[3] &= B00000000;
    SR[2] ^= B00000110;
    SR[3] ^= B11010000;
    SRout();
  }
}
// '3'
void MaskNumber3(int digit){
  if(digit == 1){
    SR[0] &= B00000001;
    SR[1] = 0;
    SR[2] = 0;
    SR[3] = 0;
    SR[0] ^= B11110010;
    SRout();
  }else if(digit == 2){
    SR[0] &= B11111110;
    SR[1] &= B00000011;
    SR[2] = 0;
    SR[3] = 0;
    SR[0] ^= B00000001;
    SR[1] ^= B11100100;
    SRout();
  }else if(digit == 3){
    SR[0] = 0;
    SR[1] &= B11111100;
    SR[2] &= B00000111;
    SR[3] = 0;
    SR[1] ^= B00000011;
    SR[2] ^= B11001000;
    SRout();
  }else if(digit == 4){
    SR[0] = 0;
    SR[1] = 0;
    SR[2] &= B11111000;
    SR[3] &= B00000000;
    SR[2] ^= B00000111;
    SR[3] ^= B10010000;
    SRout();
  }
}
// '4'
void MaskNumber4(int digit){
  if(digit == 1){
    SR[0] &= B00000001;
    SR[1] = 0;
    SR[2] = 0;
    SR[3] = 0;
    SR[0] ^= B01100110;
    SRout();
  }else if(digit == 2){
    SR[0] &= B11111110;
    SR[1] &= B00000011;
    SR[2] = 0;
    SR[3] = 0;
    SR[0] ^= B00000000;
    SR[1] ^= B11001100;
    SRout();
  }else if(digit == 3){
    SR[0] = 0;
    SR[1] &= B11111100;
    SR[2] &= B00000111;
    SR[3] = 0;
    SR[1] ^= B00000001;
    SR[2] ^= B10011000;
    SRout();
  }else if(digit == 4){
    SR[0] = 0;
    SR[1] = 0;
    SR[2] &= B11111000;
    SR[3] &= B00000000;
    SR[2] ^= B00000011;
    SR[3] ^= B00110000;
    SRout();
  }
}
// '5'
void MaskNumber5(int digit){
  if(digit == 1){
    SR[0] &= B00000001;
    SR[1] = 0;
    SR[2] = 0;
    SR[3] = 0;
    SR[0] ^= B10110110;
    SRout();
  }else if(digit == 2){
    SR[0] &= B11111110;
    SR[1] &= B00000011;
    SR[2] = 0;
    SR[3] = 0;
    SR[0] ^= B00000001;
    SR[1] ^= B01101100;
    SRout();
  }else if(digit == 3){
    SR[0] = 0;
    SR[1] &= B11111100;
    SR[2] &= B00000111;
    SR[3] = 0;
    SR[1] ^= B00000010;
    SR[2] ^= B11011000;
    SRout();
  }else if(digit == 4){
    SR[0] = 0;
    SR[1] = 0;
    SR[2] &= B11111000;
    SR[3] &= B00000000;
    SR[2] ^= B00000101;
    SR[3] ^= B10110000;
    SRout();
  }
}
// '6'
void MaskNumber6(int digit){
  if(digit == 1){
    SR[0] &= B00000001;
    SR[1] = 0;
    SR[2] = 0;
    SR[3] = 0;
    SR[0] ^= B10111110;
    SRout();
  }else if(digit == 2){
    SR[0] &= B11111110;
    SR[1] &= B00000011;
    SR[2] = 0;
    SR[3] = 0;
    SR[0] ^= B00000001;
    SR[1] ^= B01111100;
    SRout();
  }else if(digit == 3){
    SR[0] = 0;
    SR[1] &= B11111100;
    SR[2] &= B00000111;
    SR[3] = 0;
    SR[1] ^= B00000010;
    SR[2] ^= B11111000;
    SRout();
  }else if(digit == 4){
    SR[0] = 0;
    SR[1] = 0;
    SR[2] &= B11111000;
    SR[3] &= B00000000;
    SR[2] ^= B00000101;
    SR[3] ^= B11110000;
    SRout();
  }
}
// '7'
void MaskNumber7(int digit){
  if(digit == 1){
    SR[0] &= B00000001;
    SR[1] = 0;
    SR[2] = 0;
    SR[3] = 0;
    SR[0] ^= B11100000;
    SRout();
  }else if(digit == 2){
    SR[0] &= B11111110;
    SR[1] &= B00000011;
    SR[2] = 0;
    SR[3] = 0;
    SR[0] ^= B00000001;
    SR[1] ^= B11000000;
    SRout();
  }else if(digit == 3){
    SR[0] = 0;
    SR[1] &= B11111100;
    SR[2] &= B00000111;
    SR[3] = 0;
    SR[1] ^= B00000011;
    SR[2] ^= B10000000;
    SRout();
  }else if(digit == 4){
    SR[0] = 0;
    SR[1] = 0;
    SR[2] &= B11111000;
    SR[3] &= B00000000;
    SR[2] ^= B00000111;
    SR[3] ^= B00000000;
    SRout();
  }
}
// '8'
void MaskNumber8(int digit){
  if(digit == 1){
    SR[0] &= B00000001;
    SR[1] = 0;
    SR[2] = 0;
    SR[3] = 0;
    SR[0] ^= B11111110;
    SRout();
  }else if(digit == 2){
    SR[0] &= B11111110;
    SR[1] &= B00000011;
    SR[2] = 0;
    SR[3] = 0;
    SR[0] ^= B00000001;
    SR[1] ^= B11111100;
    SRout();
  }else if(digit == 3){
    SR[0] = 0;
    SR[1] &= B11111100;
    SR[2] &= B00000111;
    SR[3] = 0;
    SR[1] ^= B00000011;
    SR[2] ^= B11111000;
    SRout();
  }else if(digit == 4){
    SR[0] = 0;
    SR[1] = 0;
    SR[2] &= B11111000;
    SR[3] &= B00000000;
    SR[2] ^= B00000111;
    SR[3] ^= B11110000;
    SRout();
  }
}
// '9'
void MaskNumber9(int digit){
  if(digit == 1){
    SR[0] &= B00000001;
    SR[1] = 0;
    SR[2] = 0;
    SR[3] = 0;
    SR[0] ^= B11110110;
    SRout();
  }else if(digit == 2){
    SR[0] &= B11111110;
    SR[1] &= B00000011;
    SR[2] = 0;
    SR[3] = 0;
    SR[0] ^= B00000001;
    SR[1] ^= B11101100;
    SRout();
  }else if(digit == 3){
    SR[0] = 0;
    SR[1] &= B11111100;
    SR[2] &= B00000111;
    SR[3] = 0;
    SR[1] ^= B00000011;
    SR[2] ^= B11011000;
    SRout();
  }else if(digit == 4){
    SR[0] = 0;
    SR[1] = 0;
    SR[2] &= B11111000;
    SR[3] &= B00000000;
    SR[2] ^= B00000111;
    SR[3] ^= B10110000;
    SRout();
  }
}

//////////////////////////////////////// Function to update the display.
void updateDisplay(char hrDigits[], char mnDigits[]){
      // Display first hour digit.
      if(hrDigits[0] == '0' || hrDigits[1] == NULL){
        MaskNumber0(1);
      }
      if(hrDigits[0] == '1'){
        MaskNumber1(1);
      }
      if(hrDigits[0] == '2'){
        MaskNumber2(1);
      }
      if(hrDigits[0] == '3'){
        MaskNumber3(1);
      }
      if(hrDigits[0] == '4'){
        MaskNumber4(1);
      }
      if(hrDigits[0] == '5'){
        MaskNumber5(1);
      }
      if(hrDigits[0] == '6'){
        MaskNumber6(1);
      }
      if(hrDigits[0] == '7'){
        MaskNumber7(1);
      }
      if(hrDigits[0] == '8'){
        MaskNumber8(1);
      }
      if(hrDigits[0] == '9'){
        MaskNumber9(1);
      }
      // Display second hour digit.
      if(hrDigits[1] == '0' || hrDigits[0] == NULL){
        MaskNumber0(2);
      }
      if(hrDigits[1] == '1'){
        MaskNumber1(2);
      }
      if(hrDigits[1] == '2'){
        MaskNumber2(2);
      }
      if(hrDigits[1] == '3'){
        MaskNumber3(2);
      }
      if(hrDigits[1] == '4'){
        MaskNumber4(2);
      }
      if(hrDigits[1] == '5'){
        MaskNumber5(2);
      }
      if(hrDigits[1] == '6'){
        MaskNumber6(2);
      }
      if(hrDigits[1] == '7'){
        MaskNumber7(2);
      }
      if(hrDigits[1] == '8'){
        MaskNumber8(2);
      }
      if(hrDigits[1] == '9'){
        MaskNumber9(2);
      }
    
      // Display first minute digit.
      if(mnDigits[0] == '0' ||mnDigits[0] == NULL){
        MaskNumber0(3);
      }
      if(mnDigits[0] == '1'){
        MaskNumber1(3);
      }
      if(mnDigits[0] == '2'){
        MaskNumber2(3);
      }
      if(mnDigits[0] == '3'){
        MaskNumber3(3);
      }
      if(mnDigits[0] == '4'){
        MaskNumber4(3);
      }
      if(mnDigits[0] == '5'){
        MaskNumber5(3);
      }
      if(mnDigits[0] == '6'){
        MaskNumber6(3);
      }
      if(mnDigits[0] == '7'){
        MaskNumber7(3);
      }
      if(mnDigits[0] == '8'){
        MaskNumber8(3);
      }
      if(mnDigits[0] == '9'){
        MaskNumber9(3);
      }
      // Display second minute digit.
      if(mnDigits[1] == '0' || mnDigits[1] == NULL){
        MaskNumber0(4);
      }
      if(mnDigits[1] == '1'){
        MaskNumber1(4);
      }
      if(mnDigits[1] == '2'){
        MaskNumber2(4);
      }
      if(mnDigits[1] == '3'){
        MaskNumber3(4);
      }
      if(mnDigits[1] == '4'){
        MaskNumber4(4);
      }
      if(mnDigits[1] == '5'){
        MaskNumber5(4);
      }
      if(mnDigits[1] == '6'){
        MaskNumber6(4);
      }
      if(mnDigits[1] == '7'){
        MaskNumber7(4);
      }
      if(mnDigits[1] == '8'){
        MaskNumber8(4);
      }
      if(mnDigits[1] == '9'){
        MaskNumber9(4);
      }
    }

///////////////////////////////////////////// SETUP
void setup() {  
  Wire.begin();
  rtc.begin();
    
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  for(int cc = 0; cc < sizeof(oePin); cc++){
    pinMode(oePin[cc], OUTPUT);
  }
  SR[0] = 0;
  SR[1] = 0;
  SR[2] = 0;
  SR[3] = 0;
  SRout();
}


///////////////////////////////////////////// LOOP
void loop() { 
  // Get current hour and minute from RTC.
  DateTime now = rtc.now();
  hr = now.hour();
  mn = now.minute();
  sec = now.second();
  
  // Convert int to string.
  hrS = String(hr);
  mnS = String(mn);
  secS = String(sec);

  // Add leading 0 to hours less than "10".
  if(hrS == "0") hrS = "12";
  if(hrS == "1") hrS = "01";
  if(hrS == "2") hrS = "02";
  if(hrS == "3") hrS = "03";
  if(hrS == "4") hrS = "04";
  if(hrS == "5") hrS = "05";
  if(hrS == "6") hrS = "06";
  if(hrS == "7") hrS = "07";
  if(hrS == "8") hrS = "08";
  if(hrS == "9") hrS = "09";
  // 24hr to 12hr.
  if(hrS == "13") hrS = "01";
  if(hrS == "14") hrS = "02";
  if(hrS == "15") hrS = "03";
  if(hrS == "16") hrS = "04";
  if(hrS == "17") hrS = "05";
  if(hrS == "18") hrS = "06";
  if(hrS == "19") hrS = "07";
  if(hrS == "20") hrS = "08";
  if(hrS == "21") hrS = "09";
  if(hrS == "22") hrS = "10";
  if(hrS == "23") hrS = "11";

  // Add leading 0 to minutes less than "10".
  if(mnS == "1") mnS = "01";
  if(mnS == "2") mnS = "02";
  if(mnS == "3") mnS = "03";
  if(mnS == "4") mnS = "04";
  if(mnS == "5") mnS = "05";
  if(mnS == "6") mnS = "06";
  if(mnS == "7") mnS = "07";
  if(mnS == "8") mnS = "08";
  if(mnS == "9") mnS = "09";

  // Break string into char array.
  hrS.toCharArray(hrDigits, 3);
  mnS.toCharArray(mnDigits, 3);
  secS.toCharArray(secDigits, 3);

  // Update the display only if the time (minute) has changed.
  if(lastmn == 99){ 
    // Code is running for the first time, so save the last minute to the current one.
    lastmn = mn;
    // Update display.
    updateDisplay(hrDigits, mnDigits);
  }else{
    if(lastmn != mn){ // If the saved minute is the same as the current time's minute, don't do anything.
      // Do nothing.
    }else{  // If the saved minute is different from the current time's minute, update the display.
      // Save current minute value for the next check.
      lastmn = mn;

      // Update display.
    updateDisplay(hrDigits, mnDigits);
    }
  }
  
}


