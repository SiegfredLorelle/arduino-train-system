#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h> 

const int STATIONS_SIZE = 6;
const int TRAINS_SIZE = 3;


Servo SERVOS[STATIONS_SIZE];

int servosPins[STATIONS_SIZE] = {
  3,
  5,
  6,
  9,
  10,
  11,
};

LiquidCrystal_I2C lcdStation1(0x27, 16, 2);
LiquidCrystal_I2C lcdStation2(0x26, 16, 2);
LiquidCrystal_I2C lcdStation3(0x25, 16, 2);
LiquidCrystal_I2C lcdStation4(0x24, 16, 2);
LiquidCrystal_I2C lcdStation5(0x23, 16, 2);
LiquidCrystal_I2C lcdStation6(0x22, 16, 2);
LiquidCrystal_I2C LCDS[STATIONS_SIZE] = {
  lcdStation1,
  lcdStation2,
  lcdStation3,
  lcdStation4,
  lcdStation5,
  lcdStation6,
};

String stationNames[STATIONS_SIZE] = {
  "Station 1",
  "Station 2",
  "Station 3",
  "Station 4",
  "Station 5",
  "Station 6",
};

String trainNames[TRAINS_SIZE] = {
  "Train 1",
  "Train 2",
  "Train 3",
};

// int currentServoPos = 45;
const int servoMinPosition = 45;
const int servoMaxPosition = 150;
// bool servoRaised = false;
// bool servoRaised[STATIONS_SIZE] = {
//   false, 
//   false, 
//   false, 
//   false, 
//   false, 
//   false, 
// };


void setup()
{
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output

  initLcds();
  initServos();


}

void loop()
{
  delay(100);
  // raiseServo(SERVOS[0]);
  // delay(100);
  // lowerServo(SERVOS[0]);
  // raiseServo(SERVOS[1]);
  // delay(100);
  // lowerServo(SERVOS[1]);
  //   Serial.print("INPUT RFID ON STATION (1-6): ");
  //   while (!Serial.available()) {
  //   }
  //   String input = Serial.readStringUntil('\n');
  //   input.trim();

  //   if (input == "1") {
  //     Serial.print(input);
  //   }
  //   else if (input == "2") {
  //     Serial.print(input);
  //   }
  //   else if (input == "3") {
  //     Serial.print(input);
  //   }
  //   else if (input == "4") {
  //     Serial.print(input);
  //   }
  //   else if (input == "5") {
  //     Serial.print(input);
  //   }
  //   else if (input == "6") {
  //     Serial.print(input);
  //   }
  //   else Serial.print("INVALID; ONLY 1 to 6");
  //   Serial.println();
  testServos();
}

void initServos() 
{
  for (int i = 0; i < STATIONS_SIZE; i++) 
  {
    SERVOS[i].attach(servosPins[i]);    
    SERVOS[i].write(45);
  }
}

// void initServos() 
// {
//   SERVOS[0].attach(servosPins[0]);    
//   SERVOS[0].write(45);

//   SERVOS[1].attach(servosPins[1]);    
//   SERVOS[1].write(45);

//   SERVOS[2].attach(servosPins[2]);    
//   SERVOS[2].write(45);

//   SERVOS[3].attach(servosPins[3]);    
//   SERVOS[3].write(45);
  
//   SERVOS[4].attach(servosPins[4]);    
//   SERVOS[4].write(45);

//   SERVOS[5].attach(servosPins[5]);    
//   SERVOS[5].write(45);
  
// }

void initLcds() 
{
  for (int i = 0; i < STATIONS_SIZE; i++) 
  {
    LCDS[i].init();
    LCDS[i].backlight();
    LCDS[i].print("Station ");
    LCDS[i].print(i + 1);
  }
}


// function that executes whenever data is received from master
// this function is registered as an event, see setup()
// void receiveEvent(int howMany)
// {
//   while(1 < Wire.available()) // loop through all but the last
//   {
//     char c = Wire.read(); // receive byte as a character
//     Serial.print(c);         // print the character
//   }
//   int x = Wire.read();    // receive byte as an integer
//   Serial.println(x);         // print the integer
// }

void receiveEvent(int howMany) 
{
  if (Wire.available() >= 2) 
  {  // Check if at least 1 byte is available
    char trainData = Wire.read();
    char stationData = Wire.read();

    char trainSizeStr = TRAINS_SIZE + '0';
    char stationSizeStr = STATIONS_SIZE + '0';
    bool isTrainDataValid = isDataValid(trainData, trainSizeStr);
    bool isStationDataValid = isDataValid(stationData, stationSizeStr);
    if (!isTrainDataValid || !isStationDataValid) 
    {
      return;
    }

    int trainNum = trainData - '0';
    int stationNum = stationData - '0';

    updateLCDToIncoming(stationNum - 1, trainNum - 1);
    openServo(stationNum - 1);
    updateLCDToWaiting(stationNum - 1);

  }
}


bool isDataValid(char chars, char max)
{
  Serial.print("RECEIVED ");  // Print the received integer
  Serial.print(chars);  // Print the received integer
  Serial.println(".");  // Print the received integer

  // int x = chars - '0';  // Convert the character to an integer (ASCII to int)
  if (chars >= '1' && chars <= max) 
  {
    // Serial.println(x);  // Print the received integer
    return true;
  } 

  else 
  {
    // Serial.print(x);  // Print the received integer
    Serial.println("Invalid input. Number must be within 1-3 or 1-6 range.");
    return false;
  }
}

void clearLCD(int lcdIndex) 
{
  LCDS[lcdIndex].clear();
  LCDS[lcdIndex].setCursor(0, 0);
}

void updateLCDToIncoming(int lcdIndex, int trainIndex)
{
  clearLCD(lcdIndex);
  String message = trainNames[trainIndex] + " is arriving at " + stationNames[lcdIndex];
  LCDS[lcdIndex].print(message);
}

void updateLCDToWaiting(int lcdIndex)
{
  clearLCD(lcdIndex);
  String message = stationNames[lcdIndex] + " Waiting...";
  LCDS[lcdIndex].print(message);
}


void raiseServo(Servo servo) 
{
  for (int pos = servoMinPosition; pos <= servoMaxPosition; pos += 1) 
  {
    servo.write(pos);
    delay(15);
  }
}


void lowerServo(Servo servo) 
{
  for (int pos = servoMaxPosition; pos >= servoMinPosition; pos -= 1) 
  {
    servo.write(pos);
    delay(15);
  }
}

void testServos() 
{
  for (int i = 0; i < STATIONS_SIZE; i++) 
  {
    Serial.print("RAISING SERVO ");
    Serial.println(i + 1);
    raiseServo(SERVOS[i]);
    delay(100);
    Serial.print("LOWERING SERVO ");
    Serial.println(i + 1);
    lowerServo(SERVOS[i]);
    delay(100);
  }
}

void openServo(int servoIndex) 
{
  delay(100); // IADJUST TO KUGN ILAN DELAY BAGO BUKSAN IISTART BUKSAN
  raiseServo(SERVOS[servoIndex]);
  delay(100); // IADJUST TO KUNG GANO KATAGAL NAKABUKAS
  lowerServo(SERVOS[servoIndex]);
}