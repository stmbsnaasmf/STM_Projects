import processing.serial.*;
Serial udSerial;

PrintWriter output;

//#define LSB_SENSITIVITY 1670.71156  //  Should give in ms^-2

final float LSB_SENSITIVITY = 1670.71156;

void setup() 
{
  size(400, 400);
  background(0, 255, 0);
  frameRate(100);

  udSerial = new Serial(this, Serial.list()[1], 115200);  //change the 0 to a 1 or 2 etc. to match your port
  String filePath = "C:/Users/Administrative/Desktop/Fall 2019/Human Computer Interaction/Project/Arduino/Logging/Processing Files/Logs.txt";//
  output = createWriter(filePath);

  output.print("currentMillis");  
  output.print(',');
  output.print("ax");  
  output.print(',');
  output.print("ay");  
  output.print(',');
  output.println("az");
}

byte[] byteBuffer = new byte[16];
char[] charBuffer = new char[16];
int currentMillis = 0;
float ax, ay, az;
int[] MSB = new int[4];

void draw() 
{
  //   printArray(Serial.list()); // to check serial
  if (udSerial.available() > 0) 
  {
    int noOfBytesRead = udSerial.readBytes(byteBuffer);
    //println(noOfBytesRead);
    for (int i = 0; i < 16; i++)
    {
      charBuffer[i] = char(byteBuffer[i]);
    }

    MSB[0] = int(charBuffer[3]);
    MSB[0] = MSB[0] << 24;

    MSB[1] = int(charBuffer[2]);
    MSB[1] = MSB[1] << 16;

    MSB[2] = int(charBuffer[1]);
    MSB[2] = MSB[2] << 8;

    MSB[3] = int(charBuffer[0]);
    MSB[3] = MSB[3] << 0;
    currentMillis = MSB[0] | MSB[1] | MSB[2] | MSB[3];  
    //println(currentMillis);


    MSB[0] = int(charBuffer[7]);
    MSB[0] = MSB[0] << 24;

    MSB[1] = int(charBuffer[6]);
    MSB[1] = MSB[1] << 16;

    MSB[2] = int(charBuffer[5]);
    MSB[2] = MSB[2] << 8;

    MSB[3] = int(charBuffer[4]);
    MSB[3] = MSB[3] << 0;

    int ax_ = MSB[0] | MSB[1] | MSB[2] | MSB[3];    
    ax = Float.intBitsToFloat(ax_);
    ax = ax / LSB_SENSITIVITY;
    //println(ax);

    MSB[0] = int(charBuffer[11]);
    MSB[0] = MSB[0] << 24;

    MSB[1] = int(charBuffer[10]);
    MSB[1] = MSB[1] << 16;

    MSB[2] = int(charBuffer[9]);
    MSB[2] = MSB[2] << 8;

    MSB[3] = int(charBuffer[8]);
    MSB[3] = MSB[3] << 0;

    int ay_ = MSB[0] | MSB[1] | MSB[2] | MSB[3];    
    ay = Float.intBitsToFloat(ay_);
    ay = ay / LSB_SENSITIVITY;
    //println(ay);

    MSB[0] = int(charBuffer[15]);
    MSB[0] = MSB[0] << 24;

    MSB[1] = int(charBuffer[14]);
    MSB[1] = MSB[1] << 16;

    MSB[2] = int(charBuffer[13]);
    MSB[2] = MSB[2] << 8;

    MSB[3] = int(charBuffer[12]);
    MSB[3] = MSB[3] << 0;

    int az_ = MSB[0] | MSB[1] | MSB[2] | MSB[3];    
    az = Float.intBitsToFloat(az_);
    az = az / LSB_SENSITIVITY;
    //println(az);

    output.print(currentMillis);  
    output.print(", ");
    output.print(ax);  
    output.print(", ");
    output.print(ay);  
    output.print(", ");
    output.println(az);

    output.flush();

    print(currentMillis);  
    print(", ");
    print(ax);  
    print(", ");
    print(ay);  
    print(", ");
    println(az);


    //String SenVal = udSerial.readStringUntil('\n');
    //if (SenVal != null) 
    //{
    //  output.print(SenVal);
    //}
  }
}
