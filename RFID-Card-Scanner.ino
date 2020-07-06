#include <MFRC522.h>
#include <SPI.h>
MFRC522 mfrc522(10, 9);
byte master[10];

void setup() {
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  Serial.begin(9600);
  while(!Serial);
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  newMaster();
}

void loop() {
  if (digitalRead(2) == HIGH) newMaster();
  
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) return;

  access(cmpArray(master, mfrc522.uid.uidByte, 10));
  Serial.println("Bring card to Scanner.\n");
  mfrc522.PICC_HaltA();
}

void printByteArray(byte* arr, int sz) {
  for (int i = 0; i < sz; ++i) {
    Serial.print(arr[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

bool cmpArray(byte* arr1, byte* arr2, int sz) {
  for (int i = 0; i < sz; ++i)
    if (arr1[i] != arr2[i])
      return false;
  return true;
}

void newMaster() {
  Serial.println("Bring master card to Scanner.\n");
  while (!mfrc522.PICC_IsNewCardPresent());
  while (!mfrc522.PICC_ReadCardSerial());
  memcpy(master, mfrc522.uid.uidByte, sizeof(mfrc522.uid.uidByte));
  mfrc522.PICC_HaltA();
  Serial.println("Master card has been saved");
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  delay(50);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  delay(50);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  delay(50);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  Serial.println("Bring card to Scanner.\n");
}

void access(bool stat) {
  if (stat) {
    Serial.println("Access Permitted");
    digitalWrite(3, HIGH);
    delay(1000);
    digitalWrite(3, LOW);
  }
  else {
    Serial.println("Access Denied");
    digitalWrite(4, HIGH);
    delay(1000);
    digitalWrite(4, LOW);
  }
}
