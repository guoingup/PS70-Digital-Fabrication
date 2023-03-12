
long result;   //variable for the result of the tx_rx measurement.
int analog_pin = A3;
int tx_pin = 4;

void setup() {
    pinMode(tx_pin, OUTPUT);      //Pin 4 provides the voltage step
    Serial.begin(9600);
}

void loop() {
    result = tx_rx();
    Serial.println(result);
}


long tx_rx(){         // Function to execute rx_tx algorithm and return a value
                      // that depends on coupling of two electrodes.
                      // Value returned is a long integer.
  int read_high;
  int read_low;
  int diff;
  long int sum;
  int N_samples = 100;    // Number of samples to take.  Larger number slows it down, but reduces scatter.

  sum = 0;

  for (int i = 0; i < N_samples; i++){
   digitalWrite(tx_pin,HIGH);              // Step the voltage high on conductor 1.
   read_high = analogRead(analog_pin);     // Measure response of conductor 2.
   delayMicroseconds(100);                 // Delay to reach steady state.
   digitalWrite(tx_pin,LOW);               // Step the voltage to zero on conductor 1.
   read_low = analogRead(analog_pin);      // Measure response of conductor 2.
   diff = read_high - read_low;            // desired answer is the difference between high and low.
   sum += diff;                            // Sums up N_samples of these measurements.
 }
  return sum;
}
