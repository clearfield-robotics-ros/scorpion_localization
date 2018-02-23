#include <AS5045.h>

int ODOM_DELAY_MS = 20;
long dt_micros = 0;
long past_micros = 0;
float left_past_dist = 0;
float right_past_dist = 0;
float left_dist = 0;
float right_dist = 0;
//extern float vx;
//extern float vz;

encoder right_wheel(6, 5, 7, 101.6, true); //clk, csn, input, wheel diameter

void setup() {
  Serial.begin(115200);
  right_wheel.setup_rotary_encoder();
  right_wheel.calibrate_rotary_encoder();
}

void loop() {
   float right_dist = right_wheel.rotary_data();
   dt_micros = micros() - past_micros;
   float right_v = (right_dist - right_past_dist) * 1000 / dt_micros;
   right_past_dist = right_dist;
   past_micros = micros();
   Serial.print(right_dist);
   Serial.print("\t");
   Serial.println(right_v);
   delay(ODOM_DELAY_MS);
}
