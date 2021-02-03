#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 10
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);  // Pass our oneWire reference to Dallas Temperature.

float currentTemperature = 0;
float lowestDesirableTemperature = 23;
float highestDesirableTemperature = 28;

uint64_t currentMilliseconds_u64t = 0;
uint8_t currentMilliseconds_u64t_UpdateIntervalMS = 10;
uint32_t currentMilliseconds_u64t_PreviousUpdateTimestamp = 0;

uint8_t time_offset_in_ten_minutes = 0; //millis = 0 @ 12:00 am, so this is the offset for that. If millis() = 0 at 1 am, the offset
//would be 6 so that milliseconds actually equal to = millis() + 6 * MILLISECONDS_IN_TEN_MINUTES = 0 + 3600000 = 1 am.
#define MILLISECONDS_IN_TEN_MINUTES 600000
#define TEN_MINUTES_IN_A_DAY 144        //6 * 24 = 144

#define NO_OF_CHANNELS 8  //Relay chanels
bool isRelayInputInverted = true; //Default (un-powered) position maintained at 5 volts instead of 0 volts.

#define HEATER 0
#define COOLING_FANS 1
#define WHITE_LIGHTS 2
#define BLUE_LIGHTS 3
#define BLUE_WHITE_LIGHTS_FANS 4
#define BLUE_LED_STRIP 5
#define FFN0 6  //Free for now
#define FFN1 7  //Free for now

#define CHANNEL_0_PIN 2   //Heater(220v)
#define CHANNEL_1_PIN 3   //Cooling Fans
#define CHANNEL_2_PIN 4   //White Lights
#define CHANNEL_3_PIN 5   //Blue Lights(220v)
#define CHANNEL_4_PIN 6   //Blue Lights' Fans //Also for white lights
#define CHANNEL_5_PIN 7   //Blue LED Strip(220v)
#define CHANNEL_6_PIN 8   //Free for now
#define CHANNEL_7_PIN 9   //Free for now

uint8_t ChannelPins[NO_OF_CHANNELS];

uint8_t ChannelOffsetsInTenMinutes[NO_OF_CHANNELS]; //So 6 represents 1 am as there are 6 times 10 minutes in an hour. 37 = 6:10 am
//Apply only for channels that are on a schedule. Do NOT apply on Heater channel as it is not dependent on a schedule.

uint8_t ChannelDurationsInTenMinutes[NO_OF_CHANNELS]; //Same as ChannelOffsetsInTenMinutes[NO_OF_CHANNELS].

bool isTheGivenChannelScheduled[NO_OF_CHANNELS];  //To keep track of which channels are scheduled and which are not.

bool isTheGivenChannelToBeOverridden[NO_OF_CHANNELS] = {0, 0, 0, 0, 0, 0, 0, 0};   //Which channels to override
bool ChannelsOverrideCommands[NO_OF_CHANNELS] = {0, 0, 0, 0, 0, 0, 0, 0};  //To keep them high or low

bool ChannelStates[NO_OF_CHANNELS];  //Current channnel status for each channel

#define RX_BUFFER_SIZE 50
char RxBuffer[RX_BUFFER_SIZE];
uint8_t rxBufferIndex = 0;

void setup()
{
  ChannelPins[0] = CHANNEL_0_PIN;
  ChannelPins[1] = CHANNEL_1_PIN;
  ChannelPins[2] = CHANNEL_2_PIN;
  ChannelPins[3] = CHANNEL_3_PIN;
  ChannelPins[4] = CHANNEL_4_PIN;
  ChannelPins[5] = CHANNEL_5_PIN;
  ChannelPins[6] = CHANNEL_6_PIN;
  ChannelPins[7] = CHANNEL_7_PIN;

  for (int i = 0; i < NO_OF_CHANNELS; i++)
  {
    digitalWrite(ChannelPins[i], LOW);
    pinMode(ChannelPins[i], OUTPUT);
    digitalWrite(ChannelPins[i], LOW);
    ChannelStates[i] = LOW;
  }

  for (int i = 0; i < NO_OF_CHANNELS; i++)
  {
    isTheGivenChannelScheduled[i] = false;  //Mark all channels as un-scheduled. Scheduled channels must be marked after this loop
    //and so explicitly.
  }

  ChannelOffsetsInTenMinutes[WHITE_LIGHTS] = 60;    //10 am
  ChannelDurationsInTenMinutes[WHITE_LIGHTS] = 48;  //10 am to 6 pm, 8 hours
  isTheGivenChannelScheduled[WHITE_LIGHTS] = true;

  ChannelOffsetsInTenMinutes[BLUE_WHITE_LIGHTS_FANS] = 48;    //8 am
  ChannelDurationsInTenMinutes[BLUE_WHITE_LIGHTS_FANS] = 72;  //8 am to 8 pm, 12 hours
  isTheGivenChannelScheduled[BLUE_WHITE_LIGHTS_FANS] = true;

  ChannelOffsetsInTenMinutes[BLUE_LIGHTS] = 48;    //8 am
  ChannelDurationsInTenMinutes[BLUE_LIGHTS] = 72;  //8 am to 8 pm, 12 hours
  isTheGivenChannelScheduled[BLUE_LIGHTS] = true;
  
  ChannelOffsetsInTenMinutes[BLUE_LED_STRIP] = 36;    //6 am
  ChannelDurationsInTenMinutes[BLUE_LED_STRIP] = 108;  //6 am to 12 am, 18 hours
  isTheGivenChannelScheduled[BLUE_LED_STRIP] = true;


  pinMode(13, OUTPUT);
  Serial.begin(115200);
  for (int i = 0; i < RX_BUFFER_SIZE; i++)
  {
    RxBuffer[i] = '\0';
  }
}

void loop()
{
  if (millis() - currentMilliseconds_u64t_PreviousUpdateTimestamp >= currentMilliseconds_u64t_UpdateIntervalMS)
  {
    currentMilliseconds_u64t += millis() - currentMilliseconds_u64t_PreviousUpdateTimestamp;

    currentMilliseconds_u64t_PreviousUpdateTimestamp = millis();            //Update the timestamp
  }

  uint8_t currentTimeInTenMinutes = uint32_t(((currentMilliseconds_u64t + time_offset_in_ten_minutes * MILLISECONDS_IN_TEN_MINUTES)
                                    / MILLISECONDS_IN_TEN_MINUTES) % 144);  //Resets at 144(24 hours mark)


  for (int i = 0; i < NO_OF_CHANNELS; i++)
  {
    if (true == isTheGivenChannelToBeOverridden[i])  //The Channel is to be overridden
    {
      if (true == ChannelsOverrideCommands[i])
      {
        ChannelStates[i] = HIGH;  //The channel is commanded to be turned ON.
      }
      else
      {
        ChannelStates[i] = LOW;  //The channel is commanded to be turned OFF.
      }
    }
    else  //The channel is supposed to work according to the schedule if it is bound to one.
    {
      if (true == isTheGivenChannelScheduled[i])  //The channel is bound to a schedule.
      {
        if ((ChannelOffsetsInTenMinutes[i] + ChannelDurationsInTenMinutes[i]) > TEN_MINUTES_IN_A_DAY)  //If reset happens
          //during the schedule
        {
          if (currentTimeInTenMinutes  >= ChannelOffsetsInTenMinutes[i] || currentTimeInTenMinutes <
              (ChannelOffsetsInTenMinutes[i] + ChannelDurationsInTenMinutes[i]) % TEN_MINUTES_IN_A_DAY )
          {
            ChannelStates[i] =  HIGH;
          }
          else
          {
            ChannelStates[i] =  LOW;
          }
        }
        else
        {
          if (currentTimeInTenMinutes  >= ChannelOffsetsInTenMinutes[i] && currentTimeInTenMinutes <
              (ChannelOffsetsInTenMinutes[i] + ChannelDurationsInTenMinutes[i]))
          {
            ChannelStates[i] =  HIGH;
          }
          else
          {
            ChannelStates[i] =  LOW;
          }
        }
      }
      else  //The channel works on some other condition.(Eg: Heater on temperature)
      {
        //These channels are to be treated outside of this main channel loop. This loop is for scheduled channels.
      }
    }
  }

  if (true == isTheGivenChannelToBeOverridden[HEATER])  //Heater
  {
    ChannelStates[HEATER] = ChannelsOverrideCommands[HEATER];
  }
  else
  {
    if (currentTemperature < lowestDesirableTemperature)
    {
      ChannelStates[HEATER] = HIGH;
    }
    else
    {
      ChannelStates[HEATER] = LOW;
    }
  }

  if (true == isTheGivenChannelToBeOverridden[COOLING_FANS])  //Cooling Fans
  {
    ChannelStates[COOLING_FANS] = ChannelsOverrideCommands[COOLING_FANS];
  }
  else
  {
    if (currentTemperature > highestDesirableTemperature)
    {
      ChannelStates[COOLING_FANS] = HIGH;
    }
    else
    {
      ChannelStates[COOLING_FANS] = LOW;
    }
  }

  for (int i = 0; i < NO_OF_CHANNELS; i++)  //Actual pin writing takes place here.
  {
    if (true == isRelayInputInverted)
    {
      digitalWrite(ChannelPins[i], !ChannelStates[i]);  //Notice the NOT.
    }
    else
    {
      digitalWrite(ChannelPins[i], ChannelStates[i]);
    }

  }

  /*
     Command string parsing code below.
  */

  int noOfBytesAvailable = 0;
  noOfBytesAvailable  = Serial.available();
  if (noOfBytesAvailable > 0 && noOfBytesAvailable < RX_BUFFER_SIZE)
  {
    //    Serial.println("H");

    for (int i = 0; i < noOfBytesAvailable; i++)
    {
      RxBuffer[i] = Serial.read();
    }

    //    Serial.print(RxBuffer);

    if (areCharArraysSame(RxBuffer, "RESET ALL", 9))
    {
      for (int i = 0; i < NO_OF_CHANNELS; i++)
      {
        isTheGivenChannelToBeOverridden[i] = false;
      }
      Serial.println("All channels reset");
    }
    else if (areCharArraysSame(RxBuffer, "RESET ", 6))  //Ex: RESET 5
    {
      if (RxBuffer[7] != NULL)        //If index exists...
      {
        int index = int(RxBuffer[7]);
        index = index - 48;

        if (index >= 0 && index <= 7) //...and if it is valid
        {
          isTheGivenChannelToBeOverridden[index] = false;
          Serial.print(index);
          Serial.println(" channel reset");
        }
      }
    }
    else if (areCharArraysSame(RxBuffer, "SET ALL", 7)) //Ex: SET ALL 1
    {
      if (RxBuffer[8] != NULL)  //If state exists
      {
        bool state = RxBuffer[8] - 48;  //-48 because actual 0 is needed instead of ASCII 0 which is 48.

        for (int i = 0; i < NO_OF_CHANNELS; i++)
        {
          isTheGivenChannelToBeOverridden[i] = true;
          ChannelsOverrideCommands[i] = state;
        }
        Serial.print("All channels set ");
        Serial.println(state);
      }
    }
    else if (areCharArraysSame(RxBuffer, "SET TIME ", 9))
    {
      uint8_t noOfDigits = 0; //No of digits in string after "SET TIME "  //Last space included
      uint8_t index = 9;
      while (RxBuffer[index] >= 48 && RxBuffer[index] <= 57)
      {
        index++;
        noOfDigits++;
      }

      if (noOfDigits != 0)  //There is atleast one digit starting from the 9th index(inclusive)
      {
        if (noOfDigits > 3) //If the string contained a number larger than 3 digits
        {
          noOfDigits = 3;
        }

        char digitArray[4];
        for (int i = 0; i < 4; i++)
        {
          digitArray[i] = '\0';
        }

        for (int i = 0; i < noOfDigits; i++)  //At most runs three times
        {
          digitArray[i] = RxBuffer[9 + i];
        }

        time_offset_in_ten_minutes = atoi(digitArray);

        Serial.print("Set time to ");
        Serial.println(time_offset_in_ten_minutes);
      }
      else
      {
        Serial.println("Invalid SET TIME command!");
      }
    }
    else if (areCharArraysSame(RxBuffer, "SET ", 4)) //Ex: SET 5 1
    {
      if (RxBuffer[4] != NULL)  //If channel no. exists
      {
        int index = int(RxBuffer[4]);
        index = index - 48;

        if (index >= 0 && index <= 7) //...and if it is valid for a single channel
        {
          if (RxBuffer[6] != NULL)  //If state exists
          {
            bool state = RxBuffer[6] - 48;  //-48 because actual 0 is needed instead of ASCII 0 which is 48.

            isTheGivenChannelToBeOverridden[index] = true;
            ChannelsOverrideCommands[index] = state;
            Serial.print(index);
            Serial.print(" channel set ");
            Serial.println(state);
          }
        }
        else if (8 == index)  //SET 8 x0x1x2x3x4x5x6x7 format
        {
          bool isTheFormatValid = false;

          if (RxBuffer[5] == ' ' && (RxBuffer[13] != NULL)) //Check for space before the state string and if its last
            //character is non-null. State string = 00001111
          {
            isTheFormatValid = true;
          }

          bool States[8];

          for (int i = 0; i < 8; i++)
          {
            States[i] = RxBuffer[6 + i] - 48; //-48 because actual 0 is needed instead of ASCII 0 which is 48.

            isTheGivenChannelToBeOverridden[i] = true;
            ChannelsOverrideCommands[i] = States[i];
          }

          Serial.print("All channels set as ");
          for (int i = 0; i < NO_OF_CHANNELS; i++)
          {
            Serial.print(States[i]);
          }
          Serial.println();

        }
      }
    }
    else if (areCharArraysSame(RxBuffer, "INFO", 4))
    {
      Serial.print("Time: ");
      Serial.println(currentTimeInTenMinutes);

      Serial.print("Temperature: ");
      Serial.println(currentTemperature);

      Serial.println("Channel states:");
      for (int i = 0; i < NO_OF_CHANNELS; i++)
      {
        Serial.print("Ch ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(ChannelStates[i]);

      }

      Serial.println("Sceduled Channels:");
      for (int i = 0; i < NO_OF_CHANNELS; i++)
      {
        if (isTheGivenChannelScheduled[i])
        {
          Serial.print("Ch ");
          Serial.print(i);
          Serial.print(": From: ");
          Serial.print(ChannelOffsetsInTenMinutes[i]);
          Serial.print(" Duration: ");
          Serial.println(ChannelDurationsInTenMinutes[i]);
        }
      }

      Serial.print("Lower temperature limit: ");
      Serial.println(lowestDesirableTemperature);
      Serial.print("Upper temperature limit: ");
      Serial.println(highestDesirableTemperature);
    }
    else if (areCharArraysSame(RxBuffer, "TEMP", 4))
    {
      Serial.print("Temperature: ");
      Serial.println(currentTemperature);
    }
    else if (areCharArraysSame(RxBuffer, "TIME", 4))
    {
      Serial.print("Time: ");
      //Serial.println(timeInTenMinutes);
      Serial.println(currentTimeInTenMinutes);
    }





    if (areCharArraysSame(RxBuffer, "A", 1))
    {
      digitalWrite(13, HIGH);
      Serial.println("WH");
    }
    if (areCharArraysSame(RxBuffer, "C", 1))
    {
      digitalWrite(13, LOW);
      Serial.println("WL");
    }

    for (int i = 0; i < noOfBytesAvailable; i++)  //Resetting the buffer
    {
      RxBuffer[i] = '\0';
    }
  }
  else  //The message may be larger than 50 chars. Empty the buffer.
  {
    while (Serial.available())
    {
      char abc = Serial.read();
    }
  }

  sensors.requestTemperatures(); // Send the command to get temperatures
  currentTemperature = sensors.getTempCByIndex(0);


  delay(500);
}

bool areCharArraysSame(char* arr, char* brr, uint8_t rrSize)
{
  for (int i = 0; i < rrSize; i++)
  {
    if (arr[i] != brr[i])
    {
      return false;
    }
  }
  return true;
}
