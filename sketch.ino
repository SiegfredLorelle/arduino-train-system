#include <LCD_I2C.h> 

LCD_I2C lcdStation1(0x27, 16, 2);
LCD_I2C lcdStation2(0x26, 16, 2);
LCD_I2C lcdStation3(0x25, 16, 2);
LCD_I2C lcdStation4(0x24, 16, 2);
LCD_I2C lcdStation5(0x23, 16, 2);
LCD_I2C lcdStation6(0x22, 16, 2);

const int LCDS_SIZE = 6;
const LCD_I2C LCDS[LCDS_SIZE] = {
  lcdStation1,
  lcdStation2,
  lcdStation3,
  lcdStation4,
  lcdStation5,
  lcdStation6,
};



void setup()
{
  Serial.begin(9600);
  initLcds();
}
 
 
void loop()
{

}

void initLcds() {
  for (int i = 0; i < LCDS_SIZE; i++) {
    LCDS[i].begin();
    LCDS[i].backlight();
    LCDS[i].print("Station ");
    LCDS[i].print(i + 1);
  }
}