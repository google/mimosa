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

#include "Adafruit_NeoPixel.h"
#include "mimosa_force.h"
#include <Keyboard.h>

// Adjust this parameter from 1 to 5 to
// adjust the sensitivity from super sensitive (1) to least sensitive (5)
#define SENSITIVITY 1

constexpr int16_t kThresholds[5] = {100, 1000, 5000, 10000, 20000};
constexpr int16_t kThresholdPress = kThresholds[SENSITIVITY - 1];
constexpr int16_t kThresholdRelease = - kThresholds[SENSITIVITY - 1]/4;
constexpr int kSamplingPeriodMilliseconds = 50;
constexpr float kSamplingPeriod = 0.001f * kSamplingPeriodMilliseconds;
constexpr int8_t kPolarity = -1;

Adafruit_NeoPixel pixels(1, PIN_NEOPIXEL);
MimosaForce button_right(kThresholdPress, kThresholdRelease, kSamplingPeriod, kPolarity);
MimosaForce button_left(kThresholdPress, kThresholdRelease, kSamplingPeriod, kPolarity);

// Use this option for OSX:
// char ctrlKey = KEY_LEFT_GUI;
// Use this option for Windows and Linux:
// const char leftKey = KEY_LEFT_CTRL;
// const char rightKey = KEY_LEFT_SHIFT;
const char leftKey = 'a';
const char rightKey = 'b';

void setup() {
  Serial.begin(57600);
  pixels.begin();
  Keyboard.begin();
  button_right.init(0x48);
  button_left.init(0x49);
}

void loop() {
  
  bool trigr = button_right.update();
  bool trigl = button_left.update();

  Serial.println(button_right.rawSample());
  
  if (trigr || trigl) {
    // First let's hangle the LED indicator.
    if (button_right.pressed() && !button_left.pressed()) {
      // Set pixel to amber color on press.
      pixels.setPixelColor(0, pixels.Color(0, 0, 255));
    } else if (!button_right.pressed() && button_left.pressed()){
      pixels.setPixelColor(0, pixels.Color(0, 255, 0));
    } else if (button_right.pressed() && button_left.pressed()){
      pixels.setPixelColor(0, pixels.Color(0, 255, 255));
    } else {
      // Turn off the pixel on release.
      pixels.clear();
    }
    pixels.show();

    // Now let's handle keycode press/release commands
    if (trigr && button_right.pressed()){
      Keyboard.press(rightKey);
    } else if (trigr && !button_right.pressed()){
      Keyboard.release(rightKey);
    }

    if (trigl && button_left.pressed()){
      Keyboard.press(leftKey);
    } else if (trigl && !button_left.pressed()){
      Keyboard.release(leftKey);
    }
  }
  delay(kSamplingPeriodMilliseconds);
}
