uint16_t r1, r2;

#define PIN_1 A7

void setup()
{
  Serial.begin(115200);
//  analogReadResolution(12);
}

void loop()
{
  r1 = analogRead(PIN_1);
  delay(1);
  r1 = analogRead(PIN_1);
  Serial.print("r1: "); Serial.println(r1);
  r1 = map(r1, 0, 4095, 1000, 2000);
  Serial.print("r1 mapped: "); Serial.println(r1);


/*
  r2 = analogRead(A0);
  delay(1);
  r2 = analogRead(A0);
  Serial.print("r2: "); Serial.println(r2);
  r2 = map(r2, 0, 4095, 1000, 2000);
  Serial.print("r2 mapped: "); Serial.println(r2);
*/

  delay(10);

}
