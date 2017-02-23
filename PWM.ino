
//define pins for ISR
#define ENC_A 2
#define ENC_B 3

// motor
int enA = 10;
int in1 = 9;
int in2 = 8;
int PWMvalue = 255; 
int i = 0; 
volatile unsigned int encoderPos;

void setup() {
  Serial.begin(9600); //initialize serial
  sei(); // enable interrupts
  
  //set up the encoder as input on pins 2 and 3
  pinMode(ENC_A, INPUT);
  digitalWrite(ENC_A, HIGH);
  pinMode(ENC_B, INPUT);
  digitalWrite(ENC_B, HIGH);

  //set up motor pins
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  digitalWrite(enA, HIGH);
  
   


  //set up interrupt
 attachInterrupt( 0, doEncoder1, FALLING ); // Encoder #1, FALLING=128 steps/rev CHANGE=256 steps/rev  

}

// Start of Interrupt routine for encoder #1
void doEncoder1() {
   if (digitalRead(ENC_A) == HIGH) {       // test for a low-to-high on channel A
     if ( digitalRead(ENC_B) == LOW ) {     // check channel B to see which way encoder is turning
       encoderPos = ++encoderPos;                // CW rotation
     }
     else {
        encoderPos = --encoderPos;                // CCW rotation
     }
   }
   else {                                        // it was a high-to-low on channel A
     if ( digitalRead(ENC_B) == HIGH ) {    // check channel B to see which way encoder is turning
       encoderPos = ++encoderPos;                // CW rotation
     }
     else {
       encoderPos = --encoderPos;                // CCW rotation
     }
 }
 if(encoderPos == 400)
    encoderPos = 0;
    
}     // End of interrupt code for encoder #1



void loop() {
  int deg;
  int desired_pos = 300;
  //int PWMvalue = abs(deg - desired_pos);
 //  PWMvalue -= 20; 

  digitalWrite(in2, LOW);
  analogWrite(in1, PWMvalue);
  delay(2000);
  analogWrite(in1, 0);
  delay(2000);
 digitalWrite(in1, LOW);
 analogWrite(in2, PWMvalue);
 delay(2000);
    //calculate degrees from encoder_count
  deg = encoderPos*0.9;
  Serial.println(PWMvalue);
 
}


