/* 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
March 27th: control code working with Processing - motor applies counter force in one axis
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Arduino Pin       HCTL-2022 function
------------------------------------
DIG 9             /RST (active low)
DIG 11            SEL1
DIG 12            SEL2
DIG 13            /OE (active low)
DIG 22->29        Data pins D0->D7
+5V               VDD
GND               VSS
from encoder      CHA
from encoder      CHB
not connected     INDEX
not connected     U/D
not connected     TEST 
External clock    CLK
*/
char val; // Data received from the serial port
int ledPin = 13; // Set the pin to digital I/O 13
boolean ledState = LOW; //to toggle our LED
int state = 0;

int in1 = 2; // define PWM pins for motor (IN1, IN2)
int in2 = 3;

// Global variable declaration
volatile long countData = 0; // 32-bit int (signed?) the count represents absolute position
int PWMvalue = 0 ; //to be altered in main loop
int wallPos = 0;

//function definition 
void establishContact() {
  while (Serial.available() <= 0) {
  Serial.println("A");   // send a capital A
  delay(300);
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(9, OUTPUT);
  digitalWrite(9, LOW); // /RST resets the internal counter between runs
  delay(10);
  digitalWrite(9, HIGH); // Stay high for rest of the run
  
  // Set all pins in PORTA (digital pins 22->29 on the Mega) as input pins
  for(int i = 22; i<30; i++) {
    pinMode(i, INPUT);
  }
  // Set pins 5,6,7 in PORT B (digital pins 11,12,13 on the Mega) as output pins
  for(int j = 11; j<14; j++) {
    pinMode(j, OUTPUT);
    digitalWrite(j, LOW);
    
  }
  pinMode(in1, OUTPUT); //set pin mode for PWM output pins
  pinMode(in2, OUTPUT);

  pinMode(ledPin, OUTPUT); // Set pin as OUTPUT
  establishContact();  // send a byte to establish contact until receiver responds 


}
/*-------------------------------------------------
  -------------------Functions---------------------
  -------------------------------------------------*/
byte getMSB(){
/*Get stable data for the most significant byte of countData*/

  byte MSBold = PINA;       // read datapins D0->D7 and store in MSBold
  byte MSBnew = PINA;       // read again immediatly after to assure stable data
  if (MSBnew == MSBold){ 
    byte MSBresult = MSBnew;
    return MSBresult;  
  }
  else getMSB();
}

byte getSecond(){
/*Get stable data for the 2nd byte of countData*/
  byte secondOld = PINA;       // read datapins D0->D7 and store in secondOld
  byte secondNew = PINA;       // read again immediatly after to assure stable data
  if (secondNew == secondOld){ 
    byte secondResult = secondNew;
    return secondResult;  
  }
  else getSecond();
}

byte getThird(){
/*Get stable data for the 3rd byte of countData*/
  byte thirdOld = PINA;       // read datapins D0->D7 and store in thirdOld
  byte thirdNew = PINA;       // read again immediatly after to assure stable data
  if (thirdNew == thirdOld){ 
    byte thirdResult = thirdNew;
    return thirdResult;  
  }
  else getThird();
}

byte getLSB(){
/*Get stable data for the least significant byte of countData*/
  byte LSBold = PINA;       // read datapins D0->D7 and store in LSBold
  byte LSBnew = PINA;       // read again immediatly after to assure stable data
  if (LSBnew == LSBold){  
    byte LSBresult = LSBnew;
    return LSBresult;  
  }
  else getLSB();
}

long mergeFunc(byte MSBresult, byte secondResult, byte thirdResult, byte LSBresult){
/*Merges the 4 bytes returning one 32-bit variable called countData*/
  long tempVar = 0;
  tempVar |= ((long) MSBresult << 24) | ((long) secondResult << 16) | ((long) thirdResult << 8) | ((long) LSBresult << 0);
  countData = tempVar;
  return countData;
}



void loop() {
int desired_pos = 0;
int P = 8; // PID control value

if (Serial.available() > 0) { // If data is available to read,
    val = Serial.read(); // read it and store it in val
    
    if (int(val) != 65)
       wallPos = int(val); //wait for an integer 
    
    if(val != '0') //if we get a 1
    {
       ledState = !ledState; //flip the ledState
       digitalWrite(ledPin, ledState); 
    }
    delay(100);

  } 
  
else { // in controlling mode
  digitalWrite(13, HIGH); // Set OE to HIGH (disable)
  delay(10);  // need a better way
  
  digitalWrite(11, LOW);
  digitalWrite(12, HIGH); // SEL1 = 0 and SEL2 = 1
  
  digitalWrite(13, LOW); // Set OE to LOW (enable)
  byte MSBresult = getMSB();

  
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH); // SEL1 = 1 and SEL2 = 1
  byte secondResult = getSecond();
  
  digitalWrite(11, LOW);
  digitalWrite(12, LOW); // SEL1 = 0 and SEL2 = 0
  byte thirdResult = getThird();
  
  digitalWrite(11, HIGH);
  digitalWrite(12, LOW); // SEL1 = 1 and SEL2 = 0
  byte LSBresult = getLSB();

  digitalWrite(13, HIGH); // Set OE to HIGH (disable)
  delay(10);
  
  countData = mergeFunc(MSBresult, secondResult, thirdResult, LSBresult);
  
if(wallPos != 0 and wallPos != 48){
  // wall at x = val
  //countData = abs(countData); 
  
  if(countData < wallPos or countData > (wallPos+60))
    PWMvalue = 0;
  
  else {
  //  PWMvalue = abs(wallPos-abs(countData))*P;
    PWMvalue = 200; 
  if(PWMvalue > 255)
    PWMvalue = 255;
  }
  if(countData < (wallPos+30)) {
    digitalWrite(in1, LOW);
    analogWrite(in2, PWMvalue); }
  else 
    {
    digitalWrite(in2, LOW);
    analogWrite(in1, PWMvalue); }
}
//  Serial.print("Counter: ");
  Serial.println(countData);
//  Serial.print(" ");
//  Serial.print("PWMvalue: ");
//  Serial.println(PWMvalue);
    delay(50);

}

  }

  ///////////////POSITION TRACKING////////////////
/*
if(countData < 0){
  countData = -countData;
}
 if(abs(countData-desired_pos) < 5){ // P control
    PWMvalue = 0;
    digitalWrite(in2, LOW);
    digitalWrite(in1, LOW);
    }
else if(desired_pos < abs(countData)){
    PWMvalue = abs(desired_pos-countData)*P;
    if(PWMvalue > 255){
      PWMvalue = 255; }
    digitalWrite(in1, LOW);
    analogWrite(in2, PWMvalue);
  }
  else{
  PWMvalue = abs(desired_pos-abs(countData))*P;
  if(PWMvalue > 255) {
    PWMvalue = 255; }
  digitalWrite(in2, LOW);
  analogWrite(in1, PWMvalue);
 } 
 
 */
  
  
