#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h> 

const int STATIONS_SIZE = 6;

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;
Servo SERVOS[STATIONS_SIZE] {
  servo1,
  servo2,
  servo3,
  servo4,
  servo5,
  servo6,
};

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

int currentServoPos = 45;
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

  //   if (input = "1") {
  //     Serial.print(input);
  //   }
  //   else if (input = "2") {
  //     Serial.print(input);
  //   }
  //   else if (input = "3") {
  //     Serial.print(input);
  //   }
  //   else if (input = "4") {
  //     Serial.print(input);
  //   }
  //   else if (input = "5") {
  //     Serial.print(input);
  //   }
  //   else if (input = "6") {
  //     Serial.print(input);
  //   }
  //   else Serial.print("INVALID; ONLY 1 to 6");
  //   Serial.println();
  testServos();
}

void initServos() 
{
  for (int i; i < STATIONS_SIZE; i++) 
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
  
//   SERVOS[6].attach(servosPins[6]);    
//   SERVOS[6].write(45);
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
  if (Wire.available() >= 1) 
  {  // Check if at least 1 byte is available
    char receivedData = Wire.read();
    bool isValid = isReceivedDataValid(receivedData);
    // TODO
  }
}


bool isReceivedDataValid(char chars)
{
  Serial.print("RECEIVED ");  // Print the received integer
  // Serial.print(x);  // Print the received integer

  int x = chars - '0';  // Convert the character to an integer (ASCII to int)
  if (chars >= '1' && chars <= '6') 
  {
    // Serial.println(x);  // Print the received integer
    return true;
  } 

  else 
  {
    // Serial.print(x);  // Print the received integer
    Serial.println("Invalid input. Please enter a number between 1-6.");
    return false;
  }
}

void raiseServo(Servo servo) 
{
  for (int pos = currentServoPos; pos <= 150; pos += 1) 
  {
    servo.write(pos);
    delay(15);
  }
}



void lowerServo(Servo servo) 
{
  for (int pos = currentServoPos; pos >= 45; pos -= 1) 
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