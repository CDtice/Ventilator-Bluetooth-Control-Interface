/*  Simple H-Bridge CPU an Based CPAP with dynamic breaking*/

#define enA 9
#define in1 6
#define in2 7
#define button 4
int rotDirection = 0;
//int pressed = false;
void setup() {
  Serial.begin(9600);
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(button, INPUT);
  // Set initial rotation direction
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}
void loop() {
  int potValue = analogRead(A0); // Read potentiometer value
  int pwmOutput = map(potValue, 0, 1023, 0 , 255); // Map the potentiometer value from 0 to 255
//  analogWrite(enA, pwmOutput); // Send PWM signal to L298N Enable pin
  analogWrite(enA, 255); // Send PWM signal to L298N Enable pin
  Serial.println(pwmOutput);
//  Read button - Debounce
//  if (digitalRead(button) == true) {
//  pressed = !pressed;}

    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    rotDirection = 1;
    delay(2000);

//  *** Dynamic break by reversing the motor
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    Serial.println(pwmOutput);
    delay(2000);
    

}
