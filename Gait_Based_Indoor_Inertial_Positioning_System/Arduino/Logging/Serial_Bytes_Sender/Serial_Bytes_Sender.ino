int32_t currentMillis;
float ax, ay, az;
uint8_t* bytePointer; //Pointer to type uint8_t

void setup()
{
  Serial.begin(115200);
}

void loop()
{
//  ax = float(analogRead(A0));
//  ay = float(analogRead(A1));
//  az = float(analogRead(A2));
  currentMillis = millis();
  ax = 100.001;
  ay = -222.333;
  az = 3.14159;

  //*bytePointer = currentMillis; //Does not work and appears to be wrong.
  bytePointer = (uint8_t *) (& currentMillis); // get the address of the variable currentMillis

  for (int i = 0; i < 4; i++)
  {
    Serial.write(bytePointer[i]);
  }

  //  Serial.println("==");
  //  Serial.print(bytePointer[3], BIN);
  //  Serial.print("\t");
  //  Serial.print(bytePointer[2], BIN);
  //  Serial.print("\t");
  //  Serial.print(bytePointer[1], BIN);
  //  Serial.print("\t");
  //  Serial.println(bytePointer[0], BIN);
  //  while(1)

  bytePointer = (uint8_t *) (& ax);
  for (int i = 0; i < 4; i++)
  {
    Serial.write(bytePointer[i]);
  }

  bytePointer = (uint8_t *) (& ay);
  for (int i = 0; i < 4; i++)
  {
    Serial.write(bytePointer[i]);
  }

  bytePointer = (uint8_t *) (& az);
  for (int i = 0; i < 4; i++)
  {
    Serial.write(bytePointer[i]);
  }
  delay(10);
}
