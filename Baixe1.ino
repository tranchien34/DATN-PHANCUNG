#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN1 9
#define SS_PIN1 10

MFRC522 mfrc522_1(SS_PIN1, RST_PIN1);

void printUID(MFRC522 &mfrc522)
{
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
}

void uidToString(MFRC522 &mfrc522, String &uidString)
{
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    if (i > 0) {
      uidString += " "; // Thêm khoảng trắng sau mỗi cặp số hex
    }
    String hexByte = String(mfrc522.uid.uidByte[i], HEX);
    hexByte.toUpperCase(); // Chuyển đổi thành chữ in hoa
    uidString += hexByte;
  }
}


void sendUIDViaI2C(const String &uidString)
{
  Wire.beginTransmission(8); // Bắt đầu truyền với địa chỉ 8
  Wire.write(uidString.c_str()); // Gửi chuỗi UID
  Wire.endTransmission(); // Dừng truyền
}

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  Wire.begin();

  mfrc522_1.PCD_Init();

  Serial.println("Chuẩn bị quét thẻ RFID...");
}

void loop()
{
  if (mfrc522_1.PICC_IsNewCardPresent() && mfrc522_1.PICC_ReadCardSerial())
  {
    Serial.print("Module 2 đọc thẻ UID: ");
    printUID(mfrc522_1);

    String uidString1;
    uidToString(mfrc522_1, uidString1);
    sendUIDViaI2C(uidString1);

    mfrc522_1.PICC_HaltA();
    mfrc522_1.PCD_StopCrypto1();
  }
  delay(50);
}