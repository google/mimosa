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
#include "mimosa_force.h"

constexpr int kSamplingPeriodMilliseconds = 80;

MimosaForce button1;
MimosaForce button2;

void setup() {
  button1.setSamplingPeriodS(0.001f * kSamplingPeriodMilliseconds);
  button2.setSamplingPeriodS(0.001f * kSamplingPeriodMilliseconds);
  button1.init(0x48);
  button2.init(0x49);
  Serial.begin(57600);
  Serial.println("MIMOSA demo: using two force sensors as mouse buttons.");
}

void loop() {
  if (button1.update()) {
    if (button1.pressed()) {
      Mouse.press(MOUSE_LEFT);
      Serial.println("Left mouse button pressed.");
    } else {
      Mouse.release(MOUSE_LEFT);
      Serial.println("Left mouse button released.");
    }
  }

  if (button2.update()) {
    if (button2.pressed()) {
      Mouse.press(MOUSE_RIGHT);
      Serial.println("Right mouse button pressed.");
    } else {
      Mouse.release(MOUSE_RIGHT);
      Serial.println("Right mouse button released.");
    }
  }

  delay(kSamplingPeriodMilliseconds);
}

