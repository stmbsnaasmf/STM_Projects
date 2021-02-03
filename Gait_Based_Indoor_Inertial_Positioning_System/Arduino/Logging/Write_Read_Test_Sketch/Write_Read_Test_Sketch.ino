#include <SD.h>
#include <SPI.h>

#define FILENAME "testlogs.txt"

File myFile;
int pinCS = 4; // Pin 4 on Arduino Uno

void setup()
{
  Serial.begin(115200);

  pinMode(pinCS, OUTPUT);

  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  }
  else
  {
    Serial.println("SD card initialization failed");
    return;
  }

  File myFile = SD.open(FILENAME, O_CREAT | O_WRITE);
  if (myFile)
  {
    for (int i = 0; i < 26; i++)
    {
      myFile.print(char(65 + i)); myFile.print(" ");  //Write whole ABC
    }

    myFile.println();
    myFile.println(analogRead(A0)); //Write an int
    myFile.println(3.14);           //Write a float

    myFile.close();
  }
  else
  {
    Serial.print("Could not open file for writing. The file is "); Serial.println(FILENAME);
  }

  Serial.println("Done with Writing/nReading now");

  File yourFile = SD.open(FILENAME, O_READ);

  if (yourFile)
  {
    while(yourFile.available())
    {
      Serial.print(yourFile.read());
    }
  }
  else
  {
    Serial.print("Could not open file for reading. The file is "); Serial.println(FILENAME);
  }

}

void loop()
{

}
