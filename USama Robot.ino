#include <util/atomic.h> 
#include <NewPing.h>

#define SONAR_NUM 3    // Number of sensors.
#define MAX_DISTANCE 350 // Maximum distance (in cm) to ping.

#define limit 30


NewPing sonar[SONAR_NUM] = {   // Sensor object array.
  NewPing(A6, A7, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping. front
  NewPing(A13, A12, MAX_DISTANCE), //Right
  NewPing(A9, A8, MAX_DISTANCE) //left
};

#define ENCR 3// Orange
#define ENCR1 A0 // Black
#define ENCL 2 // Black
#define ENCL1 14 // Orange

#define RM 5 // EN - RIGHT
#define LM 8 // EN - Left

#define relay A15

// the number of t he Motor Pins
const int RM1 = A3; //IN4 - Right
const int RM2 = A4;  //IN3 - Right
const int LM1 = 10; //IN2 - Left
const int LM2 = 9;  //IN1 - Left


volatile int ticksR = 0;
volatile int ticksL = 0; 
int prevticksR = 0 ;
int prevticksL = 0;
int pwmR = 170;
int pwmL = 170;
int motors_offset = 2;
int diff_ticksR =0;
int diff_ticksL = 0;


void readEncoderR();
void readEncoderL();
void forward(int p, int l);
void backward(int p, int l);
void left(int p, int l);
void right(int p, int l);
void brake();
void drivestraight();


bool get_front()
{
  delay(50);
  int front = sonar[0].ping_cm();
  if(front > 1 && front < limit)
    return true;
  else
    return false;
}

bool get_right()
{
  delay(50);
  int right = sonar[1].ping_cm();
  if(right > 1 && right < limit)
    return true;
  else
    return false;
}

bool get_left()
{
  delay(50);
  int left = sonar[2].ping_cm();
  if(left > 1 && left < limit)
    return true;
  else
    return false;
}

void setup() {
  Serial.begin(115200);
  
  pinMode(ENCR,INPUT_PULLUP);
  pinMode(ENCR1,INPUT_PULLUP);   
  pinMode(ENCL,INPUT_PULLUP);
  pinMode(ENCL1,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCR),readEncoderR,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCL),readEncoderL,RISING);
 
  pinMode(RM,OUTPUT);
  pinMode(RM1,OUTPUT);
  pinMode(RM2,OUTPUT);
  pinMode(LM,OUTPUT);
  pinMode(LM1,OUTPUT);
  pinMode(LM2,OUTPUT);

  //pinMode(relay, OUTPUT);

  delay(5000);
}

int state = 3;
int count = 6;

void loop()
{ 
  //digitalWrite(relay, LOW);
  unsigned long start = 0;
  start = millis();
  bool obs = false;
  while(millis() - start < 300000)
  {
    // Number of ticks counted since last time

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
    diff_ticksR = ticksR - prevticksR;
    diff_ticksL = ticksL - prevticksL;

    // Store current tick counter for next time
    prevticksR = ticksR;
    prevticksL = ticksL;
    }

    // If left is faster, slow it down and speed up right
    if ( diff_ticksR > diff_ticksL ) 
    {
      pwmR -= motors_offset;
      pwmL += motors_offset;
    }

    // If right is faster, slow it down and speed up left
    else if ( diff_ticksR < diff_ticksL ) {
      pwmR += motors_offset;
      pwmL -= motors_offset;        
    }
    else
     {
      //do nothing
     }
    pwmR = constrain(pwmR, 70,120);
    pwmL = constrain(pwmL, 70,120);

    bool fronti = get_front();
    bool lefti = get_left();
    bool righti = get_right();

    if(state < 3 && state >= 1)
      count--;
    else
      count = 8;

    if(count <= 0)
    {
      delay(100);
      if(fronti)
      {
        backward(100,150);
        delay(300);
        brake();
      }
      else
      {
        forward(100,100);
        delay(100);
        brake();
      }
      count == 4;
    }
      
    if(fronti && righti && lefti)
    {
      while(get_right() && get_left() && get_front())
      {
       backward(100,150);
       delay(50);
      }
      delay(100);
      brake();
    }
    else if(fronti)
    {
      brake();
      while(get_front())
      {
        if(get_left() && !get_right())
           right(100,100);
        else if(!get_left() && get_right())
            left(100,100);
        else
          left(100,100);
       state = 3;
      }
      brake();
 
    }
    else if(lefti)
    {
      brake();
      while(get_left())
      {
        right(100,100);
        state = 2;
      }
      brake();

    }
    else if(righti)
    {
      brake();
      while(get_right())
      {
        left(100,100);
        state = 1;
      }
      brake();
    }
    else
    {
      forward(pwmR,pwmL);
    }
  }
  brake();
  while(1)
  {
    
  }
}

// Motors Directions with PWM
void forward(int pwmr,int pwml)
{
   digitalWrite(RM1, HIGH);
   digitalWrite(RM2, LOW);
   analogWrite(RM, pwmr);  
   digitalWrite(LM1, HIGH);
   digitalWrite(LM2, LOW);  
   analogWrite(LM, pwml);
}

void backward(int pwmr,int pwml)
{
   digitalWrite(RM1, LOW);
   digitalWrite(RM2, HIGH);
   analogWrite(RM, pwmr);   
   digitalWrite(LM1, LOW);
   digitalWrite(LM2, HIGH);  
   analogWrite(LM, pwml);
}
void left(int pwmr,int pwml)
{
   digitalWrite(RM1, HIGH);
   digitalWrite(RM2, LOW);
   analogWrite(RM, pwmr);   
   digitalWrite(LM1, LOW);
   digitalWrite(LM2, HIGH);  
   analogWrite(LM, pwml);
}

void right(int pwmr,int pwml)
{
   digitalWrite(RM1, LOW);
   digitalWrite(RM2, HIGH);
   analogWrite(RM, pwmr);   
   digitalWrite(LM1, HIGH);
   digitalWrite(LM2, LOW);  
   analogWrite(LM, pwml);
}

void brake()
{
   digitalWrite(RM1, LOW);
   digitalWrite(RM2, LOW);
   analogWrite(RM, 0);    
   digitalWrite(LM1, LOW);
   digitalWrite(LM2, LOW);  
   analogWrite(LM, 0);
}

//Interrupt Functions for left and right motors

void readEncoderR(){
  if(digitalRead(ENCR1) > 0){
    ticksR++;
  }
  else{
    ticksR--;
  }
}

void readEncoderL(){
  if(digitalRead(ENCL1) > 0){
    ticksL++;
  }
  else{
    ticksL--;
  }
}
