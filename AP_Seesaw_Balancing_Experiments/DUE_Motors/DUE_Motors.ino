#include <Servo.h>
#define M1_PIN 6
#define M2_PIN 7

Servo m1, m2;


void setup()
{
  m1.attach(M1_PIN);
  m2.attach(M2_PIN);

  m1.writeMicroseconds(1000);
  m2.writeMicroseconds(1000);

  //delay(1000);

  //for (uint8_t i = 1000; i < 1100; i++)
  {
    //    m1.writeMicroseconds(i);
    //delay(100);
  }

}

void loop()
{
  //  for (uint8_t i = 1000; i < 1100; i++)
  {
    //    m1.writeMicroseconds(i);
    //    delay(10);
  }


  m1.writeMicroseconds(1000);
  m2.writeMicroseconds(1000);
  //  delay(60);
}
