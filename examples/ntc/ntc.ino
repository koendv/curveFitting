/* An NTC (negative temperature coefficient) resistor is used to measure temperature.
   Model the resistor as resistance = b * exp(a * temp).
 */

#include "curveFitting.h"

expFit exp_fit;

void setup() {
  while (!Serial)
    ;

  /* simulates (temperature, resistance) data from sensors */
  exp_fit.add(0, 5896);
  exp_fit.add(10, 3792);
  exp_fit.add(20, 2500);
  exp_fit.add(30, 1707);
  exp_fit.add(40, 1175);
  exp_fit.add(50, 835);
  exp_fit.add(60, 596);
  exp_fit.add(70, 436);
  exp_fit.add(80, 323);
  exp_fit.add(90, 243);
  exp_fit.add(100, 187);
  exp_fit.add(110, 144);
  exp_fit.add(120, 113);
  exp_fit.add(130, 89);

  Serial.print("resistance = ");
  Serial.print(exp_fit.b(), 4);
  Serial.print(" * exp(");
  Serial.print(exp_fit.a(), 4);
  Serial.println(" * temp)");
  Serial.print("correlation: ");
  Serial.println(exp_fit.r());
  Serial.print("resistance at 35 degree = ");
  Serial.println(exp_fit.y(35));
  /* output:
  resistance = 4646.8237 * exp(-0.0319 * temp)
  correlation: -1.00
  resistance at 35 degree = 1518.86
   */
}

void loop() {
  // put your main code here, to run repeatedly:
}
