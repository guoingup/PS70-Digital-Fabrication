/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-relay-module-ac-web-server/
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/
const int relay = 40;

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
 unsigned int sample;

 void setup() 
 {
    Serial.begin(9600);
    pinMode(relay, OUTPUT); //set relay pin as an OUTPUT
 }


 void loop() 
 {
    unsigned long startMillis= millis();  // Start of sample window
    unsigned int peakToPeak = 0;   // peak-to-peak level

    unsigned int signalMax = 0;
    unsigned int signalMin = 1024;

    // collect data for 50 mS
    while (millis() - startMillis < sampleWindow)
    {
       sample = analogRead(0);   //reading DC pin from pin A0
       if (sample < 1024)  // toss out spurious readings
       {
          if (sample > signalMax)
          {
             signalMax = sample;  // save just the max levels
          }
          else if (sample < signalMin)
          {
             signalMin = sample;  // save just the min levels
          }
       }
    }
    peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude

    Serial.println(peakToPeak);
    
    //If there's some noise, let the current flow.
    if (peakToPeak > 20) {
      digitalWrite(relay, LOW);
      Serial.println("Current Flowing");
      delay(5000); 
    }
    //If there's no loud noises, don't let the current flow.
    else {
      digitalWrite(relay, HIGH);
      Serial.println("Current not Flowing");
      delay(1000);
    }
 }
