#include <Wire.h>

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
}

void loop()
{
  const char *message = "Kennesu idol"; // Define the message to send

  Wire.beginTransmission(4); // transmit to device #4

  // Send each character of the message
  for (int i = 0; message[i] != '\0'; i++) {
    Wire.write(message[i]);
  }

  Wire.endTransmission(); // stop transmitting

  delay(500);
}
