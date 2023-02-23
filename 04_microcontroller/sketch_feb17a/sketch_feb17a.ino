class Spin
{
  // Class Member Variables
  // These are initialized at startup
  const int A1A;      
  const int A1B;
  long OnSpinInterval;
  long OffSpinInterval; 

  // These maintain the current state
  int motorState;                // ledState used to set the LED
  unsigned long previousMillis;   // will store last time LED was updated

  //Constructor - creates a Spinning Motor
  public:
  Spin(int pinA, int pinB, long ONinterval, long OFFinterval)
  {
    A1A = pinA;
    A1B = pinB;

    pinMode(A1A, OUTPUT);
    pinMode(A1B, OUTPUT);

    digitalWrite(A1A, LOW);
    digitalWrite(A1B, LOW);

    motorState = LOW;

    ONSpinInterval = ONinterval;
    OFFSpinInterval = OFFinterval;
    previousMillis = 0;
  }
  
  void Update()
  {
    // check to see if it's time to change the state of the LED
    unsigned long currentMillis = millis();
     
    if((motorState == LOW) && (currentMillis - previousMillis >= OffSpinInterval))
    {
      digitalWrite(A1A, HIGH);
      motorState = HIGH;
      previousMillis = currentMillis;
    }
    else if ((motorState == HIGH) && (currentMillis - previousMillis >= OnSpinInterval))
    {
      digitalWrite(A1A, LOW);
      motorState = LOW;
      previousMillis = currentMillis; // Remember the time
    }
  }
};

int pinA, int pinB, long ONinterval, long OFFinterval;

Spin motor1(3,4,1000,500);
Spin motor2(5,6,600,2000);

void setup() {
  Serial.begin(9600);
  pinMode(potPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  potValue = analogRead(potPin);
  Serial.println(potValue);
  delay(100);
  ledValue = map(potValue,0,1023,0,255);
  analogWrite(ledPin, ledValue);
  motor1.Update();
  motor2.Update();
}
