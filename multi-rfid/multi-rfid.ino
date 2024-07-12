#include <SPI.h>
#include <MFRC522.h>

// Define chip select (SS) pins for each reader
#define SS_PIN_READER1  9
#define SS_PIN_READER2 10
#define RST_PIN 8

// Create MFRC522 instances for each reader
MFRC522 mfrc522_reader1(SS_PIN_READER1, RST_PIN);  // Reader 1
MFRC522 mfrc522_reader2(SS_PIN_READER2, RST_PIN);  // Reader 2

void setup() {
  Serial.begin(9600);  // Initialize serial communications with the PC

  // Init SPI bus
  SPI.begin();

  // Initialize each MFRC522 reader
  mfrc522_reader1.PCD_Init();
  mfrc522_reader2.PCD_Init();

  delay(4);  // Optional delay for initialization

  // Show details of each reader using separate Serial.print statements
  mfrc522_reader1.PCD_DumpVersionToSerial();
  Serial.println(F("Reader 1: Scan PICC to see UID, SAK, type, and data blocks..."));
  mfrc522_reader2.PCD_DumpVersionToSerial();
  Serial.println(F("Reader 2: Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {
  // Check for new cards on each reader
  if (mfrc522_reader1.PICC_IsNewCardPresent()) {
    // Handle card on reader 1 (e.g., read data, display info)
    if (mfrc522_reader1.PICC_ReadCardSerial()) {
      mfrc522_reader1.PICC_DumpToSerial(&(mfrc522_reader1.uid));
      Serial.println(F("Card detected on Reader 1"));
    }
  } else if (mfrc522_reader2.PICC_IsNewCardPresent()) {
    // Handle card on reader 2 (e.g., read data, display info)
    if (mfrc522_reader2.PICC_ReadCardSerial()) {
      mfrc522_reader2.PICC_DumpToSerial(&(mfrc522_reader2.uid));
      Serial.println(F("Card detected on Reader 2"));
    }
  }
  // Add a small delay if needed to avoid overwhelming the bus
  // delay(100);
}