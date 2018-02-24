/*
  AS5045_scorpion.cpp - Library for 2 encoders as odometry
  Created by Aaron Chong, Feb 2018
  minebot, Team B, MRSD 2018
*/

#include "Arduino.h"
#include "AS5045_scorpion.h"

scorpion_wheels::scorpion_wheels(int clock_pin, int CSn_pin, int left_input, int right_input, float wheel_diam, float base_width)
{
  _clock_pin  = clock_pin;
  _CSn_pin    = CSn_pin;
  _left_input = left_input;
  _right_input= right_input;
  _wheel_diam = wheel_diam;
  _base_width = base_width; //in meters
}

void scorpion_wheels::setup_rotary_encoders()
{
  pinMode(_clock_pin, OUTPUT); // SCK
  pinMode(_CSn_pin, OUTPUT); // CSn -- has to toggle high and low to signal chip to start data transfer
  pinMode(_left_input, INPUT); // SDA
  pinMode(_right_input, INPUT);
  debug = 0;
}

void scorpion_wheels::rotary_data()
{
  digitalWrite(_CSn_pin, HIGH); // CSn high
  digitalWrite(_clock_pin, HIGH); // CLK high
  delayMicroseconds(5); // wait for 1 second for no particular reason
  digitalWrite(_CSn_pin, LOW); // CSn low: start of transfer
  delayMicroseconds(5); // delay for chip -- 1000x as long as it needs to be
  digitalWrite(_clock_pin, LOW); // CLK goes low: start clocking
  delayMicroseconds(5); // hold low for 10 ms
  for (int x=0; x < 18; x++) // clock signal, 18 transitions, output to clock pin
  { 
    digitalWrite(_clock_pin, HIGH); // clock goes high
    delayMicroseconds(5); // wait 5 microsec
    left_input_stream   = digitalRead(_left_input); // read one bit of data from pin
    right_input_stream  = digitalRead(_right_input);
    //Serial.print(inputstream, DEC); // useful if you want to see the actual bits
    left_packed_data    = ((left_packed_data << 1) + left_input_stream); // left-shift summing variable, add pin value
    right_packed_data   = ((right_packed_data << 1) + right_input_stream);
    digitalWrite(_clock_pin, LOW);
    delayMicroseconds(5); // end of one clock cycle
  } // end of entire clock cycle

  left_angle        = left_packed_data & angle_mask; // mask rightmost 6 digits of packeddata to zero, into angle.
  right_angle       = right_packed_data & angle_mask;
  left_angle        = (left_angle >> 6); // shift 18-digit angle right 6 digits to form 12-digit value
  right_angle       = (right_angle >> 6);
  left_angle_float  = left_angle * resolution; // angle * (360/4096) == actual degrees
  right_angle_float = right_angle * resolution;

  //arduino's C++ can't handle remainder of floats, here's a workaround
  left_angle_temp   = round((left_angle_float - _left_resting_angle + 360.0) * 10000);
  right_angle_temp  = round((right_angle_float - _right_resting_angle + 360.0) * 10000);
  left_angle_temp   = left_angle_temp % (360*10000);
  right_angle_temp  = right_angle_temp % (360*10000);
  left_true_angle   = left_angle_temp / 10000.0;
  right_true_angle  = right_angle_temp / 10000.0;

  //Convert angle to rad.
  left_true_rad     = pi*left_true_angle / 180;
  right_true_rad    = pi*right_true_angle / 180;
}

void scorpion_wheels::calibrate_rotary_encoders()
{
  _left_resting_angle = 0;
  _right_resting_angle = 0;
  // _resting_angle = rotary_data();
  rotary_data();
  _left_resting_angle = left_true_angle;
  _right_resting_angle = right_true_angle;
  Serial.print("Left angle calibrated, new 0 at ");
  Serial.println(_left_resting_angle);
  Serial.print("Right angle calibrated, new 0 at ");
  Serial.println(_right_resting_angle);

  past_micros = micros();
}

void scorpion_wheels::dist_data()
{
  rotary_data();
  //Count rotations 
  if (left_true_rad - left_prev_rad > 4) {
    left_cnt      = left_cnt-1;
  }
  if (left_true_rad - left_prev_rad < -4) {
    left_cnt      = left_cnt+1;
  }
  if (right_true_rad - right_prev_rad > 4) {
    right_cnt     = right_cnt-1;
  }
  if (right_true_rad - right_prev_rad < -4) {
    right_cnt     = right_cnt+1;
  }

  //handle reverse value here
  left_dist       = - _wheel_diam*100*(left_cnt*pi + left_true_rad/(2));
  right_dist      = _wheel_diam*100*(right_cnt*pi + right_true_rad/(2));
  // true_dist = - _wheel_diam*(cnt*pi + true_rad/(2));  //negative in front for the wheel rolling direction relative to scorpion
   
  left_prev_rad   = left_true_rad;
  right_prev_rad  = right_true_rad;
}

void scorpion_wheels::get_vx_vth()
{
  //updating left_dist and right_dist
  dist_data();

  dt_micros       = micros() - past_micros;

  float left_v    = (left_dist - left_past_dist) * 1000 / dt_micros;   //m/s
  float right_v   = (right_dist - right_past_dist) * 1000 / dt_micros;

  vx              = (left_v + right_v) / 2;
  vth             = (right_v - left_v) / _base_width;

  left_past_dist  = left_dist;
  right_past_dist = right_dist;
  past_micros     = micros();
}
