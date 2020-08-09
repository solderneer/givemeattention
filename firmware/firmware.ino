/*
  Example sketch which demonstrates how to use the Serial Monitor to
  dynamically change animation parameters.
  
  Keith Lord - 2018

  LICENSE

  The MIT License (MIT)

  Copyright (c) 2018  Keith Lord 

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sub-license, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
  
  CHANGELOG
  2018-11-10 initial version (refactor of kitesurfer1404's original serial control sketch)
*/

#define REDUCED_MODES // sketch too big for Arduino Leonardo flash, so invoke reduced modes
#include <WS2812FX.h>

#define LED_COUNT 30
#define LED_PIN 6
#define MAX_NUM_CHARS 1 // maximum number of characters read from the Serial Monitor

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

char cmd[MAX_NUM_CHARS];       // char[] to store incoming serial commands
boolean cmd_complete = false;  // whether the command string is complete

void setup() {
  Serial.begin(115200);
  
  ws2812fx.init();
  ws2812fx.setBrightness(0);
  ws2812fx.setSpeed(200);
  ws2812fx.setMode(FX_MODE_STATIC);
  ws2812fx.start();

}

void loop() {
  ws2812fx.service();

  recvChar(); // read serial comm

  if(cmd_complete) {
    if(cmd[0] == '0') {
      ws2812fx.setBrightness(0);
      ws2812fx.setMode(FX_MODE_STATIC);
    } else if(cmd[0] == '1') {
      ws2812fx.setBrightness(100);
      ws2812fx.setMode(FX_MODE_BLINK);
    }

    cmd[0] = '\0';         // reset the commandstring
    cmd_complete = false;  // reset command complete
  }
}

/*
 * Reads new input from serial to cmd string. Command is completed on \n
 */
void recvChar(void) {
  static byte index = 0;
  while (Serial.available() > 0 && cmd_complete == false) {
    char rc = Serial.read();
    if (rc != '\n') {
      if(index < MAX_NUM_CHARS) cmd[index++] = rc;
    } else {
      cmd[index] = '\0'; // terminate the string
      index = 0;
      cmd_complete = true;
      Serial.print("received '"); Serial.print(cmd); Serial.println("'");
    }
  }
}
