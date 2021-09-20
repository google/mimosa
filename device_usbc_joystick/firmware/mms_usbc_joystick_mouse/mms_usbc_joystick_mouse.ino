/* Copyright 2021 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Mouse.h"
#include <Adafruit_ADS1X15.h>

// parameters for reading the joystick:
int range = 12;               // output range of X or Y movement
int responseDelay = 5;        // response delay of the mouse, in ms
int threshold = range / 4;    // resting threshold
int center = range / 2;       // resting position value
constexpr int kSamplingPeriodMilliseconds = 50;

bool mouseIsActive = false;    // whether or not to control the mouse
int lastSwitchState = LOW;        // previous switch state

Adafruit_ADS1115 mouse_joystick; //on mux ch0

void setup() {
  mouse_joystick.setGain(GAIN_ONE);
  mouse_joystick.setDataRate(RATE_ADS1115_250SPS);
  mouse_joystick.begin(0x4A);
  // take control of the mouse:
  Mouse.begin();
}

void loop() {
  // read the switch:
  int switchState = mouse_joystick.readADC_SingleEnded(2);
  if (switchState > 13000){
    switchState = LOW;
  }
  else {
    switchState = HIGH;
  }
  
  // if it's changed and it's high, toggle the mouse state:
  if (switchState != lastSwitchState) {
    if (switchState == HIGH) {
      mouseIsActive = !mouseIsActive;
    }
  }
  // save switch state for next comparison:
  lastSwitchState = switchState;
  
  // read and scale the two axes:
  int xReading = mouse_joystick.readADC_SingleEnded(0);
  int yReading = mouse_joystick.readADC_SingleEnded(1);

  if (xReading < 13000 && xReading > 12000){
     xReading = 0;
  } else {
     xReading = (int)-map(xReading, 4000, 23000, -20, 20);
  }  
  
  if (yReading < 13000 && yReading > 12000){
     yReading = 0;
  } else {
     yReading = (int)-map(yReading, 4000, 23000, -20, 20);
  }
 
  // if the mouse control state is active, move the mouse:
  if (mouseIsActive) {
    Mouse.move(xReading, yReading, 0);
  }
  
  delay(responseDelay);
}
