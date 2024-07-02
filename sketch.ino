#include <LiquidCrystal_I2C.h> 

LiquidCrystal_I2C lcdStation1(0x27, 16, 2);
LiquidCrystal_I2C lcdStation2(0x26, 16, 2);
LiquidCrystal_I2C lcdStation3(0x25, 16, 2);
LiquidCrystal_I2C lcdStation4(0x24, 16, 2);
LiquidCrystal_I2C lcdStation5(0x23, 16, 2);
LiquidCrystal_I2C lcdStation6(0x22, 16, 2);

const int LCDS_SIZE = 6;
LiquidCrystal_I2C LCDS[LCDS_SIZE] = {
  lcdStation1,
  lcdStation2,
  lcdStation3,
  lcdStation4,
  lcdStation5,
  lcdStation6,
};

int IR1 = 8; 
int IR2 = 9; 
int IR3 = 10; 
int IR4 = 11; 
int IR5 = 12; 
int IR6 = 13; 
const int IRS_SIZE = 6;
const int IRS[IRS_SIZE] = {
  IR1,
  IR2,
  IR3,
  IR4,
  IR5,
  IR6,
};

int motor1 = 5;
int motor2 = 6;
int motor3 = 7;
const int MOTORS_SIZE = 3;
const int MOTORS[MOTORS_SIZE] = {
  motor1,
  motor2,
  motor3,
};


void setup()
{
  Serial.begin(9600);
  initLcds();
  initIrs();
}


void loop()
{
  // readIrSignals();
  testMotors();

  delay(10);
}

void initLcds() {
  for (int i = 0; i < LCDS_SIZE; i++) {
    LCDS[i].init();
    LCDS[i].backlight();
    LCDS[i].print("Station ");
    LCDS[i].print(i + 1);
  }
}

void initIrs() {
  for (int i = 0; i < IRS_SIZE; i++) {
    pinMode(IRS[i], INPUT);
  }
}

void initMotors() {
  for (int i = 0; i < MOTORS_SIZE; i++) {
    pinMode(MOTORS[i], OUTPUT);
  }
}

void readIrSignals() {
  Serial.print("IR Signals: ");
    for (int i = 0; i < IRS_SIZE; i++) {
      int currSignal = digitalRead(IRS[i]);
      Serial.print(currSignal);
    }
  Serial.println();
}

void testMotors() {
  for (int i = 0; i < MOTORS_SIZE; i++) {
    digitalWrite(MOTORS[i], HIGH);
    delay(5000);
    digitalWrite(MOTORS[i], LOW);
    delay(1000);
  }
}