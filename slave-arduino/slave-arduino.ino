  // #include <Wire.h>
  #include <LiquidCrystal_I2C.h>
  #include <Servo.h> 

  const int STATIONS_SIZE = 6;
  const int TRAINS_SIZE = 3;

  const int IRS[STATIONS_SIZE] = {
    A3,
    A2,
    A1,
    7,
    8,
    A0,
  };

  Servo SERVOS[STATIONS_SIZE];

  int servosPins[STATIONS_SIZE] = {
    3,
    5,
    6,
    10,
    11,
    9,
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
    lcdStation5,
    lcdStation6,
    lcdStation4,
  };


  String stationNames[STATIONS_SIZE] = {
    "Family Island",
    "Hockey Island",
    "HQ 2",
    "Honesty Island",
    "HQ 1",
    "Goofball Island", 
  };

  String trainNames[TRAINS_SIZE] = {
    "Lorelle",
    "Caskie",
    "Sebneth",
  };

  const int servoMinPosition = 45;
  const int servoMaxPosition = 150;
  int servoDelay = 10;
  int trainStopTime = 2500;
  int trainLeaveDuration = 1500;  

  bool servoRaised[STATIONS_SIZE] = {
    false, 
    false, 
    false, 
    false, 
    false, 
    false, 
  };

  int runningTrainIndex = 1;

  bool areStationsOccupied[STATIONS_SIZE] = {
    false,
    false,
    false,
    false,
    false,
    false,
  };

  bool currentIrSignals[STATIONS_SIZE] = {
    false,
    false,
    false,
    false,
    false,
    false,
  };




  void setup()
  {
    // Wire.begin(4);                // join i2c bus with address #4
    // Wire.onReceive(receiveEvent); // register event
    Serial.begin(9600);           // start serial for output

    initIrs();
    initLcds();
    initServos();


    findOccupiedStations();
    readIrSignals();
    // test();
    // findFirstTrainToRun();
    // runFirstTrain();
    // trainArriving(runningTrainIndex, i);
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
    
    // runTrains();
    test();
    // String str = promptForBinaryString();

    
  }

  void initIrs() 
  {
    for (int i = 0; i < STATIONS_SIZE; i++) {
      pinMode(IRS[i], INPUT);
    }
  }

  void initServos() 
  {
    for (int i = 0; i < STATIONS_SIZE; i++) 
    {
      SERVOS[i].attach(servosPins[i]);    
      SERVOS[i].write(servoMaxPosition);
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


  // // function that executes whenever data is received from master
  // // this function is registered as an event, see setup()
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


void findFirstTrainToRun() {
  runningTrainIndex = 0;
  int i = 0;
  while (i < STATIONS_SIZE) 
  {
    int nextStationIndex = i + 1;
    if (nextStationIndex >= STATIONS_SIZE)
    {
      nextStationIndex = 0;
    }

    if (!areStationsOccupied[i] && areStationsOccupied[nextStationIndex]) 
    {
      trainArriving(runningTrainIndex, i);
      return;
    }
    if (!areStationsOccupied[i]) 
    {
      runningTrainIndex++;
    }
    i++;
  }
}


void runTrains() 
{
  for (int i = 0; i < STATIONS_SIZE; i++) 
  {
    
  if (areStationsOccupied[i] != currentIrSignals[i]) 
    {
      // areStationsOccupied[i] = currentIrSignals[i];
      // int nextStationIndex = i + 1;

      // if (nextStationIndex >= STATIONS_SIZE)
      // {
      //   nextStationIndex = 0;
      // }
      // if (areStationsOccupied[nextStationIndex] != currentIrSignals[nextStationIndex])
      // {
      //   areStationsOccupied[nextStationIndex] = currentIrSignals[nextStationIndex];
      //   int nextNextStationIndex = nextStationIndex + 1;
      //   updateRunningTrain(nextNextStationIndex);
      // }
      // else
      // {
      //   updateRunningTrain(nextStationIndex);
      // } 
      
      trainArriving(runningTrainIndex, i);
        // break;

      // }
    }
  }
}

  void trainArriving(int trainIndex, int stationIndex)
  {
      updateLCDToIncoming(stationIndex, trainIndex);
      openServo(stationIndex);
      updateLCDToWaiting(stationIndex);
  }

  void findOccupiedStations() 
  {
    for (int i = 0; i < STATIONS_SIZE; i++)
    {
      int currentSignal = digitalRead(IRS[i]);
      areStationsOccupied[i] = currentSignal;
      // currentIrSignals[i] = currentSignal;
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

  void printArray(char* message, bool arr[], int size) {
    Serial.print(message);
    for (int i = 0; i < size; i++) {
      Serial.print(arr[i]);
    }
    Serial.println();
  }

  // bool isDataValid(char chars, char max)
  // {
  //   Serial.print("RECEIVED ");  
  //   Serial.print(chars);
  //   Serial.println(".");

  //   if (chars >= '1' && chars <= max) 
  //   {
  //     return true;
  //   } 

  //   else 
  //   {
  //     Serial.println("Invalid input. Number must be within 1-3 or 1-6 range.");
  //     return false;
  //   }
  // }

  void updateLCDToIncoming(int lcdIndex, int trainIndex)
  {
    LCDS[lcdIndex].clear();
    LCDS[lcdIndex].setCursor(0, 0);
    LCDS[lcdIndex].print(stationNames[lcdIndex]);

    String arrivingMessage = trainNames[trainIndex] + " Arrived";
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
    for (int pos = servoMinPosition; pos <= servoMaxPosition; pos++) 
    {
      servo.write(pos);
      delay(servoDelay);
    }
  }


  void lowerServo(Servo servo) 
  {
    for (int pos = servoMaxPosition; pos >= servoMinPosition; pos--) 
    {
      servo.write(pos);
      delay(servoDelay);
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
    delay(trainStopTime);
    if (servoRaised[servoIndex]) {
      return;
    } 
    servoRaised[servoIndex] = true; // Set lowered flag


    Serial.print("LOWERING SERVO ");
    Serial.println(servoIndex + 1);
    // lowerServo(SERVOS[servoIndex]);
    SERVOS[servoIndex].write(servoMinPosition);

    delay(trainLeaveDuration);

    Serial.print("RAISING SERVO ");
    Serial.println(servoIndex + 1);
    // raiseServo(SERVOS[servoIndex]);
    SERVOS[servoIndex].write(servoMaxPosition);
    

    servoRaised[servoIndex] = false; // Set lowered flag
  }



void updateCurrentRunningTrain () {
  runningTrainIndex++;
  if  (runningTrainIndex >= TRAINS_SIZE) runningTrainIndex = 0;
}

void test() {


  for (int i = 0; i < STATIONS_SIZE; i += 2) {
    while (true) 
    {
      if (!digitalRead(IRS[i])) 
      {
        // trainArriving(i/2, i);
        int nextIndex = i;
        if (i + 1 >= STATIONS_SIZE)  nextIndex = 0;
        else nextIndex++; 
        updateCurrentRunningTrain();
        updateLCDToIncoming(nextIndex, runningTrainIndex);
        openServo(i);
        updateLCDToWaiting(i);
        break;
      }
    }
  }

  for (int i = 1; i < STATIONS_SIZE; i += 2) {
    while (true) 
    {
      if (!digitalRead(IRS[i]))
      {
        // trainArriving(i/2 - 1,  i);
        int nextIndex = i;
        if (i + 1 >= STATIONS_SIZE)  nextIndex = 0;
        else nextIndex++; 
        updateCurrentRunningTrain();
        updateLCDToIncoming(nextIndex, runningTrainIndex);
        openServo(i);
        updateLCDToWaiting(i);
        break;
      }
    }
  }
}


