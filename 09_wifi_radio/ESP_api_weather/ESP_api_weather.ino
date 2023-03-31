#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "MAKERSPACE";
const char* password =  "12345678";

const String endpoint = "https://api.weather.gov/gridpoints/TOP/31,80/forecast";

void setup() {

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");

}

void loop() {

  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status

    HTTPClient http;

    http.begin(endpoint); //Specify the URL
    int httpCode = http.GET();  //Make the request

    if (httpCode > 0) { //Check for the returning code

        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }

    else {
      Serial.println("Error on HTTP request");
    }

    http.end(); //Free the resources
  }

  delay(30000);

}
