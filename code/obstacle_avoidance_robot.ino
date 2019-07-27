#include <AFMotor.h>      //add Adafruit Motor Shield library
#include <Servo.h>        //add Servo Motor library            

#define TRIG_PIN A0 // Pin A0 on the Motor Drive Shield soldered to the ultrasonic sensor
#define ECHO_PIN A1 // Pin A1 on the Motor Drive Shield soldered to the ultrasonic sensor
#define TRIG_PINL A2 // Pin A2 on the Motor Drive Shield soldered to the ultrasonic sensor
#define ECHO_PINL A3 // Pin A3 on the Motor Drive Shield soldered to the ultrasonic sensor
#define TRIG_PINR A4 // Pin A4 on the Motor Drive Shield soldered to the ultrasonic sensor
#define ECHO_PINR A5 // Pin A5 on the Motor Drive Shield soldered to the ultrasonic sensor
#define MAX_SPEED 75 // sets speed of DC traction motors to 150/250 or about 70% of full speed - to get power drain down.
#define MAX_SPEED_OFFSET 40 // this sets offset to allow for differences between the two DC traction motors
#define COLL_DIST 40 // sets distance at which robot stops and reverses to 30cm
#define TURN_DIST COLL_DIST+20 // sets distance at which robot veers away from object
float duration,distance,durationR, distanceR,durationL, distanceL;

AF_DCMotor leftMotor1(1, MOTOR12_1KHZ); // create motor #1 using M1 output on Motor Drive Shield, set to 1kHz PWM frequency
AF_DCMotor leftMotor2(2, MOTOR12_1KHZ); // create motor #2, using M2 output, set to 1kHz PWM frequency
AF_DCMotor rightMotor1(3, MOTOR34_1KHZ);// create motor #3, using M3 output, set to 1kHz PWM frequency
AF_DCMotor rightMotor2(4, MOTOR34_1KHZ);// create motor #4, using M4 output, set to 1kHz PWM frequency
Servo myservo;  // create servo object to control a servo 
Servo myservo1;  // create servo object to control a servo 
const int soundpin=2;

int leftDistance, rightDistance; //distances on either side
int curDist = 0;
String motorSet = "";
int speedSet = 0;

//-------------------------------------------- SETUP LOOP ----------------------------------------------------------------------------
void setup() {
 myservo.attach(10);  // attaches the servo on pin 10 (SERVO_1 on the Motor Drive Shield to the servo object 
  myservo.write(90); // tells the servo to position at 90-degrees ie. facing forward.
  delay(600); // delay for one seconds
  myservo1.attach(9);  // attaches the servo on pin 10 (SERVO_1 on the Motor Drive Shield to the servo object 
  myservo1.write(90); // tells the servo to position at 90-degrees ie. facing forward.
  delay(600); // delay for one seconds
  pinMode(TRIG_PIN,OUTPUT);
  pinMode(ECHO_PIN,INPUT);
  Serial.begin(9600);
   pinMode(soundpin,INPUT);
  pinMode(TRIG_PINR,OUTPUT);
 pinMode(ECHO_PINR,INPUT);
 pinMode(TRIG_PINL,OUTPUT);
 pinMode(ECHO_PINL,INPUT);
 }
//------------------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------MAIN LOOP ------------------------------------------------------------------------------
void loop() {
  sound1();
  myservo.write(90);  // move eyes forward
  delay(90);
  curDist = readPing();   // read distance
  //int s=forecheck();
  //int q=forecheck1();
  if ((curDist < COLL_DIST)/*||s==1*/) {if (curDist<=10){moveBackward3();}else{changePath();}}  // if forward is blocked change direction
  else {moveForward();delay(200);}
 
 }
//-------------------------------------------------------------------------------------------------------------------------------------

void changePath() {
     
  moveBackward2();
  moveStop();            // stop forward movement
  myservo.write(36);  // check distance to the right
    delay(500);
    rightDistance = readPing(); //set right distance
    //delay(500);
    myservo.write(144);  // check distace to the left
    delay(700);
    leftDistance = readPing(); //set left distance
    //delay(500);
    myservo.write(90); //return to center
    delay(100);
    compareDistance();
  }

  
void compareDistance()   // find the longest distance
{
  if ((leftDistance>rightDistance)&&(rightDistance>10 && leftDistance>10)) //if left is less obstructed 
  {Serial.print("left");
    turnLeft();
  }
  else if ((rightDistance>leftDistance)&&(rightDistance>10 && leftDistance>10)) //if right is less obstructed
  {Serial.print("right");
    turnRight();
  }
   else if (rightDistance<=10 && leftDistance<=10) {Serial.print("backwards"); moveBackward();}
  else                    //if they are equally obstructed
  {Serial.print("back");
  if (leftDistance<10){Serial.print("turnaroundR");turnAroundR();}
  else {Serial.print(" turnAroundL");turnAroundL();}
  }
}


//-------------------------------------------------------------------------------------------------------------------------------------

int readPing() 
{ // read the ultrasonic sensor distance
   analogWrite(TRIG_PIN,0);
 delayMicroseconds(2);
  analogWrite(TRIG_PIN,255);
 delayMicroseconds(10);
  digitalWrite(TRIG_PIN,0);

 duration = pulseIn(ECHO_PIN,255);
 distance = (duration*.0343)/2;
 Serial.print("Distance: ");
 Serial.println(distance);
 return distance;
 
}
//-------------------------------------------------------------------------------------------------------------------------------------
void moveStop() {leftMotor1.run(RELEASE); leftMotor2.run(RELEASE); rightMotor1.run(RELEASE); rightMotor2.run(RELEASE);}  // stop the motors.
void moveForward2(){
   motorSet = "BACKWARD";
    leftMotor1.run(FORWARD);     // turn it on going backward
    leftMotor2.run(FORWARD);     // turn it on going backward
    leftMotor1.setSpeed(speedSet+MAX_SPEED_OFFSET);     
    leftMotor2.setSpeed(speedSet+MAX_SPEED_OFFSET);    
    rightMotor1.run(FORWARD);    // turn it on going backward
    rightMotor2.run(FORWARD);    // turn it on going backward
    rightMotor1.setSpeed(speedSet+MAX_SPEED_OFFSET);      
    rightMotor2.setSpeed(speedSet+MAX_SPEED_OFFSET);
    delay(50);
}
void moveBackward2(){
   motorSet = "BACKWARD";
    leftMotor1.run(BACKWARD);     // turn it on going backward
    leftMotor2.run(BACKWARD);     // turn it on going backward
    leftMotor1.setSpeed(speedSet+MAX_SPEED_OFFSET);     
    leftMotor2.setSpeed(speedSet+MAX_SPEED_OFFSET);    
    rightMotor1.run(BACKWARD);    // turn it on going backward
    rightMotor2.run(BACKWARD);    // turn it on going backward
    rightMotor1.setSpeed(speedSet+MAX_SPEED_OFFSET);      
    rightMotor2.setSpeed(speedSet+MAX_SPEED_OFFSET);
    delay(50);
}
//-------------------------------------------------------------------------------------------------------------------------------------
void moveForward() {
  check();
    motorSet = "FORWARD";
    leftMotor1.run(FORWARD);      // turn it on going forward
    leftMotor2.run(FORWARD);      // turn it on going forward
    rightMotor1.run(FORWARD);     // turn it on going forward
    rightMotor2.run(FORWARD);     // turn it on going forward
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    leftMotor1.setSpeed(speedSet);
    leftMotor2.setSpeed(speedSet);
    rightMotor1.setSpeed(speedSet); 
    rightMotor2.setSpeed(speedSet);
    delay(5);
  }
}
//-------------------------------------------------------------------------------------------------------------------------------------
void moveBackward3(){
  moveBackward2();
  int statu=digitalRead(soundpin);
 if (statu==0){moveForward2(); changePath();}
 else{
   motorSet = "BACKWARD";
    leftMotor1.run(BACKWARD);     // turn it on going backward
    leftMotor2.run(BACKWARD);     // turn it on going backward
    rightMotor1.run(BACKWARD);    // turn it on going backward
    rightMotor2.run(BACKWARD);    // turn it on going backward
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    leftMotor1.setSpeed(speedSet);
    leftMotor2.setSpeed(speedSet);
    rightMotor1.setSpeed(speedSet); 
    rightMotor2.setSpeed(speedSet); 
    delay(10);
  }delay(350);
}
}
void moveBackward1(){
   motorSet = "BACKWARD";
    leftMotor1.run(BACKWARD);     // turn it on going backward
    leftMotor2.run(BACKWARD);     // turn it on going backward
    rightMotor1.run(BACKWARD);    // turn it on going backward
    rightMotor2.run(BACKWARD);    // turn it on going backward
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    leftMotor1.setSpeed(speedSet);
    leftMotor2.setSpeed(speedSet);
    rightMotor1.setSpeed(speedSet); 
    rightMotor2.setSpeed(speedSet); 
    delay(10);
  }delay(350);
}
void moveBackward()
{   
    int q=backservo(); 
    if(q==0){    
    motorSet = "BACKWARD";
    leftMotor1.run(BACKWARD);     // turn it on going backward
    leftMotor2.run(BACKWARD);     // turn it on going backward
    rightMotor1.run(BACKWARD);    // turn it on going backward
    rightMotor2.run(BACKWARD);    // turn it on going backward
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    leftMotor1.setSpeed(speedSet);
    leftMotor2.setSpeed(speedSet);
    rightMotor1.setSpeed(speedSet); 
    rightMotor2.setSpeed(speedSet); 
    delay(10);
  }
  delay(600);
  moveStop();   // stop forward movement
  myservo.write(10);  // check distance to the right
    delay(500);
    rightDistance = readPing(); //set right distance
    //delay(500);
    myservo.write(170);  // check distace to the left
    delay(800);
    leftDistance = readPing(); //set left distance
    //delay(500);
    myservo.write(90); //return to center
    delay(100);

  if(rightDistance>10 || leftDistance>10){
   if(rightDistance>leftDistance){turnAroundR();}
   else{turnAroundL();}
  }
  else{moveBackward();}
} 
} 
//-------------------------------------------------------------------------------------------------------------------------------------
void turnRight() {
  motorSet = "RIGHT";
  leftMotor1.run(FORWARD);      // turn motor 1 forward
  leftMotor2.run(FORWARD);      // turn motor 2 forward
  leftMotor1.setSpeed(speedSet+MAX_SPEED_OFFSET);     
  leftMotor2.setSpeed(speedSet+MAX_SPEED_OFFSET);    
  rightMotor1.run(BACKWARD);    // turn motor 3 backward
  rightMotor2.run(BACKWARD);    // turn motor 4 backward
  rightMotor1.setSpeed(speedSet+MAX_SPEED_OFFSET);      
  rightMotor2.setSpeed(speedSet+MAX_SPEED_OFFSET);  
  delay(510); // run motors this way for 1500        
 
}  
//-------------------------------------------------------------------------------------------------------------------------------------
void turnLeft() {
  motorSet = "LEFT";
  leftMotor1.run(BACKWARD);      // turn motor 1 backward
  leftMotor2.run(BACKWARD);      // turn motor 2 backward
  leftMotor1.setSpeed(speedSet+MAX_SPEED_OFFSET);     
  leftMotor2.setSpeed(speedSet+MAX_SPEED_OFFSET);    
  rightMotor1.run(FORWARD);     // turn motor 3 forward
  rightMotor2.run(FORWARD);     // turn motor 4 forward
  rightMotor1.setSpeed(speedSet+MAX_SPEED_OFFSET);     
  rightMotor2.setSpeed(speedSet+MAX_SPEED_OFFSET);    
  delay(510); // run motors this way for 1500  
  
}  
//-------------------------------------------------------------------------------------------------------------------------------------
void turnAroundR1() {
  motorSet = "RIGHT";
  leftMotor1.run(FORWARD);      // turn motor 1 forward
  leftMotor2.run(FORWARD);      // turn motor 2 forward
  leftMotor1.setSpeed(speedSet+MAX_SPEED_OFFSET);     
  leftMotor2.setSpeed(speedSet+MAX_SPEED_OFFSET);    
  rightMotor1.run(BACKWARD);    // turn motor 3 backward
  rightMotor2.run(BACKWARD);    // turn motor 4 backward
  rightMotor1.setSpeed(speedSet+MAX_SPEED_OFFSET);      
  rightMotor2.setSpeed(speedSet+MAX_SPEED_OFFSET);
  delay(1000); // run motors this way for 1700
    
}
void turnAroundR() {
  motorSet = "RIGHT";
  leftMotor1.run(FORWARD);      // turn motor 1 forward
  leftMotor2.run(FORWARD);      // turn motor 2 forward
  leftMotor1.setSpeed(speedSet+MAX_SPEED_OFFSET);     
  leftMotor2.setSpeed(speedSet+MAX_SPEED_OFFSET);    
  rightMotor1.run(BACKWARD);    // turn motor 3 backward
  rightMotor2.run(BACKWARD);    // turn motor 4 backward
  rightMotor1.setSpeed(speedSet+MAX_SPEED_OFFSET);      
  rightMotor2.setSpeed(speedSet+MAX_SPEED_OFFSET);
  delay(900); // run motors this way for 1700
    
}
void turnAroundL() {
  motorSet = "LEFT";
  leftMotor1.run(BACKWARD);      // turn motor 1 forward
  leftMotor2.run(BACKWARD);      // turn motor 2 forward
  leftMotor1.setSpeed(speedSet+MAX_SPEED_OFFSET);     
  leftMotor2.setSpeed(speedSet+MAX_SPEED_OFFSET);    
  rightMotor1.run(FORWARD);    // turn motor 3 backward
  rightMotor2.run(FORWARD);    // turn motor 4 backward
  rightMotor1.setSpeed(speedSet+MAX_SPEED_OFFSET);      
  rightMotor2.setSpeed(speedSet+MAX_SPEED_OFFSET);
  delay(900); // run motors this way for 1700
     
}    
void check() 
{ // read the ultrasonic sensor distance
  Serial.println("insidecheck()");
   analogWrite(TRIG_PINR,0);
 delayMicroseconds(2);
  analogWrite(TRIG_PINR,255);
 delayMicroseconds(10);
  analogWrite(TRIG_PINR,0);

 durationR = pulseIn(ECHO_PINR,255);
 distanceR = (durationR*.0343)/2;
 Serial.print("DistanceR: ");
 Serial.println(distanceR);
 //return distance;
 
analogWrite(TRIG_PINL,0);
 delayMicroseconds(2);
  analogWrite(TRIG_PINL,255);
 delayMicroseconds(10);
  analogWrite(TRIG_PINL,0);

 durationL = pulseIn(ECHO_PINL,255);
 distanceL = (durationL*.0343)/2;
 Serial.print("DistanceL: ");
 Serial.println(distanceL);

 if (distanceR<10 || distanceL<10)
 {Serial.print("insidecheck             if");
  moveStop();
  if (distanceR>distanceL){rightS();}
  else {leftS();}
 }
}

void rightS() {
  motorSet = "RIGHT";
  leftMotor1.run(FORWARD);      // turn motor 1 forward
  leftMotor2.run(FORWARD);      // turn motor 2 forward
  leftMotor1.setSpeed(speedSet+MAX_SPEED_OFFSET);     
  leftMotor2.setSpeed(speedSet+MAX_SPEED_OFFSET);    
  rightMotor1.run(BACKWARD);    // turn motor 3 backward
  rightMotor2.run(BACKWARD);    // turn motor 4 backward
  rightMotor1.setSpeed(speedSet+MAX_SPEED_OFFSET);      
  rightMotor2.setSpeed(speedSet+MAX_SPEED_OFFSET);  
  delay(230); // run motors this way for 1500        
 
}  
//-------------------------------------------------------------------------------------------------------------------------------------
void leftS() {
  motorSet = "LEFT";
  leftMotor1.run(BACKWARD);      // turn motor 1 backward
  leftMotor2.run(BACKWARD);      // turn motor 2 backward
  leftMotor1.setSpeed(speedSet+MAX_SPEED_OFFSET);     
  leftMotor2.setSpeed(speedSet+MAX_SPEED_OFFSET);    
  rightMotor1.run(FORWARD);     // turn motor 3 forward
  rightMotor2.run(FORWARD);     // turn motor 4 forward
  rightMotor1.setSpeed(speedSet+MAX_SPEED_OFFSET);      
  rightMotor2.setSpeed(speedSet+MAX_SPEED_OFFSET);  
  delay(230); // run motors this way for 1500  
  
}  
int sound()
{
  int statussensor=digitalRead(soundpin);

 if (statussensor==0)
  { 
     return 1;
  }
  else {return 0;}
}
void sound1()
{
  int statussensor=digitalRead(soundpin);

 if (statussensor==0)
  { myservo.write(90);delay(100);int k=readPing();
     if (k>=30){leftS();rightS();leftS();rightS();}
     else {changePath();}
  }
  else {return 0;}
}
int backservo()
{
 myservo1.write(30);
    delay(400);
    int d=sound();
    delay(100);
    myservo1.write(150);
    delay(850);
    int e=sound();
    delay(100);
    myservo1.write(90);
    delay(300);
    int g=sound();
    delay(100);
    if(d==1){moveStop();leftS();return 1;}
    else if(e==1){moveStop();rightS();return 1;}
    else if(g==1){moveStop();turnAroundR1();moveBackward1();return 1;}
    else {return 0;}
}

