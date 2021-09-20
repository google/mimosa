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

// Main API for the MIMOSA Force.
//
// Prerequisite library: Adafruit ADS1X15.h

#ifndef MIMOSA_FORCE_MIMOSA_FORCE_H
#define MIMOSA_FORCE_MIMOSA_FORCE_H

#include <stdint.h>

#include "Adafruit_ADS1X15.h"
#include "Arduino.h"

class MimosaForce{
 public:
  MimosaForce();
  MimosaForce(int16_t threshold_press, int16_t threshold_release, float seconds, int8_t polarity);
  // Starts the sensor at `address`.
  void init(uint8_t address = 0x48);

  // The following accessors tune detection processing. They may be called at
  // any time (or not called at all, in which case default parameters are used.)

  // Tells MimosaForce how often `Update()` will be called in units of seconds.
  // It is up to the caller to ensure that calls to `Update()` are actually at
  // this period, at least approximately. The sampling period is needed
  // internally for processing. Suggested setting: between 0.01 and 0.1 s.
  void setSamplingPeriodS(float seconds);
  // Sets the filter time constant in units of seconds for estimating the bias
  // component in the sensor signal. A larger value makes the estimation adapt
  // more slowly. Suggested setting: 0.4 s.
  void setTimeConstantS(float seconds);
  // Sets the threshold for press detection. A press event is triggered when the
  // processed sensor value falls below this threshold. Suggested setting: -240.
  void setPressThreshold(int16_t threshold);
  // Sets the threshold for release detection. Suggested setting: 60.
  void setReleaseThreshold(int16_t threshold);

  // Sampling period in seconds.
  float samplingPeriodS() const { return sampling_period_s_; }
  // Filter time constant in seconds.
  float timeConstantS() const { return time_constant_s_; }
  // Press detection threshold.
  int16_t pressThreshold() const;
  // Release detection threshold.
  int16_t releaseThreshold() const;

  // Communicates with the ADS1115 sensor onboard MIMOSA HDR Force block. Gets
  // the latest integrated force value digitized from the sensor and processes
  // the state of the button. Returns true if a button press or release event
  // occurred (call `pressed()` to determine which), and false if no change.
  //
  // NOTE: Call `Init()` before calling this method.
  //
  // Typical use:
  //
  //   if (button.Update()) {
  //     if (button.pressed()) {
  //       // Button was pressed.
  //     } else {
  //       // Button was released.
  //     }
  //   }
  bool update();

  // Clears the button to released state and resets processing.
  void reset();

  // Gets whether the button is currently pressed.
  bool pressed() const { return pressed_; }
  // Returns latest raw sample from the sensor.
  int16_t rawSample() const { return raw_; }
  // Returns latest processed sample.
  int16_t processedSample() const;

 private:
  Adafruit_ADS1115 ads_;

  float sampling_period_s_;
  float time_constant_s_;
  int32_t press_threshold_fixed_;
  int32_t release_threshold_fixed_;
  int32_t smoother_coeff_fixed_;

  int32_t smoothed_fixed_;
  int32_t residual_fixed_;
  int16_t raw_;
  int16_t warm_up_counter_;
  bool pressed_;
  int8_t polarity_;
};

#endif  // MIMOSA_FORCE_MIMOSA_FORCE_H

