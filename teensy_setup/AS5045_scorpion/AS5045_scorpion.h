/*
  AS5045_scorpion.h - Library for 2 encoders as odometry
  Created by Aaron Chong, Feb 2018
  minebot, Team B, MRSD 2018

*/
#ifndef AS5045_SCORPION_h
#define AS5045_SCORPION_h

#include "Arduino.h"

class scorpion_wheels
{
  public:
    //if reverse is true, it probably means its the right wheel
    scorpion_wheels(int CSn_pin, int clock_pin, int left_input, int right_input, float wheel_diam, float base_width);
    void setup_rotary_encoders();
    void calibrate_rotary_encoders();
    void rotary_data();
    void dist_data();
    void get_vx_vth();

    //non shared
    int left_input_stream = 0; // one bit read from pin
    int right_input_stream = 0;
    long left_packed_data = 0; // two bytes concatenated from inputstream
    long right_packed_data = 0;
    long left_angle = 0; // holds processed angle value
    long right_angle = 0;
    float left_angle_float = 0.0;
    float right_angle_float = 0.0;
    float left_true_angle = 0.0;
    float right_true_angle = 0.0;
    float left_calib_angle = 0.0;
    float right_calib_anglt = 0.0;
    float left_offset_angle = 0.0; //18 22
    float right_offset_angle = 0.0;
    float left_rotary_angle = 0.0;
    float right_rotary_angle = 0.0;
    float left_starting_angle = 0.0;
    float right_starting_angle = 0.0;
    long left_angle_temp = 0;
    long right_angle_temp = 0;

    float left_true_rad;
    float right_true_rad;
    int left_cnt = 0;
    int right_cnt = 0;
    float left_prev_rad;
    float right_prev_rad;

    float left_dist;
    float right_dist;
    long dt_micros = 0;
    long past_micros = 0;
    float left_past_dist = 0;
    float right_past_dist = 0; 

    float vx;
    float vth;

    //shared
    long angle_mask = 262080; // 0x111111111111000000: mask to obtain first 12 digits with position info
    float pi = 3.1415;
    float resolution = 0.08789;
    int debug;
    
    
  private:
    int _clock_pin;
    int _CSn_pin;
    int _left_input;
    int _right_input;
    float _left_resting_angle;
    float _right_resting_angle;
    float _wheel_diam; //in meters
    float _base_width; //in meters
};

#endif