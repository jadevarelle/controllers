
//define pins for ISR
#define ENC_A 2
#define ENC_B 3

// motor
int enA = 10;
int in1 = 9;
int in2 = 11;
//isr
volatile unsigned int encoderPos1 =0;
volatile unsigned int encoderPos = 0; 
volatile unsigned int encoder = 0;
volatile unsigned int cycles = 0;
int turn = 0;
int ccw = false; 

//other
int PWMvalue = 0;
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
 attachInterrupt( 0, doEncoder1, CHANGE ); // Encoder #1, FALLING=128 steps/rev CHANGE=256 steps/rev 

}

void doEncoder1() {
  if(digitalRead(ENC_A) == HIGH){
    if(digitalRead(ENC_B) == LOW) {
      ccw = true; //counter clockwise
    }else {
      ccw = false; //clockwise
    }
  }else { //found a high to low on channel A
    if(digitalRead(ENC_B) == LOW){
      ccw = false; //clockwise
    }else{
      ccw = true; //counter clockwise
    }
  }
  if (ccw == true){
    encoderPos1 = (encoderPos1 - 1);
    if(encoderPos1 == -1){
      encoderPos1 = 199;
      turn = (turn-1);
    }}
    else{
      encoderPos1 = (encoderPos1 + 1);
      if (encoderPos1 == 200){
        encoderPos1 = 0;
        turn = (turn+1); 
      }
    } 
}

void do_circle()
{
  PWMvalue = 0;
}
void loop() {
  int desired_pos = 180;
  int actual_pos = encoderPos1*1.8;

/*
  if(actual_pos < desired_pos) {
    PWMvalue = 0;
    digitalWrite(in2, LOW);
    analogWrite(in1, PWMvalue);
   }
   else {
    PWMvalue = abs(actual_pos - desired_pos)*10; 
    digitalWrite(in1, LOW); //switch direction
    analogWrite(in2, PWMvalue);
   }
 */
 PWMvalue = abs(actual_pos - desired_pos);
 
 if((desired_pos-actual_pos) < (actual_pos-desired_pos)){
  digitalWrite(in2, LOW);
   analogWrite(in1, PWMvalue);
 }
 else{
  digitalWrite(in1, LOW);
  analogWrite(in2, PWMvalue);
//  Serial.print("clockwise");
 }
 // Serial.println(encoderPos1);
  Serial.println(actual_pos);
//  Serial.print(" ");
//  Serial.print( turn );
//  Serial.print(" ");
//  Serial.print( ccw );
//  Serial.print(" ");
//  Serial.print(PWMvalue);
//  Serial.print(" ");

}


