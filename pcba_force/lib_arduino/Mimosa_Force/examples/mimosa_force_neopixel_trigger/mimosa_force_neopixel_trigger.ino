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

constexpr int kSamplingPeriodMilliseconds = 50;

Adafruit_NeoPixel pixels(1, PIN_NEOPIXEL);
MimosaForce button;

void setup() {
  Serial.begin(57600);
  pixels.begin();
  button.setSamplingPeriodS(0.001f * kSamplingPeriodMilliseconds);
  button.init(0x48);
}

void loop() {
  if (button.update()) {
    if (button.pressed()) {
      // Set pixel to amber color on press.
      pixels.setPixelColor(0, pixels.Color(90, 72, 20));
    } else {
      // Turn off the pixel on release.
      pixels.clear();
    }
    pixels.show();
  }

  delay(kSamplingPeriodMilliseconds);
}
