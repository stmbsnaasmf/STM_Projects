#include <Servo.h>
#define M1_PIN 5  //Right
#define M2_PIN 6  //Left

#define INTERVAL_MILLISECONDS 1
#define INTERVAL_MICROSECINDS 1000
#define USUAL_ESC_POSITION 1200

#define MAX_ALLOWED_POSITION 1500   //  (°-°): I am Areej.

Servo m1, m2;

uint16_t m1Position, m2Position;

uint32_t previousMillis = 0;


int16_t currentRoll;
int16_t rollError;
int16_t desiredRoll = 0000;     //1000 = 10°

uint32_t proportionalConstant = 1000;  //Units in Hundreds          //800, 10000, 2 work ok.  //1200, 1500, 1000, 1000, 10 work fine.

//800 undershoot

uint32_t sumOfErrors;
uint16_t integralConstant = 10000;

int16_t rollErrorRate;
uint16_t derivativeConstant = 500;       //100x units
int16_t previousRollError;

uint32_t loopTime;
uint8_t loopCounter = 0;
uint32_t timeForLoopCounter;


uint32_t twoDegreeStableTime;
uint32_t threeDegreeStableTime;
uint32_t fiveDegreeStableTime;
uint32_t tenDegreeStableTime;

bool wasPreviouslyTwoDegreeStable = false;
bool wasPreviouslyThreeDegreeStable = false;
bool wasPreviouslyFiveDegreeStable = false;
bool wasPreviouslyTenDegreeStable = false;

uint32_t disturbaceTimer = 0;
bool isToBeDisturbed = false;
uint16_t previousProportionalConstant;
































//UPDATED AND CORRECT OFFSETS


#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

MPU6050 mpu;

// uncomment "OUTPUT_READABLE_QUATERNION" if you want to see the actual
// quaternion components in a [w, x, y, z] format (not best for parsing
// on a remote host such as Processing or something though)
//#define OUTPUT_READABLE_QUATERNION

// uncomment "OUTPUT_READABLE_EULER" if you want to see Euler angles
// (in degrees) calculated from the quaternions coming from the FIFO.
// Note that Euler angles suffer from gimbal lock (for more info, see
// http://en.wikipedia.org/wiki/Gimbal_lock)
//#define OUTPUT_READABLE_EULER

// uncomment "OUTPUT_READABLE_YAWPITCHROLL" if you want to see the yaw/
// pitch/roll angles (in degrees) calculated from the quaternions coming
// from the FIFO. Note this also requires gravity vector calculations.
// Also note that yaw/pitch/roll angles suffer from gimbal lock (for
// more info, see: http://en.wikipedia.org/wiki/Gimbal_lock)
#define OUTPUT_READABLE_YAWPITCHROLL

// uncomment "OUTPUT_READABLE_REALACCEL" if you want to see acceleration
// components with gravity removed. This acceleration reference frame is
// not compensated for orientation, so +X is always +X according to the
// sensor, just without the effects of gravity. If you want acceleration
// compensated for orientation, us OUTPUT_READABLE_WORLDACCEL instead.
//#define OUTPUT_READABLE_REALACCEL

// uncomment "OUTPUT_READABLE_WORLDACCEL" if you want to see acceleration
// components with gravity removed and adjusted for the world frame of
// reference (yaw is relative to initial orientation, since no magnetometer
// is present in this case). Could be quite handy in some cases.
//#define OUTPUT_READABLE_WORLDACCEL

// uncomment "OUTPUT_TEAPOT" if you want output that matches the
// format used for the InvenSense teapot demo
//#define OUTPUT_TEAPOT



//#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
//bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
//VectorInt16 aa;         // [x, y, z]            accel sensor measurements
//VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
//VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
//float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
  mpuInterrupt = true;
}

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================







void setup()
{
  m1.attach(M1_PIN);
  m2.attach(M2_PIN);

  m1.writeMicroseconds(1000);
  m2.writeMicroseconds(1000);

  delay(2500);

  //  if (millis() - previousMillis >= INTERVAL_MILLISECONDS)
  {

  }

  timeForLoopCounter = millis();

  randomSeed(analogRead(A0));































  {
    // join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();

    //TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
    Wire.setClock(500000L); //To experiment.  //500 KHz speed

#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
#endif

    Serial.begin(115200);

    // initialize device
    //    Serial.println(F("Initializing I2C devices..."));
    //    mpu.initialize();

    // verify connection
    //    Serial.println(F("Testing device connections..."));
    //    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // wait for ready
    //    Serial.println(F("\nSend any character to begin DMP programming and demo: "));
    //    while (Serial.available() && Serial.read()); // empty buffer
    //    while (!Serial.available());                 // wait for data
    //   while (Serial.available() && Serial.read()); // empty buffer again

    // load and configure the DMP
    //    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity  //These are updated for the particular sensor in use now.
    mpu.setXGyroOffset(45);
    mpu.setYGyroOffset(-7);
    mpu.setZGyroOffset(40);
    mpu.setZAccelOffset(853); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
      // turn on the DMP, now that it's ready
      //        Serial.println(F("Enabling DMP..."));
      mpu.setDMPEnabled(true);

      // enable Arduino interrupt detection
      //        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
      attachInterrupt(2, dmpDataReady, RISING);
      mpuIntStatus = mpu.getIntStatus();

      // set our DMP Ready flag so the main loop() function knows it's okay to use it
      //        Serial.println(F("DMP ready! Waiting for first interrupt..."));
      dmpReady = true;

      // get expected DMP packet size for later comparison
      packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
      // ERROR!
      // 1 = initial memory load failed
      // 2 = DMP configuration updates failed
      // (if it's going to break, usually the code will be 1)
      //        Serial.print(F("DMP Initialization failed (code "));
      //        Serial.print(devStatus);
      //        Serial.println(F(")"));
    }

    // configure LED for output
    //    pinMode(LED_PIN, OUTPUT);
  }
}

void loop()
{
  previousMillis = millis();

  ++loopCounter;

  if (millis() - timeForLoopCounter >= 1000)
  {
    Serial.print("if(): "); Serial.println(millis() - timeForLoopCounter);

    timeForLoopCounter = millis();
    Serial.print("Loop counter: "); Serial.println(loopCounter);
    loopCounter = 0;
    Serial.println("================================================================================================================");
    //    delay(1000);

  }

  theFunction();


  {
    // if programming failed, don't try to do anything
    if (!dmpReady) return;



    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize)       //The Problematic While Loop
    {
      /*
            {
              //previousMillis = millis();



              m1Position = USUAL_ESC_POSITION + 20;
              m2Position = USUAL_ESC_POSITION;

              Serial.print("Usual ESC pos: "); Serial.println(USUAL_ESC_POSITION);

              currentRoll = (ypr[2] * 180 / M_PI) * 100;


              if (millis() - previousMillis >= INTERVAL_MILLISECONDS)
              {
                previousMillis = millis();

                rollError = currentRoll - desiredRoll; //dR = 40, cR = 45, eR = 45 - 40 = 5.  dr=355, cr=350, er=350-355=-5

                rollErrorRate = rollError - previousRollError;
                previousRollError = rollError;

                Serial.print("Roll Error: "); Serial.println(rollError);
              }

              if (rollError >  -100 && rollError < 100)   //+-3 degrees
              {
                sumOfErrors = 0;
              }

              Serial.print("Roll error rate: "); Serial.println(rollErrorRate);

              //fz 331

              if (rollError > 0) //Right side down, increase right(m1)'s thrust.
              {
                sumOfErrors += rollError;
                Serial.print("Sum of errors: "); Serial.println(sumOfErrors);

                if (rollError < 1000)   //10 degrees
                {
                  m1Position += 0 + (((rollError * proportionalConstant) / 10000) + (sumOfErrors / integralConstant) +
                                     (rollErrorRate * derivativeConstant)) ; //From 100 * 100   //P + I
                }
                else
                {
                  m1Position += 0 + (((rollError * proportionalConstant) / 10000) + (sumOfErrors / integralConstant) +
                                     (rollErrorRate * derivativeConstant)); //From 100 * 100   //P + I
                }

              }

              if (rollError < 0)  //Left side down, increase left(m2)'s thrust.
              {
                Serial.print("Roll Error 2: "); Serial.println(rollError);

                sumOfErrors += (rollError * (-1));
                Serial.print("Sum of errors: "); Serial.println(sumOfErrors);

                if ((rollError * (-1)) < 1000)
                {
                  m2Position += (((rollError * (-1) * proportionalConstant) / 10000) + (sumOfErrors / integralConstant) +
                                 (rollErrorRate * (-1) * derivativeConstant));
                }
                else
                {
                  m2Position += (((rollError * (-1) * proportionalConstant) / 10000) + (sumOfErrors / integralConstant) +
                                 (rollErrorRate * (-1) * derivativeConstant));
                }

              }














              Serial.print("m1Position: "); Serial.println(m1Position);
              Serial.print("m2Position: "); Serial.println(m2Position);

              loopCounter++;

              m1.writeMicroseconds(constrain(m1Position, 1180, MAX_ALLOWED_POSITION));
              m2.writeMicroseconds(constrain(m2Position, 1180, MAX_ALLOWED_POSITION));

              //m1.writeMicroseconds(1000);
              //m2.writeMicroseconds(1000);

              //m1.writeMicroseconds(2000);
              //m2.writeMicroseconds(1800);


            }


      */





















      //theFunction();




      // other program behavior stuff here
      // .
      // .
      // .
      // if you are really paranoid you can frequently test in between other
      // stuff to see if mpuInterrupt is true, and if so, "break;" from the
      // while() loop to immediately process the MPU data
      // .
      // .
      // .

    }



    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
      // reset so we can continue cleanly
      mpu.resetFIFO();
      //        Serial.println(F("FIFO overflow!"));

      // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
      // wait for correct available data length, should be a VERY short wait
      while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();



      // read a packet from FIFO
      mpu.getFIFOBytes(fifoBuffer, packetSize);



      // track FIFO count here in case there is > 1 packet available
      // (this lets us immediately read more without waiting for an interrupt)
      fifoCount -= packetSize;



#ifdef OUTPUT_READABLE_YAWPITCHROLL
      // display Euler angles in degrees
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

      ///*
      Serial.print("ypr\t");
      Serial.print(ypr[0] * 180 / M_PI);
      Serial.print("\t");
      Serial.print(ypr[1] * 180 / M_PI);
      Serial.print("\t");
      Serial.println(ypr[2] * 180 / M_PI);
      //*/
#endif

      /*
              #ifdef OUTPUT_READABLE_REALACCEL
                  // display real acceleration, adjusted to remove gravity
                  mpu.dmpGetQuaternion(&q, fifoBuffer);
                  mpu.dmpGetAccel(&aa, fifoBuffer);
                  mpu.dmpGetGravity(&gravity, &q);
                  mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
                  Serial.print("areal\t");
                  Serial.print(aaReal.x);
                  Serial.print("\t");
                  Serial.print(aaReal.y);
                  Serial.print("\t");
                  Serial.println(aaReal.z);
              #endif
      */

      /*
              #ifdef OUTPUT_READABLE_WORLDACCEL
                  // display initial world-frame acceleration, adjusted to remove gravity
                  // and rotated based on known orientation from quaternion
                  mpu.dmpGetQuaternion(&q, fifoBuffer);
                  mpu.dmpGetAccel(&aa, fifoBuffer);
                  mpu.dmpGetGravity(&gravity, &q);
                  mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
                  mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
                  Serial.print("aworld\t");
                  Serial.print(aaWorld.x);
                  Serial.print("\t");
                  Serial.print(aaWorld.y);
                  Serial.print("\t");
                  Serial.println(aaWorld.z);
              #endif
      */
    }
  }


  ///////////////////////////////////////////////////////////


  //Serial.println(millis() - previousMillis);




















  loopTime = millis() - previousMillis;
  //  Serial.print("Loop time: "); Serial.println(loopTime);

}


void theFunction()
{
  //Produce a disturbance
  //  /*
  if (isToBeDisturbed = true)
  {
    proportionalConstant = previousProportionalConstant;
    isToBeDisturbed = false;
  }
  uint32_t now = millis();

  if (now - disturbaceTimer >= 10000)
  {
    if (now - fiveDegreeStableTime >= 2 * loopTime) //To see if fiveDegreeStableTime is valid.
    {
      if (fiveDegreeStableTime >= 10000)
      {
        disturbaceTimer = now;

        desiredRoll = random(0, 3000);
        if (random(0, 2000) >= 999)
        {
          desiredRoll *= (-1);
        }
        Serial.print("= = = = = = = = New Roll: "); Serial.print(desiredRoll); Serial.println(" = = = = = = = =");


        isToBeDisturbed = true;
        previousProportionalConstant = proportionalConstant;
        proportionalConstant = 0;
      }
    }
  }




  //  */



















  //=============================================================================================================================

  if (rollError > -1000 && rollError < 1000)    //10 degrees
  {
    //uint32_t now = millis();

    if (true == wasPreviouslyTenDegreeStable)
    {
      tenDegreeStableTime = loopTime + tenDegreeStableTime;
    }
    else
    {
      wasPreviouslyTenDegreeStable = true;
      //tenDegreeStableTime = now;
    }                                     // (°-°): Programming is so tiring.   <--- So TRUEeeeeeeeeeeeeeeeee


    if (rollError > -500 && rollError < 500)    //5 degrees
    {
      if (true == wasPreviouslyFiveDegreeStable)
      {
        fiveDegreeStableTime = loopTime + fiveDegreeStableTime;
      }
      else
      {
        wasPreviouslyFiveDegreeStable = true;
        //fiveDegreeStableTime = now;
      }


      if (rollError > -300 && rollError < 300)    //3 degrees
      {
        if (true == wasPreviouslyThreeDegreeStable)
        {
          threeDegreeStableTime = loopTime + threeDegreeStableTime;
        }
        else
        {
          wasPreviouslyThreeDegreeStable = true;
          //threeDegreeStableTime = now;
        }


        if (rollError > -200 && rollError < 200)    //2 degrees
        {
          if (true == wasPreviouslyTwoDegreeStable)
          {
            twoDegreeStableTime = loopTime + twoDegreeStableTime;
          }
          else
          {
            wasPreviouslyTwoDegreeStable = true;
            //twoDegreeStableTime = now;
          }
        }
        else
        {
          wasPreviouslyTwoDegreeStable = false;
          twoDegreeStableTime = 0;
        }

      }
      else
      {
        wasPreviouslyThreeDegreeStable = false;
        threeDegreeStableTime = 0;

        wasPreviouslyTwoDegreeStable = false;
        twoDegreeStableTime = 0;
      }

    }
    else
    {
      wasPreviouslyFiveDegreeStable = false;
      fiveDegreeStableTime = 0;

      wasPreviouslyThreeDegreeStable = false;
      threeDegreeStableTime = 0;

      wasPreviouslyTwoDegreeStable = false;
      twoDegreeStableTime = 0;

    }

  }
  else
  {
    wasPreviouslyTenDegreeStable = false;
    tenDegreeStableTime = 0;

    wasPreviouslyFiveDegreeStable = false;
    fiveDegreeStableTime = 0;

    wasPreviouslyThreeDegreeStable = false;
    threeDegreeStableTime = 0;

    wasPreviouslyTwoDegreeStable = false;
    twoDegreeStableTime = 0;
  }

  //Serial.print("Two degree stable time: "); Serial.println(twoDegreeStableTime);
  //Serial.print("Three degree stable time: "); Serial.println(threeDegreeStableTime);
  //Serial.print("Five degree stable time: "); Serial.println(fiveDegreeStableTime);
  Serial.print("FDST: "); Serial.println(fiveDegreeStableTime);
  //Serial.print("Ten degree stable time: "); Serial.println(tenDegreeStableTime);


  //=======================================================================================================================//

  {
    m1Position = USUAL_ESC_POSITION + 20;
    m2Position = USUAL_ESC_POSITION;

    //Serial.print("Usual ESC pos: "); Serial.println(USUAL_ESC_POSITION);

    currentRoll = (ypr[2] * 180 / M_PI) * 100;    //Unit: Centidegree = 1 degree x 100

    //    if (millis() - previousMillis >= 10)
    {
      //previousMillis = millis();

      rollError = currentRoll - desiredRoll; //dR = 40, cR = 45, eR = 45 - 40 = 5.  dr=355, cr=350, er=350-355=-5

      rollErrorRate = rollError - previousRollError;      //Unit: Centidegrees per loopTime(10-20 ms)
      previousRollError = rollError;

      Serial.print("Roll Error: "); Serial.println(rollError);
    }

    if (rollError >  -100 && rollError < 100)   //+-1 degrees       //Unit: Centidegree = 1 degree x 100
    {
      sumOfErrors = 0;
    }

    //   Serial.print("Roll error rate: "); Serial.println(rollErrorRate);

    //fz 331

    if (rollError > 0) //Right side down, increase right(m1)'s thrust.
    {
      sumOfErrors += rollError;
      //Serial.print("Sum of errors: "); Serial.println(sumOfErrors);

      //if (rollError < 1000)   //10 degrees
      {
        m1Position += 0 + (((rollError * proportionalConstant) / 10000) + (sumOfErrors / integralConstant) +
                           (rollErrorRate * (derivativeConstant / 100))) ; //From 100 * 100   //P + I

        Serial.print("pTerm: "); Serial.println(((rollError * proportionalConstant) / 10000));
      }
      //else
      {
        //  m1Position += 0 + (((rollError * proportionalConstant) / 10000) + (sumOfErrors / integralConstant) +
        //                     (rollErrorRate * (derivativeConstant / 100))); //From 100 * 100   //P + I
      }

    }

    if (rollError < 0)  //Left side down, increase left(m2)'s thrust.
    {
      //Serial.print("Roll Error 2: "); Serial.println(rollError);

      sumOfErrors += (rollError * (-1));
      //Serial.print("Sum of errors: "); Serial.println(sumOfErrors);

      //  if ((rollError * (-1)) < 1000)
      {
        m2Position += (((rollError * (-1) * proportionalConstant) / 10000) + (sumOfErrors / integralConstant) +
                       (rollErrorRate * (-1) * (derivativeConstant / 100)));

        Serial.print("pTermNeg: "); Serial.println(((rollError * (-1) * proportionalConstant) / 10000));
      }
      //else
      {
        // m2Position += (((rollError * (-1) * proportionalConstant) / 10000) + (sumOfErrors / integralConstant) +
        //               (rollErrorRate * (-1) * (derivativeConstant / 100)));
      }

    }











    Serial.print("m1Position: "); Serial.println(m1Position);
    Serial.print("m2Position: "); Serial.println(m2Position);



    m1.writeMicroseconds(constrain(m1Position, 1180, MAX_ALLOWED_POSITION));
    m2.writeMicroseconds(constrain(m2Position, 1180, MAX_ALLOWED_POSITION));

    //m1.writeMicroseconds(1000);
    //m2.writeMicroseconds(1000);

    //m1.writeMicroseconds(2000);
    //m2.writeMicroseconds(1800);
  }
}

