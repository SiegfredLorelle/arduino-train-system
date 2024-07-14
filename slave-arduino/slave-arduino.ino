#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h> 

const int STATIONS_SIZE = 6;
const int TRAINS_SIZE = 3;

int IR1 = 8; 
int IR2 = 9; 
int IR3 = 10; 
int IR4 = 11; 
int IR5 = 12; 
int IR6 = 13; 
const int IRS[STATIONS_SIZE] = {
  A3,
  A2,
  A1,
  A0,
  7 ,
  12,
};


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
  "HQ 1",
  "HQ 2",
  "Goofball Island",
  "Hockey Island",
  "Honesty Island",
  "Family Island"
};

String trainNames[TRAINS_SIZE] = {
  "Lorelle",
  "Caskie",
  "Sebneth",
};

const int servoMinPosition = 45;
const int servoMaxPosition = 150;

bool servoRaised[STATIONS_SIZE] = {
  false, 
  false, 
  false, 
  false, 
  false, 
  false, 
};

int runningTrainIndex = 0;

int areStationsOccupied[STATIONS_SIZE] = {
  false,
  false,
  false,
  false,
  false,
  false,
};

int currentIrSignals[STATIONS_SIZE] = {
  false,
  false,
  false,
  false,
  false,
  false,
};



void setup()
{
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output

  initIrs();
  initLcds();
  initServos();


  findOccupiedStations();
}

void loop()
{
  delay(100);
  // testServos();

  // char trainData = promptForChar();
  // Serial.print("TRAIN ");
  // Serial.println(trainData);
  // char stationData = promptForChar();
  // Serial.print("STATION ");
  // Serial.println(stationData);
  // trainArriving(trainData, stationData);


  // int currSignal = digitalRead(A3);
  // if (currSignal == 0) 
  // {
  //   updateLCDToIncoming(0, runningTrainIndex);
  //   openServo(0);
  //   updateLCDToWaiting(0);
  // }
  readIrSignals();
  runTrains();

  
}

void initIrs() {
  for (int i = 0; i < STATIONS_SIZE; i++) {
    pinMode(IRS[i], INPUT);
  }
}

void initServos() 
{
  for (int i = 0; i < STATIONS_SIZE; i++) 
  {
    SERVOS[i].attach(servosPins[i]);    
    SERVOS[i].write(45);
  }
}

void initLcds() 
{
  for (int i = 0; i < STATIONS_SIZE; i++) 
  {
    LCDS[i].init();
    LCDS[i].backlight();
    LCDS[i].setCursor(0, 0);
    LCDS[i].print(stationNames[i]);
    LCDS[i].setCursor(0, 1);
    LCDS[i].print("Waiting...");
  }
}


// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  while(1 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}


void runTrains() 
{
  for (int i = 0; i < STATIONS_SIZE; i++) 
  {
    if (areStationsOccupied[i] != currentIrSignals[i]) 
    {
      trainArriving(runningTrainIndex, i);
    } 
  }
}

// void trainArriving(char trainData, char stationData)
// {
//  char trainSizeStr = TRAINS_SIZE + '0';
//     char stationSizeStr = STATIONS_SIZE + '0';
//     bool isTrainDataValid = isDataValid(trainData, trainSizeStr);
//     bool isStationDataValid = isDataValid(stationData, stationSizeStr);
//     if (!isTrainDataValid || !isStationDataValid) 
//     {
//       return;
//     }

//     int trainNum = trainData - '0';
//     int stationNum = stationData - '0';

//     updateLCDToIncoming(stationNum - 1, trainNum - 1);
//     openServo(stationNum - 1);
//     updateLCDToWaiting(stationNum - 1);
// }

void trainArriving(int trainIndex, int stationIndex)
{
    updateLCDToIncoming(stationIndex, trainIndex);
    openServo(stationIndex);
    updateLCDToWaiting(stationIndex);
}

void findOccupiedStations() {
  for (int i = 0; i < STATIONS_SIZE; i++) {
    int currentSignal = digitalRead(IRS[i]);
    areStationsOccupied[i] = currentSignal;
    currentIrSignals[i] = currentSignal;
  }
  printArray("STATIONS: ", areStationsOccupied, STATIONS_SIZE);
}


void readIrSignals() {
  for (int i = 0; i < STATIONS_SIZE; i++) {
    int currSignal = digitalRead(IRS[i]);
    currentIrSignals[i] = currSignal;
  }
    printArray("IR SIGNALS: ", currentIrSignals, STATIONS_SIZE);
    printArray("STATIONS : ", areStationsOccupied, STATIONS_SIZE);
}

void printArray(char* message, int arr[], int size) {
  Serial.print(message);
  for (int i = 0; i < size; i++) {
    Serial.print(arr[i]);
  }
  Serial.println();
}

bool isDataValid(char chars, char max)
{
  Serial.print("RECEIVED ");  
  Serial.print(chars);
  Serial.println(".");

  if (chars >= '1' && chars <= max) 
  {
    return true;
  } 

  else 
  {
    Serial.println("Invalid input. Number must be within 1-3 or 1-6 range.");
    return false;
  }
}

void updateLCDToIncoming(int lcdIndex, int trainIndex)
{
  LCDS[lcdIndex].clear();
  LCDS[lcdIndex].setCursor(0, 0);
  LCDS[lcdIndex].print(stationNames[lcdIndex]);

  String arrivingMessage = trainNames[trainIndex] + " arriving";
  LCDS[lcdIndex].setCursor(0, 1);
  LCDS[lcdIndex].print(arrivingMessage);
}

void updateLCDToWaiting(int lcdIndex)
{
  LCDS[lcdIndex].clear();
  LCDS[lcdIndex].setCursor(0, 0);
  LCDS[lcdIndex].print(stationNames[lcdIndex]);

  LCDS[lcdIndex].setCursor(0, 1);
  LCDS[lcdIndex].print("Waiting...");
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
    openServo(i);
  }
}  

void openServo(int servoIndex) 
{
  delay(100); // IADJUST TO KUGN ILAN DELAY BAGO BUKSAN IISTART BUKSAN
   if (servoRaised[servoIndex]) {
    return;
  } 
  servoRaised[servoIndex] = true; // Set lowered flag

  Serial.print("RAISING SERVO ");
  Serial.println(servoIndex + 1);
  raiseServo(SERVOS[servoIndex]);

  delay(100); // IADJUST TO KUNG GANO KATAGAL NAKABUKAS
  
  Serial.print("LOWERING SERVO ");
  Serial.println(servoIndex + 1);
  lowerServo(SERVOS[servoIndex]);

  servoRaised[servoIndex] = false; // Set lowered flag
}

char promptForChar() {
  char inputChar = '\0';
  bool numberEntered = false;
  
  // Prompt the user to enter a number
  Serial.println("Please enter a number:");
  
  while (!numberEntered) {
    // Check if data is available to read
    if (Serial.available() > 0) {
      // Read the incoming data as a string
      String input = Serial.readStringUntil('\n');

      if (input.length() > 0) 
      {
        inputChar = input.charAt(0);
        numberEntered = true;
      } 
      else 
      {
        Serial.println("Invalid input. Please enter a valid number:");
      }
    }
  }
  return inputChar;
}


// TODO
// FIND CONSECUTIVE OCCUPIED SPACE TO CHANGE CURRENT RUNNING TRAIN