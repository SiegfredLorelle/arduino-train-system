#include <SPI.h>
#include <MFRC522.h>

// Define chip select (SS) pins for each reader
#define SS_PIN_READER1  3
#define SS_PIN_READER2  4
#define SS_PIN_READER3  5
#define SS_PIN_READER4  6
#define SS_PIN_READER5  7
#define SS_PIN_READER6  8
#define RST_PIN         9  // Common reset pin for all readers

// Create MFRC522 instances for each reader
MFRC522 mfrc522_reader1(SS_PIN_READER1, RST_PIN);
MFRC522 mfrc522_reader2(SS_PIN_READER2, RST_PIN);
MFRC522 mfrc522_reader3(SS_PIN_READER3, RST_PIN);
MFRC522 mfrc522_reader4(SS_PIN_READER4, RST_PIN);
MFRC522 mfrc522_reader5(SS_PIN_READER5, RST_PIN);
MFRC522 mfrc522_reader6(SS_PIN_READER6, RST_PIN);

void setup() {
  Serial.begin(9600);  // Initialize serial communications with the PC

  // Init SPI bus
  SPI.begin();

  // Initialize each MFRC522 reader
  mfrc522_reader1.PCD_Init();
  mfrc522_reader2.PCD_Init();
  mfrc522_reader3.PCD_Init();
  mfrc522_reader4.PCD_Init();
  mfrc522_reader5.PCD_Init();
  mfrc522_reader6.PCD_Init();

  delay(4);  // Optional delay for initialization

  // Show details of each reader using separate Serial.print statements
  mfrc522_reader1.PCD_DumpVersionToSerial();
  Serial.println(F("Reader 1: Scan PICC to see UID, SAK, type, and data blocks..."));

  mfrc522_reader2.PCD_DumpVersionToSerial();
  Serial.println(F("Reader 2: Scan PICC to see UID, SAK, type, and data blocks..."));

  mfrc522_reader3.PCD_DumpVersionToSerial();
  Serial.println(F("Reader 3: Scan PICC to see UID, SAK, type, and data blocks..."));

  mfrc522_reader4.PCD_DumpVersionToSerial();
  Serial.println(F("Reader 4: Scan PICC to see UID, SAK, type, and data blocks..."));

  mfrc522_reader5.PCD_DumpVersionToSerial();
  Serial.println(F("Reader 5: Scan PICC to see UID, SAK, type, and data blocks..."));

  mfrc522_reader6.PCD_DumpVersionToSerial();
  Serial.println(F("Reader 6: Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {
  // Check for new cards on each reader
  if (mfrc522_reader1.PICC_IsNewCardPresent()) {
    if (mfrc522_reader1.PICC_ReadCardSerial()) {
      mfrc522_reader1.PICC_DumpToSerial(&(mfrc522_reader1.uid));
      Serial.println(F("Card detected on Reader 1"));
    }
  }
  
  if (mfrc522_reader2.PICC_IsNewCardPresent()) {
    if (mfrc522_reader2.PICC_ReadCardSerial()) {
      mfrc522_reader2.PICC_DumpToSerial(&(mfrc522_reader2.uid));
      Serial.println(F("Card detected on Reader 2"));
    }
  }
  
  if (mfrc522_reader3.PICC_IsNewCardPresent()) {
    if (mfrc522_reader3.PICC_ReadCardSerial()) {
      mfrc522_reader3.PICC_DumpToSerial(&(mfrc522_reader3.uid));
      Serial.println(F("Card detected on Reader 3"));
    }
  }
  
  if (mfrc522_reader4.PICC_IsNewCardPresent()) {
    if (mfrc522_reader4.PICC_ReadCardSerial()) {
      mfrc522_reader4.PICC_DumpToSerial(&(mfrc522_reader4.uid));
      Serial.println(F("Card detected on Reader 4"));
    }
  }
  
  if (mfrc522_reader5.PICC_IsNewCardPresent()) {
    if (mfrc522_reader5.PICC_ReadCardSerial()) {
      mfrc522_reader5.PICC_DumpToSerial(&(mfrc522_reader5.uid));
      Serial.println(F("Card detected on Reader 5"));
    }
  }
  
  if (mfrc522_reader6.PICC_IsNewCardPresent()) {
    if (mfrc522_reader6.PICC_ReadCardSerial()) {
      mfrc522_reader6.PICC_DumpToSerial(&(mfrc522_reader6.uid));
      Serial.println(F("Card detected on Reader 6"));
    }
  }
  
  // Add a small delay if needed to avoid overwhelming the bus
  delay(100);
}