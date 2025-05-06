/*
 * Modeling LED brightness over time.
 */

#include "curveFitting.h"

expFit exp_fit;

void setup() {
  while (!Serial)
    ;

  /* simulates getting brightness data from sensor */

  /* data is (hours, brightness) */
  exp_fit.add(1000, 1.0032);
  exp_fit.add(2000, 0.9924);
  exp_fit.add(3000, 0.9919);
  exp_fit.add(4000, 0.9677);
  exp_fit.add(5000, 0.9493);
  exp_fit.add(6000, 0.9258);

  /* calculate model */
  Serial.print("brightness = ");
  Serial.print(exp_fit.b(), 4);
  Serial.print(" * exp(- hours / ");
  Serial.print(-1 / exp_fit.a(), 4);
  Serial.println(")");
  Serial.print("correlation = ");
  Serial.println(exp_fit.r(), 4);
  Serial.print("70% brightness after ");
  Serial.print(exp_fit.x(0.7));
  Serial.println(" hours");
}

void loop() {
  // put your main code here, to run repeatedly:
}
