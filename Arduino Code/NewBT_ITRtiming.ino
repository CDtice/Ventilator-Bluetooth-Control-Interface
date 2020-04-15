/*
 * Ver. BT2.0 
 * 
 * Credits - please do not remove
 * 
 * Arduino RC Motor Ventilator with Android Bluetooth Control Interface - Craig Tice
 * 3D Printed RC Motor Fan Housing Design - Craig Tice, Peter Sucy, David Kanoy
 * 
 * Spawned from shortage of re-purposed CPAP/BiPAP Machines 
 * Re-purposed CPAP/BIPAP motor original concept - Johney Lee
 * 
 * Android app configured with with keuwlsoft Bluetooth Electronics configurator
 * https://www.keuwl.com/apps/bluetoothelectronics/
 *
*/

#include <SoftwareSerial.h>
SoftwareSerial BTserial(5, 4); // RX | TX
#include <Servo.h>
Servo myservo;  

char xferStr[10];
char CONTROLstate = 'R';
int BEEP = 1;
int HP_Value = 100;
int LP_Value = 20;
int HD_Value = 1500;
int LD_Value = 2400;

#define enable_motor true // useful for debugging without noise
#define max_speed 200
#define min_speed 0
#define PEEP_speed 40 //approx 5cm/H2O
#define servo_pin 3
#define led_pin 6

const byte ITR_led_Pin = 7;
const byte ITR_Pin = 2;
bool SYNC_itr = 0;

void setup(){

    Serial.begin(57600);
    BTserial.begin(57600);    //The factory default baud rate is 115200
    delay(100);
    Serial.println("Bluetooth Start!");
    delay(100);

    Serial.println("BiPAP RESET");

    pinMode(led_pin, OUTPUT);
    
    pinMode(ITR_led_Pin, OUTPUT);
    digitalWrite(ITR_led_Pin, LOW);
    pinMode(ITR_Pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(ITR_Pin), ITRexhale, FALLING);
    
    BTserial.print("*W");
    BTserial.print(HP_Value);
    BTserial.println("*");
    delay(100);
    BTserial.print("*X");
    BTserial.print(LP_Value);
    BTserial.println("*");
    delay(100);
    BTserial.print("*Y");
    BTserial.print(HD_Value);
    BTserial.println("*");
    delay(100);
    BTserial.print("*Z");
    BTserial.print(LD_Value);
    BTserial.println("*");
    delay(100);  
    
  if(enable_motor){
    myservo.attach(servo_pin,1000,2000); // some motors need min/max setting    
    Serial.print("Initializing ESC...");    
    digitalWrite(led_pin, HIGH);
    myservo.write(max_speed);
    delay(3000); // wait for esc to boot, and sample maximum
    myservo.write(min_speed);
    delay(2000);  // wait for esc to sample minimum
    digitalWrite(led_pin, LOW);
    myservo.write(PEEP_speed);
    Serial.println("Done");  
  } else {
    Serial.println("Motor disabled.  Bypassing initialization");      
  }   
}

  void loop(){
    
    int readBT = 0,i = 0,count = 0;
    String BTIn;
    if (BTserial.available()){
      while((readBT = BTserial.read()) != (int)-1){ //while there are characters to read...
        BTIn.concat(char(readBT));                  //append them to the String btIn
        delay(2); //to avoid hickups
      }
      Serial.println(BTIn);  //print the string to Arduino IDE Serial Monitor

    }

    if (BTIn.startsWith("O")){
      CONTROLstate = 'R';
      BEEP = 1;  
      Serial.println("BiPaP ON");
      BTIn = "";             //and clear the variable
    }
    
    if (BTIn.startsWith("F")){
      CONTROLstate = 'F';  
      Serial.println("BiPAP OFF");
      BTIn = "";             //and clear the variable
    }
    
    if (BTIn.startsWith("R")){
      CONTROLstate = 'R';
      BEEP = 1;  
      Serial.println("BiPAP RESET");
      BTIn = "";             //and clear the variable
    }
    
    if (BTIn.startsWith("B")){
      CONTROLstate = 'B';
      BEEP = 1;  
      Serial.println("Bi LEVEL MODE");
      BTIn = "";             //and clear the variable
    }
    
    if (BTIn.startsWith("C")){
      CONTROLstate = 'C';
      BEEP = 1;  
      Serial.println("CONTINUOUS MODE");
      BTIn = "";             //and clear the variable
    }
    
    if (BTIn.startsWith("S")){
      CONTROLstate = 'S';
      BEEP = 1;
      
      HP_Value = 80;
      LP_Value = 0;
      HD_Value = 680;
      LD_Value = 340;
      
      BTserial.print("*W");
      BTserial.print(HP_Value);
      BTserial.println("*");
      delay(100);
      BTserial.print("*X");
      BTserial.print(LP_Value);
      BTserial.println("*");
      delay(100);
      BTserial.print("*Y");
      BTserial.print(HD_Value);
      BTserial.println("*");
      delay(100);
      BTserial.print("*Z");
      BTserial.print(LD_Value);
      BTserial.println("*");
      delay(100);
      BTserial.println("*SV50*");
      delay(500);
      BTserial.println("*SV50*");
  
      Serial.println("VENTILATOR BREATH SYNC MODE");
      BTIn = "";             //and clear the variable
    }
   
    if (BTIn.startsWith("H")){
      BTIn.remove(0,1);
      BTIn.remove(BTIn.length()-1,1);
      Serial.print("Hi Pressure Value = ... ");
      BTIn.toCharArray(xferStr,BTIn.length()+1);
      HP_Value = atoi(xferStr);
      Serial.println(HP_Value);
      BTserial.print("*W");
      BTserial.print(HP_Value);
      BTserial.println("*");
      BTIn = "";             //and clear the variable
    }  
    if (BTIn.startsWith("L")){
      BTIn.remove(0,1);
      BTIn.remove(BTIn.length()-1,1);
      Serial.print("Low Pressure Value = ... ");
      BTIn.toCharArray(xferStr,BTIn.length()+1);
      LP_Value = atoi(xferStr);
      Serial.println(LP_Value);
      BTserial.print("*X");
      BTserial.print(LP_Value);
      BTserial.println("*");
      BTIn = "";             //and clear the variable
    }  
    if (BTIn.startsWith("D")){
      BTIn.remove(0,1);
      BTIn.remove(BTIn.length()-1,1);
      Serial.print("Hi Pressure Delay = ... ");
      BTIn.toCharArray(xferStr,BTIn.length()+1);
      HD_Value = atoi(xferStr);
      Serial.println(HD_Value);
      BTserial.print("*Y");
      BTserial.print(HD_Value);
      BTserial.println("*");
      BTIn = "";             //and clear the variable
    }  
    if (BTIn.startsWith("P")){
      BTIn.remove(0,1);
      BTIn.remove(BTIn.length()-1,1);
      Serial.print("Low Pressure Delay = ... ");
      BTIn.toCharArray(xferStr,BTIn.length()+1);
      LD_Value = atoi(xferStr);
      Serial.println(LD_Value);
      BTserial.print("*Z");
      BTserial.print(LD_Value);
      BTserial.println("*");
      BTIn = "";             //and clear the variable
    }

    if (CONTROLstate == 'B'){  

      if(BEEP == 1){;
        BTserial.println("*SV50*");
        delay(500);
        BTserial.println("*SV50*");
        delay(500);
        BEEP = 0;
      }
     
      if(enable_motor){
        myservo.write(HP_Value);
        digitalWrite(led_pin, HIGH);
        delay(HD_Value);
      }
      if(enable_motor){
        myservo.write(LP_Value);
        digitalWrite(led_pin, LOW);
        delay(LD_Value);
        digitalWrite(ITR_led_Pin, LOW);
      }
    }
    
   
   if (CONTROLstate == 'C'){
    myservo.write(HP_Value);
    digitalWrite(led_pin, HIGH);
    if(BEEP == 1){ 
    BTserial.println("*SV50*");
    delay(500);
    BTserial.println("*SV50*");
    delay(500);
    BTserial.println("*SV50*");
    delay(100);
    
    BEEP =0;
    }
   }
      
    if (CONTROLstate == 'R'){
      
     HP_Value = 100;
     LP_Value = 20;
     HD_Value = 1500;
     LD_Value = 2400;
      
     BTserial.print("*W");
     BTserial.print(HP_Value);
     BTserial.println("*");
     delay(100);
     BTserial.print("*X");
     BTserial.print(LP_Value);
     BTserial.println("*");
     delay(100);
     BTserial.print("*Y");
     BTserial.print(HD_Value);
     BTserial.println("*");
     delay(100);
     BTserial.print("*Z");
     BTserial.print(LD_Value);
     BTserial.println("*");
     delay(1000);
     BTserial.println("*SV50*");
     delay(1500);
     CONTROLstate = 'B';
   }
   
    if (CONTROLstate == 'F'){
               
      HP_Value = 0;
      LP_Value = 0;
      HD_Value = 0;
      LD_Value = 0;
      
    if(enable_motor){
      myservo.write(HP_Value);
      digitalWrite(led_pin, LOW);
      delay(HD_Value);
     }
    if(enable_motor){
      myservo.write(LP_Value);
      digitalWrite(led_pin, LOW);
      delay(LD_Value);
    }
      BTserial.print("*W");
      BTserial.print(HP_Value);
      BTserial.println("*");
      delay(100);
      BTserial.print("*X");
      BTserial.print(LP_Value);
      BTserial.println("*");
      delay(100);
      BTserial.print("*Y");
      BTserial.print(HD_Value);
      BTserial.println("*");
      delay(100);
      BTserial.print("*Z");
      BTserial.print(LD_Value);
      BTserial.println("*");
      delay(100);
      
      BEEP = 1;
    }
        
    if (CONTROLstate == 'S' && SYNC_itr == 1){              


/* Disable exhale interrupt so mask back pressure 
*   does not initiate a premature breath cycle
*/    
    detachInterrupt(digitalPinToInterrupt(ITR_Pin));
    Serial.println("Start ITR !!!");
    digitalWrite(ITR_led_Pin, HIGH);
    delay(LD_Value);

    if(enable_motor){
      myservo.write(HP_Value);
      delay(HD_Value);
    }
 /* Add low pressure value later */
    if(enable_motor){
      myservo.write(0);
    }
    delay(LD_Value);
    digitalWrite(ITR_led_Pin, LOW); 
    Serial.begin(57600);
    Serial.println("End ITR !!!");
 /* re-enble exhale interrupt */
     attachInterrupt(digitalPinToInterrupt(ITR_Pin), ITRexhale, FALLING);
     SYNC_itr = !SYNC_itr;;
    
   }
  }

        //  ***  Exhale Switch Assited Breathing on Exhale ITR*** //

  void ITRexhale(){
    for (int x = 0; x <= 20000; x++); // ITR pin debouce   {{{{{ 4/14/20
    SYNC_itr = 1;
  }
      
    
