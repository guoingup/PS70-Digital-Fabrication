void setup() {
    Serial.begin(9600);
    pinMode(4, OUTPUT);
    pinMode(2, INPUT);
    pinMode(6, INPUT); 
}

void loop() {
    long start = micros();

    for(int count=0; count<1000; count++){
        digitalWrite(4, HIGH);
        while(!digitalRead(2) && !digitalRead(6)){
            }

        digitalWrite(4, LOW);

        while(digitalRead(2) && digitalRead(6)){
            }   
    }

    Serial.println(micros() - start);

    delay(10);
}
