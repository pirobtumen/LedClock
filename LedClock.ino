/*
  The MIT License (MIT)
  Copyright (c) 2016 Alberto Sola
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
 * Led Clock
 * =============================================================================
 * 
 * Parts:
 * 
 * - 3x 8x8 LED Matrix
 * - 1x Arduino
 * - 1x TinyRTC
 * 
 * To setup the date, conect and use the TinyRTC see: https://github.com/pirobtumen/TinyRTC
 * To see the full project doc see: https://github.com/pirobtumen/LedClock
 * 
 * It uses 'LedControl' library: https://github.com/wayoda/LedControl
 * 
 * Alberto Sola - 2016
 */

// -----------------------------------------------------------------------------

#include "LedControl.h"
#include <Wire.h>

// -----------------------------------------------------------------------------

// TinyRTC (DS1307) I2C Address
const uint8_t RTC_ADDRESS = 0x68;

// Date variables
uint8_t sec, mins, hour, day, month, year, weekday;

LedControl lc = LedControl(12,11,10,3);

// Seconds
byte separator = B00000110;
byte separator2 = B01100000;

// Number characters
byte number[10][5] = {
  // 0
  { B11111111, B10000001, B10000001, B11111111 },
  // 1
  { B00000000, B00000100, B00000010, B11111111 },
  // 2
  { B11100011, B10010001, B10001001, B10000111},
  // 3
  { B10000001, B10001001, B10001001, B11111111},
  // 4
  { B00001111, B00001000, B00001000, B11111110},
  // 5
  { B10001111, B10001001, B10001001, B11111001 },
  // 6
  { B11111111, B10001001, B10001001, B11111001 },
  // 7
  { B11100001, B00010001, B00001001, B00000111 },
  // 8
  { B11111111, B10001001, B10001001, B11111111 },
  // 9
  { B00001111, B00001001, B00001001, B11111111 }  
};


// -----------------------------------------------------------------------------

// TINY RTC

// -----------------------------------------------------------------------------

uint8_t bcd_to_int( uint8_t data ){
  /*
   * Convert BCD 
   */
  return (data / 16 * 10) + (data & 0x0F);
}

// -----------------------------------------------------------------------------

bool getDate( uint8_t & secs, uint8_t & mins, uint8_t & hours,
              uint8_t & day, uint8_t & month, uint8_t & year,
              uint8_t & weekday){
  /*
   * Get the date from the DS1307.
   */

  uint8_t control;
  
  Wire.beginTransmission(0x68);
  Wire.write(0);
  
  // Check if the DS1307 answered
  if(Wire.endTransmission() != 0)
    return false;

  // Read data (8B) request
  Wire.requestFrom(0x68, 8);
  
  secs = bcd_to_int(Wire.read());
  mins = bcd_to_int(Wire.read());
  hour = bcd_to_int(Wire.read());
  weekday = bcd_to_int(Wire.read());
  day = bcd_to_int(Wire.read());
  month = bcd_to_int(Wire.read());
  year = bcd_to_int(Wire.read());
  
  control = Wire.read();

  return true;
}

// -----------------------------------------------------------------------------

// LED CLOCK

// -----------------------------------------------------------------------------

void setup() {

  // Initialize Wire (I2C) lib
  Wire.begin();

  // Get number of matrices
  int devices = lc.getDeviceCount();

  // Initialize all of them
  for(int address=0; address < devices; address++) {
    
    // The MAX72XX is in power-saving mode on startup
    lc.shutdown(address,false);
    
    /* Set the brightness [0,15].
     *  
     * In my opinion 1 is bright enough, just try.
     */
    lc.setIntensity(address,1);
    
    lc.clearDisplay(address);
  }
}

// -----------------------------------------------------------------------------

void loop() {

  const int COL_NUMBER = lc.getDeviceCount() * 8;  
  static bool animation = true;

  int digits[4];
  int pos = 0;

  // Read the date from the RTC
  getDate(sec,mins,hour,day,month,year,weekday);
  
  digits[0] = hour / 10;
  digits[1] = hour % 10;
  digits[2] = mins / 10;
  digits[3] = mins % 10;

  for( int i = 0; i < COL_NUMBER; i+=5 ){

    lc.setColumn( i/8, 7 - i%8, number[digits[pos]][0] );
    lc.setColumn( (i+1)/8, 7 - (i+1)%8, number[digits[pos]][1] );
    lc.setColumn( (i+2)/8, 7 - (i+2)%8, number[digits[pos]][2] );
    lc.setColumn( (i+3)/8, 7 - (i+3)%8, number[digits[pos]][3] );

    
    if(pos == 1){
      
      i+=5;   

      if( animation ){
        lc.setColumn( (i+1)/8, 7 - (i+1)%8, separator );
        lc.setColumn( (i+2)/8, 7 - (i+2)%8, separator );
      }
      else{
        lc.setColumn( (i+1)/8, 7 - (i+1)%8, separator2 );
        lc.setColumn( (i+2)/8, 7 - (i+2)%8, separator2 );
      }
      
    }

    pos++;
  }
  
  delay( 430 ); // TODO: calc exact value.
  animation = !animation;
}
