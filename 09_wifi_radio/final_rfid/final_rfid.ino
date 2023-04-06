/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-rfid-nfc
 */

#include <SPI.h>
#include <MFRC522.h>

//#include "Constants.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

//ESP32-SOLO-S2
//SDA --> ESP32 5 GPIO5
//SCK --> ESP32 11 GPIO18
//MOSI --> 21 GPIO21 (altered)
//MISO --> 19 GPIO19
//IRQ attached to nothing
//GND --> GND
//RST --> 20 GPIO20 (altered)
//VCC --> 3.3V

const char* ssid = "MAKERSPACE";
const char* password = "12345678";

const String speedUrl = "https://ps70-api.vercel.app/speed";
const String directionUrl = "https://ps70-api.vercel.app/direction";

#define SS_PIN  5  //ESP32 pin GIOP5 
#define RST_PIN 27 //ESP32 pin GIOP20

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  
  Serial.begin(9600);
  // Connect to the wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi");
  }
  Serial.println("Connected to the WiFi network");
  
  SPI.begin(); // init SPI bus
  rfid.PCD_Init(); // init MFRC522

  Serial.println("Tap an RFID/NFC tag on the RFID-RC522 reader");
}

void loop() {
  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
      
      // print Tag Type
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      Serial.print("RFID/NFC Tag Type: ");
      Serial.println(rfid.PICC_GetTypeName(piccType));

      // print UID in Serial Monitor in the hex format
      Serial.print("UID:");
      String content = "";
      byte letter;
      for (byte i = 0; i < rfid.uid.size; i++) {
        Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(rfid.uid.uidByte[i], HEX);
        content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(rfid.uid.uidByte[i], HEX));
      }
      Serial.println();
      content.toUpperCase();

      // Forward Tag
      if (content.substring(1) == "A1 7C 8D 1D") {
        Serial.println("Forward Tag Recognized!");
        Serial.println();
        String direction = "forward";
        Serial.println("direction=" + direction + "&time=" + millis());
        if ((WiFi.status() == WL_CONNECTED)) {
          HTTPClient http;
          http.begin(directionUrl + "?direction=" + direction + "&time=" + millis());
          int httpResponseCode = http.POST("");
          String payload = http.getString();
          Serial.println(payload);
          Serial.println(httpResponseCode);
          Serial.println("POSTED :D");
          http.end();
        }
        else {
          Serial.println("Wifi not detected.");
        }
      }
      // Backwards Tag
      else if (content.substring(1) == "2A 2A 75 17") {
        Serial.println("Backward Tag Recognized!");
        Serial.println();
        String direction = "backward";
        Serial.println("direction=" + direction + "&time=" + millis());
        if ((WiFi.status() == WL_CONNECTED)) {
          HTTPClient http;
          http.begin(directionUrl + "?direction=" + direction + "&time=" + millis());
          int httpResponseCode = http.POST("");
          String payload = http.getString();
          Serial.println(payload);
          Serial.println(httpResponseCode);
          Serial.println("POSTED :D");
          http.end();
        }
        else {
          Serial.println("Wifi not detected.");
        }
      }

      // Left Tag
      else if (content.substring(1) == "37 23 27 7B") {
        Serial.println("Left Tag Recognized!");
        Serial.println();
        String direction = "left";
        Serial.println("direction=" + direction + "&time=" + millis());
        if ((WiFi.status() == WL_CONNECTED)) {
          HTTPClient http;
          http.begin(directionUrl + "?direction=" + direction + "&time=" + millis());
          int httpResponseCode = http.POST("");
          String payload = http.getString();
          Serial.println(payload);
          Serial.println(httpResponseCode);
          Serial.println("POSTED :D");
          http.end();
        }
        else {
          Serial.println("Wifi not detected.");
        }
      }

      // Right Tag
      else if (content.substring(1) == "F7 08 2A 7B") {
        Serial.println("Right Tag Recognized!");
        Serial.println();
        String direction = "right";
        Serial.println("direction=" + direction + "&time=" + millis());
        if ((WiFi.status() == WL_CONNECTED)) {
          HTTPClient http;
          http.begin(directionUrl + "?direction=" + direction + "&time=" + millis());
          int httpResponseCode = http.POST("");
          String payload = http.getString();
          Serial.println(payload);
          Serial.println(httpResponseCode);
          Serial.println("POSTED :D");
          http.end();
        }
        else {
          Serial.println("Wifi not detected.");
        }
      }

      else {
        Serial.println("Tag not recognized.");
      }

      rfid.PICC_HaltA(); // halt PICC
      rfid.PCD_StopCrypto1(); // stop encryption on PCD
    }
  }
}
