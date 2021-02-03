import processing.serial.*;
Serial udSerial;

PrintWriter output;

void setup() 
{
  size(400, 400);
  background(0, 255, 0);
  frameRate(100);

  udSerial = new Serial(this, Serial.list()[1], 115200);  //change the 0 to a 1 or 2 etc. to match your port
  String filePath = "C:/Users/Administrative/Desktop/Fall 2019/Human Computer Interaction/Project/Arduino/Logging/Processing Files/Logs.txt";//
  output = createWriter(filePath);
}

void draw() 
{
  //   printArray(Serial.list()); // to check serial
  if (udSerial.available() > 0) 
  {
    String SenVal = udSerial.readStringUntil('\n');
    if (SenVal != null) 
    {
      output.print(SenVal);
    }
  }
}
