
/*Screen Number
1 = Main screen 
2 = LED KEY
3 = WEBSITE LINK or QR Code if time
*/

int screen;
String shotID;
int shotrowind;

///////////////////////////////////////////
/*BUTTON STUFF*/
#include <ezButton.h>
#define DEBOUNCE_TIME 50 // the debounce time in millisecond, increase this time if it still chatters
ezButton upbutton(22); // create ezButton object that attach to pin GIOP21
ezButton downbutton(21); // create ezButton object that attach to pin GIOP21


///////////////////////////////////////////
/*RFID STUFF*/
#include <SPI.h>
#include <MFRC522.h>

//#include "Constants.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define SS_PIN  5  //ESP32 pin GIOP5 
#define RST_PIN 27 //ESP32 pin GIOP20

MFRC522 rfid(SS_PIN, RST_PIN);


//PIN SETUP:
//ESP32-Mini
//SDA --> ESP32 5 GPIO5
//SCK --> ESP32 11 GPIO18
//MOSI --> 23 GPIO23
//MISO --> 19 GPIO19
//IRQ attached to nothing
//GND --> GND
//RST --> 27 GPIO27
//VCC --> 3.3V

//WIFI PASSWORD STUFF
const char* ssid = "MAKERSPACE";
const char* password = "12345678";


///////////////////////////////////////////
/*NEOPIXEL STUFF*/
#include <Adafruit_NeoPixel.h>

#define LED_PIN   38   //not 9 The pin connected to the NeoPixels.
#define LED_COUNT 20  //The number of NeoPixels attached.
#define DELAY_VAL 100


//Declare our NeoPixel strip object:
//Arg 1: # of pixels; Arg 2: Arduino pin #; Arg 3: pixel type flags, add together as needed

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


///////////////////////////////////////////
/*FONT STUFF*/
//  A processing sketch to create new fonts can be found in the Tools folder of TFT_eSPI
//  https://github.com/Bodmer/TFT_eSPI/tree/master/Tools/Create_Smooth_Font/Create_font

//  This sketch uses font files created from the Noto family of fonts:
//  https://www.google.com/get/noto/

#include "NotoSansBold15.h"
#include "NotoSansBold36.h"

// The font names are arrays references, thus must NOT be in quotes ""
#define AA_FONT_SMALL NotoSansBold15
#define AA_FONT_LARGE NotoSansBold36

#include <SPI.h>
#include <TFT_eSPI.h>       // Hardware-specific library

TFT_eSPI tft = TFT_eSPI();


//CREATE ARRAY
const int n_shotglasses = 3;
const int n_parameters = 6;
//1 (ID) 2 (Name) 3 (Date acquired) 4(Location acquired) 5(Gift?) 6(Fun extra notes)
String shots[ n_shotglasses ][ n_parameters ] = {
{"57 C0 92 75", "Family shotglass", "June 6, 2020", "Wuhan, China", "No", "Hello! This is a fun note."},
{"A2 7C 8D 1D", "Shotglass 2", "June 7, 2020", "Shanghai, China", "No", "Hello! This is a fun note."} 
};



///////////////////////////////////////////
void setup(void) {
  //Initialize important variables
  screen = 1;
  shotID = "";
  shotrowind = 0;

  //Begin Serial Monitor
  Serial.begin(9600);
  
  //BUTTON
  downbutton.setDebounceTime(DEBOUNCE_TIME); // set debounce time to 50 milliseconds
  upbutton.setDebounceTime(DEBOUNCE_TIME);

  //RFID
  // Connect to the wifi
//  WiFi.begin(ssid, password);
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(1000);
//    Serial.println("Connecting to WiFi");
//  }
//  Serial.println("Connected to the WiFi network");
  
  SPI.begin(); // init SPI bus
  rfid.PCD_Init(); // init MFRC522

  Serial.println("Tap an RFID/NFC tag on the RFID-RC522 reader");

  //NEOPIXEL
//  strip.begin();
//  strip.show(); //Initialize all pixels to off.
//  strip.setBrightness(12);

  
  //SCREEN
  tft.begin();
  tft.setRotation(1); //Set orientation of screen
  tft.setTextColor(TFT_WHITE, TFT_BLACK); // Set the font colour AND the background colour
  tft.setCursor(0, 0); // Set cursor at top left of screen
  tft.setTextWrap(true); // Wrap on width
}


void loop() {
  //NEOPIXEL

//  for(int i=0; i<LED_COUNT; i++) {
//    strip.rainbow(65535-(i*6553),1,255,255,true);
//    strip.show();
//    delay(100);
//  }
  

 /*CHECK BUTTONS*/
 /* DOWN BUTTON*/
  downbutton.loop(); // MUST call the loop() function first
  upbutton.loop();
  
  if (downbutton.isPressed()) {
    tft.fillScreen(TFT_BLACK);
    Serial.println("The down button is pressed");
  }
  
  if (downbutton.isReleased()) {
    Serial.println("The down button is released");
    Serial.print("Screen number was: ");
    Serial.println(screen);
    if (screen == 3) {
      screen = 1;
    }
    else {
      screen++;
    }
    Serial.println("Screen number is now: ");
    Serial.println(screen);
  }

  if (upbutton.isPressed()) {
    tft.fillScreen(TFT_BLACK);
    Serial.println("The up button is pressed");
  }
  
  if (upbutton.isReleased()) {
    Serial.println("The up button is released");
    Serial.print("Screen number was: ");
    Serial.println(screen);
    if (screen == 1) {
      screen = 3;
    }
    else {
      screen--;
    }
    Serial.println("Screen number is now: ");
    Serial.println(screen);
  }
  
//  button.loop(); // MUST call the loop() function first
//  if (button.isPressed()) {
//    tft.fillScreen(TFT_BLACK);
//    Serial.println("The button is pressed");
//    Serial.println("Screen number was: ");
//    Serial.print(screen);
//    if (screen == 3) {
//      screen = 1;
//    }
//    else {
//      screen++;
//    }
//    Serial.println("Screen number is now: ");
//    Serial.println(screen);
//  }
//  
//  if (button.isReleased()) {
//    Serial.println("The button is released");
//  }


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
      Serial.println(content.substring(1));
      
      shotID = content.substring(1);
      
      //Iterate through the array to find the right shotglass
      for (int i = 0; i < n_shotglasses; i++) {
        if (shots[i][0] == shotID) {
          shotrowind = i; //Set shotglass ID to the right one in the array
        }
      }

      if (screen == 1) {
    tft.loadFont(AA_FONT_SMALL);    // Must load the font first
    if (shotID == "") {
      tft.println("Thank you for choosing SNAP SHOTS!");
      tft.println("Please place a shotglass on the circle to get started.");
      tft.unloadFont(); // Remove the font to recover memory used
    }
    else {
      tft.println("Long lines wrap to the next line");
      tft.print("ID Number: ");
      tft.println(shots[shotrowind][0]);
      tft.print("Name: ");
      tft.println(shots[shotrowind][1]);
      tft.print("Date Acquired: ");
      tft.println(shots[shotrowind][2]);
      tft.print("Location Acquired: ");
      tft.println(shots[shotrowind][3]);
      tft.setTextWrap(false, false); // Wrap on width and height switched off
      tft.setTextColor(TFT_MAGENTA, TFT_BLACK);
      tft.unloadFont(); // Remove the font to recover memory used
//    delay(1000);
    }
  }
  else if (screen==2) {
    tft.loadFont(AA_FONT_LARGE);    // Must load the font first
    tft.setTextColor(TFT_WHITE, TFT_BLACK); // Set the font colour AND the background colour
    tft.setCursor(0, 0); // Set cursor at top left of screen
    tft.setTextWrap(true); // Wrap on width
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.println("LED KEY:");
    tft.println("Greens = A Gift");
    tft.println("Purples = From Outside America");
    tft.println("Rainbow = A Favorite Shotglass :D");
    tft.unloadFont(); // Remove the font to recover memory used
  }
  else {
    tft.setTextColor(TFT_WHITE, TFT_BLACK); // Set the font colour AND the background colour
    tft.setCursor(0, 0); // Set cursor at top left of screen
    tft.setTextWrap(true); // Wrap on width
    tft.setTextColor(TFT_BLUE, TFT_BLACK);
    tft.println("Get this -- Screen 3!!!");
    tft.println("You can find my website at this QR Code:");
  }
  
    }
  }
` else {
      tft.loadFont(AA_FONT_SMALL);    // Must load the font first
      tft.println("Thank you for choosing SNAP SHOTS!");
      tft.println("Please place a shotglass on the circle to get started.");
      tft.unloadFont(); // Remove the font to recover memory used
  }
  
  

  /*DISPLAY*/



  
  if (screen == 1) {
    tft.loadFont(AA_FONT_SMALL);    // Must load the font first
    if (shotID == "") {
      tft.println("Thank you for choosing SNAP SHOTS!");
      tft.println("Please place a shotglass on the circle to get started.");
      tft.unloadFont(); // Remove the font to recover memory used
    }
    else {
      tft.println("Long lines wrap to the next line");
      tft.print("ID Number: ");
      tft.println(shots[shotrowind][0]);
      tft.print("Name: ");
      tft.println(shots[shotrowind][1]);
      tft.print("Date Acquired: ");
      tft.println(shots[shotrowind][2]);
      tft.print("Location Acquired: ");
      tft.println(shots[shotrowind][3]);
      tft.setTextWrap(false, false); // Wrap on width and height switched off
      tft.setTextColor(TFT_MAGENTA, TFT_BLACK);
      tft.unloadFont(); // Remove the font to recover memory used
//    delay(1000);
    }
  }
  else if (screen==2) {
    tft.loadFont(AA_FONT_LARGE);    // Must load the font first
    tft.setTextColor(TFT_WHITE, TFT_BLACK); // Set the font colour AND the background colour
    tft.setCursor(0, 0); // Set cursor at top left of screen
    tft.setTextWrap(true); // Wrap on width
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.println("LED KEY:");
    tft.println("Greens = A Gift");
    tft.println("Purples = From Outside America");
    tft.println("Rainbow = A Favorite Shotglass :D");
    tft.unloadFont(); // Remove the font to recover memory used
  }
  else {
    tft.setTextColor(TFT_WHITE, TFT_BLACK); // Set the font colour AND the background colour
    tft.setCursor(0, 0); // Set cursor at top left of screen
    tft.setTextWrap(true); // Wrap on width
    tft.setTextColor(TFT_BLUE, TFT_BLACK);
    tft.println("Get this -- Screen 3!!!");
    tft.println("You can find my website at this QR Code:");
  }

}
