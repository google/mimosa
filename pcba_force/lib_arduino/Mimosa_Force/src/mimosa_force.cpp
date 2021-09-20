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

#include <math.h>

namespace {
enum {
  // Fractional bits for the filter state.
  kStateBits = 9,
  // Fractional bits for the filter coefficient.
  kCoeffBits = 6,
  // Number of initial samples used to "warm up" the smoothing filter. This
  // should preferably be a power of two, since we divide by this value.
  // NOTE: `Update()` doesn't make any detections until warm up is done, so this
  // value shouldn't be too large.
  kNumWarmUpSamples = 4,
};

int32_t computeSmootherCoeff(float sampling_period_s, float time_constant_s) {
  // Determine the Z-plane pole location for the smoothing filter.
  const float pole = exp(-sampling_period_s / time_constant_s);
  // Convert the pole to a fixed point smoother coefficient with kCoeffBits
  // fractional bits.
  const float smoother_coeff_float = 1.0f - pole;
  return static_cast<int32_t>(smoother_coeff_float * (1 << kCoeffBits) + 0.5f);
}

// Converts an integer value to fixed point with kStateBits fractional bits.
int32_t toFixedPoint(int16_t value_int) {
  return static_cast<int32_t>(value_int) << kStateBits;
}

// Converts a fixed point value with kStateBits fractional bits to integer.
// The value is truncated toward zero.
int16_t fromFixedPoint(int32_t value_fixed) {
  return static_cast<int16_t>(value_fixed / (1 << kStateBits));
}
}  // namespace

// The constructor sets parameters to hopefully usable default values.
MimosaForce::MimosaForce()
    : sampling_period_s_(0.05f),
      time_constant_s_(0.4f),
      press_threshold_fixed_(-240 << kStateBits),
      release_threshold_fixed_(60 << kStateBits),
      polarity_(-1),
      smoother_coeff_fixed_(computeSmootherCoeff(
            sampling_period_s_, time_constant_s_)) {
  reset();
}

// The constructor sets parameters to hopefully usable default values.
MimosaForce::MimosaForce(int16_t threshold_press, int16_t threshold_release, float seconds, int8_t polarity)
    : sampling_period_s_(seconds),
      time_constant_s_(0.4f),
      press_threshold_fixed_(threshold_press << kStateBits),
      release_threshold_fixed_(threshold_release << kStateBits),
      polarity_(polarity),
      smoother_coeff_fixed_(computeSmootherCoeff(
            sampling_period_s_, time_constant_s_)) {
  reset();
}

void MimosaForce::init(uint8_t address) {
  ads_.setGain(GAIN_ONE);
  ads_.setDataRate(RATE_ADS1115_250SPS);
  ads_.begin(address);
}

void MimosaForce::reset() {
  smoothed_fixed_ = 0;
  residual_fixed_ = 0;
  raw_ = 0;
  warm_up_counter_ = kNumWarmUpSamples;
  pressed_ = false;
}

void MimosaForce::setSamplingPeriodS(float seconds) {
  sampling_period_s_ = seconds;
  // Recompute the smoother coefficient.
  smoother_coeff_fixed_ = computeSmootherCoeff(
      sampling_period_s_, time_constant_s_);
}

void MimosaForce::setTimeConstantS(float seconds) {
  time_constant_s_ = seconds;
  // Recompute the smoother coefficient.
  smoother_coeff_fixed_ = computeSmootherCoeff(
      sampling_period_s_, time_constant_s_);
}

int16_t MimosaForce::pressThreshold() const {
  return fromFixedPoint(press_threshold_fixed_);
}

void MimosaForce::setPressThreshold(int16_t threshold) {
  press_threshold_fixed_ = toFixedPoint(threshold);
}

int16_t MimosaForce::releaseThreshold() const {
  return fromFixedPoint(release_threshold_fixed_);
}

void MimosaForce::setReleaseThreshold(int16_t threshold) {
  release_threshold_fixed_ = toFixedPoint(threshold);
}

int16_t MimosaForce::processedSample() const {
  return fromFixedPoint(residual_fixed_);
}

bool MimosaForce::update() {
  // Read the next raw sample from the ADS1115 sensor. 
  raw_ = ads_.readADC_SingleEnded(0);

  // Convert raw_ to fixed point with kStateBits fractional bits.
  const int32_t raw_fixed = toFixedPoint(raw_);

  if (warm_up_counter_) {  // The filter is warming up.
    smoothed_fixed_ += raw_fixed;

    if (--warm_up_counter_ == 0) {
      // Finished warming up. To initialize the filter, we set its state to the
      // average of the first kNumWarmUpSamples raw samples.
      smoothed_fixed_ /= kNumWarmUpSamples;
    } else {
      return false;  // Don't make detections yet while warming up.
    }
  } else {
    // We estimate the bias component in the sensor signal using a one-pole
    // lowpass filter, also known as an exponential moving average. This line
    // runs one sample through the filter. In float arithmetic, it would be:
    //
    //   smoothed += smoother_coeff * (raw - smoothed).
    //
    // `smoother_coeff_fixed_` has kCoeffBits fractional bits, so we shift the
    // product down by that amount.
    smoothed_fixed_ += (smoother_coeff_fixed_ * (raw_fixed - smoothed_fixed_))
        / (1 << kCoeffBits);
  }

  // Subtract off the smoothed signal, which represents the slowly varying bias
  // in the raw signal, and look at the residual. In other words, the raw signal
  // is highpass filtered.
  residual_fixed_ = polarity_ * (raw_fixed - smoothed_fixed_);

  // Compare with thresholds to decide whether to trigger an event.
  if (!pressed_) {
    if (residual_fixed_ > press_threshold_fixed_) {  // Press event.
      pressed_ = true;
      return true;
    }
  } else {
    if (residual_fixed_ < release_threshold_fixed_) {  // Release event.
      pressed_ = false;
      return true;
    }
  }
  return false;
}