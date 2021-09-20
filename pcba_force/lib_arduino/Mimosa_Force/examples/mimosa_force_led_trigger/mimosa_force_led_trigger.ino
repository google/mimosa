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

#include "mimosa_force.h"

constexpr int kSamplingPeriodMilliseconds = 50;
constexpr int kLedPin = 5;

MimosaForce button;

void setup() {
  pinMode(kLedPin, OUTPUT);
  digitalWrite(kLedPin, LOW);
  button.setSamplingPeriodS(0.001f * kSamplingPeriodMilliseconds);
  button.init(0x48);
}

void loop() {
  if (button.update()) {
    if (button.pressed()) {
      digitalWrite(kLedPin, HIGH);
    } else {
      digitalWrite(kLedPin, LOW);
    }
  }

  delay(kSamplingPeriodMilliseconds);
}
