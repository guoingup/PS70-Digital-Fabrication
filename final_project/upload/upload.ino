
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
const int n_shotglasses = 5;
const int n_parameters = 8;
//1 (ID) 2 (Name) 3 (Date acquired) 4(Location acquired) 5(Gift?) 6(Fun extra notes)
String shots[ n_shotglasses ][ n_parameters ] = {
{"D7 F2 A1 75", "Luca's Hometown", "January 6th, 2023", "Basel, Switzerland","This shotglass represents winter break of 2022-2023, one of my favorite breaks of all time which I spent with my best friend Maddie, Luca, Stephanie, and Izzy from UCambridge! This shotglass came specifically from a shop in Basel, Switzerland, the hometown of my friend Luca where we stayed at his home for a night before heading to the Swiss alps for some snowboarding. Earlier on the trip we visited Madrid and London, too.","Yes","No","Yes"},
{"D0 91 35 40", "Yellowstone", "Summer of 2013", "Yellowstone National Park","I got this shotglass and my Jackson Hole shotglass during a family vacation to Yellowstone National Park. I loved that trip. We drove through maybe 4 states? Including straight across Montana! So beautiful and so nice to see a different side of the states.","Yes","Yes","No"},
{"57 C0 92 75", "Heart Art", "January 26th, 2022", "Dora's Place in Leverett House, Cambridge, MA","My good friend Dora gave me this shotglass (and the New York one!) when I came to visit her in Lev Tower after winter break in sophomore year.","No","Yes","No"},
{"37 FA A1 75", "Texas", "November 2017", "Dallas, TX","I got this shotglass in Dallas during the 2017 SPJ Student Journalism Convention back when I was a sophomore in high school! I went with all the editors of my high school newspaper The Spoke, of which I would become editor-in-chief in senior year.","No","No","No"},
{"C7 E0 A0 75", "Wonder Woman", "July 16, 2022", "Six Flags New England","For me, this shotglass represents the summer of 2022, one of the best summers of my life. That's where I met my best friend Maddie and all the PRIMOs in my life.","Yes","No","No"}
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
  tft.setRotation(3); //Set orientation of screen
  tft.setTextColor(TFT_WHITE, TFT_BLACK, true); // Set the font colour AND the background colour
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
      rfid.PICC_HaltA(); // halt PICC
      rfid.PCD_StopCrypto1(); // stop encryption on PCD
    }
  }

  tft.fillScreen(TFT_BLACK);
  
  /*DISPLAY*/
  if (screen == 1) {
    if (shotID == "") {
      tft.fillScreen(TFT_BLACK);
      tft.setCursor(0, 0); // Set cursor at top left of screen
      tft.loadFont(AA_FONT_LARGE);    // Must load the font first
      
      tft.setTextColor(TFT_CYAN, TFT_BLACK, true); // Set the font colour AND the background colour
      tft.println("Thank you for choosing SNAP SHOTS!");
      tft.println("Please place a shotglass on the circle to get started.");
      tft.unloadFont(); // Remove the font to recover memory used
    }
    else {
//      tft.fillScreen(TFT_BLACK);
      tft.setCursor(0, 0); // Set cursor at top left of screen
      tft.loadFont(AA_FONT_SMALL);    // Must load the font first
      tft.setTextColor(TFT_CYAN, TFT_BLACK, true);
      
      tft.println("ABOUT THIS SHOTGLASS: ");
      tft.println("");
      tft.print("ID Number: ");
      tft.println(shots[shotrowind][0]);
      tft.print("Name: ");
      tft.println(shots[shotrowind][1]);
      tft.print("Date Acquired: ");
      tft.println(shots[shotrowind][2]);
      tft.print("Location Acquired: ");
      tft.println(shots[shotrowind][3]);
      tft.println("");
      
      tft.print("About: ");
      tft.println(shots[shotrowind][4]);
      tft.unloadFont(); // Remove the font to recover memory used
//    delay(1000);
    }
  }
  else if (screen==2) {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0); // Set cursor at top left of screen
    
    tft.loadFont(AA_FONT_LARGE);    // Must load the font first
    tft.setTextColor(TFT_WHITE, TFT_BLACK, true); // Set the font colour AND the background colour

    tft.setTextWrap(true); // Wrap on width
    tft.setTextColor(TFT_CYAN, TFT_BLACK, true);
    tft.println("LED KEY:");
    tft.println("");
    tft.println("Greens = A Gift");
    tft.println("Purples = From Outside America");
    tft.println("Rainbow = A Favorite Shotglass :D");
    tft.unloadFont(); // Remove the font to recover memory used
  }
  else {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0); // Set cursor at top left of screen

    tft.loadFont(AA_FONT_LARGE);    // Must load the font first
    
    tft.setTextColor(TFT_PURPLE, TFT_BLACK, true); // Set the font colour AND the background colour
    tft.setTextWrap(true); // Wrap on width
    tft.setTextColor(TFT_BLUE, TFT_BLACK, true);
//    tft.println("Get this -- Screen 3!!!");
    tft.println("You can find my website at https://guoingup.github.io/PS70-Digital-Fabrication/");
  }

}
