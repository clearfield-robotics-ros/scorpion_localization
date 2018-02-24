#include <AS5045_scorpion.h>

scorpion_wheels scorpion(6, 7, 4, 5, 101.6, 1.10515);//CSn, Clk, left, right, wheel diam, base_width

void setup() {
  Serial.begin(115200);
  scorpion.setup_rotary_encoders();
  scorpion.calibrate_rotary_encoders();
//  right_wheel.setup_rotary_encoder();
//  right_wheel.calibrate_rotary_encoder();
}

void loop() {
//   float right_dist = right_wheel.rotary_data();
//   dt_micros = micros() - past_micros;
//   float right_v = (right_dist - right_past_dist) * 1000 / dt_micros;
//   right_past_dist = right_dist;
//   past_micros = micros();
//   Serial.print(right_dist);
//   Serial.print("\t");
//   Serial.println(right_v);
//   delay(ODOM_DELAY_MS);
  scorpion.get_vx_vth();
  
  Serial.print(scorpion.left_dist);
  Serial.print("\t");
  Serial.print(scorpion.right_dist);
  Serial.print("\t");
  Serial.print(scorpion.vx);
  Serial.print("\t");
  Serial.println(scorpion.vth);
}
