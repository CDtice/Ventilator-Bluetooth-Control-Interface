#include <Blynk.h>

/* ESP-01 with Micro Fan Simple Timed Breath Cycle
  

// Fixed Breath Cycle
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);  // Hooked to micro fan
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the Fan on (HIGH is the voltage level)
  delay(2000);                       // wait for 2 second inhale
  digitalWrite(LED_BUILTIN, LOW);    // turn the Fan off by making the voltage LOW
  delay(2000);                       // wait for 2 second exhale
}
