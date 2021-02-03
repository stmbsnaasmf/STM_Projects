#include <SPI.h>
#include <SD.h>
#define SD_MODULE_SELECT_PIN 4 //SD card CS pin connected to pin 4 of Arduino

//#define FILENAME "mSxyzSI.txt"
#define FILENAME "XYZ.TXT"

void setup()
{
  Serial.begin(115200);

  if (!SD.begin(SD_MODULE_SELECT_PIN))
  {
    Serial.println("Card failed, or not present");
  }

//  File accelLogs = SD.open(FILENAME, O_CREAT | O_WRITE| O_APPEND);
  File accelLogs = SD.open(FILENAME, O_CREAT | FILE_WRITE);
  if (accelLogs)
  {
    accelLogs.println("millis, ax, ay, az");  //Header

    for (int i = 0; i < 10; i++)
    {
      Serial.println(accelLogs.write(i));
      accelLogs.println();
    }
    
    accelLogs.close();
    Serial.println("Done");
  }
  else
  {
    Serial.println("Error opening file in setup()");
  }
}

void loop()
{
  
  
}
