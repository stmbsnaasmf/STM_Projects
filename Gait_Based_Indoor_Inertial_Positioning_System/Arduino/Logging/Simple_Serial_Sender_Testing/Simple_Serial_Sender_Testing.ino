void setup() 
{
  Serial.begin(115200);
  pinMode(13, OUTPUT);
}

void loop() 
{
  Serial.print(millis()); Serial.print("\t");
  Serial.print(float(analogRead(A0)), 6); Serial.print("\t");
  Serial.print(float(analogRead(A1)), 6); Serial.print("\t");
  Serial.print(float(analogRead(A2)), 20);
  Serial.println();
  delay(10);

  if (millis() > 15000)
    digitalWrite(13, HIGH);
}
