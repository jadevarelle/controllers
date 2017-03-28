import processing.serial.*;
int state;
PFont f;
Serial myPort;  // Create object from Serial class
String val;     // Data received from the serial port
int flag = 1;
int wall = 100;
boolean firstContact = false;
float x = 0;
int wallPos = 0;

void setup()
{
  // I know that the first port in the serial list on my mac
  // is Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  String portName = "/dev/cu.usbmodem1411";
//  Serial.list()[0]; //change the 0 to a 1 or 2 etc. to match your port
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil('\n');
 // myoutPort = new Serial(this, portName, 9600);

    size(280, 140);
    f = createFont("Arial",16,true);
    background(255); //clear screen
    fill(0); //set colour to black

}

void draw(){
  
     ellipse(x, 20, 20,20); 
     switch(state){
    case 0:
      fill(255,0,0); //red
      ellipse(20,20,20,20); //starting position
      stroke(175);
      textFont(f);       
      fill(0);
      textAlign(CENTER);
      text("Move to starting position",width/2,60); 
      if(x == 20) state = 1;
      break;
    case 1:
      if (mousePressed) {
        wallPos = mouseX;
        myPort.write(wallPos);
        println(wallPos);
        state = 2; }
    case 2:
      background(255); //clear screen
      fill(0); //set color to black
      ellipse(x, 20, 20, 20); //display position
      fill(0,0,0,125);
      rect(wallPos, 0, 60, 40);//virtual 'wall' at midway point
  }
}
void serialEvent(Serial myPort)
{
 // if ( myPort.available() > 0) 
  {  // If data is available,
  val = myPort.readStringUntil('\n');         // read it and store it in val
  } 
  if (val != null){
    val = trim(val);
    x = float(val);
    println(val);
  
  if(firstContact == false){
    if(val.equals("A")){
      myPort.clear();
      firstContact = true;
      myPort.write("A");
      println("contact");
    }
  }
  else {
    println(val);
  myPort.write("A");
    }
  }
}