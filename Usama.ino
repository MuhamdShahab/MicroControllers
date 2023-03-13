#include <util/atomic.h> 
#define ENCR 3// Orange
#define ENCR1 51 // Black
#define ENCL 2 // Black
#define ENCL1 23 // Orange

#define RM 10 // EN - RIGHT
#define LM 7 // EN - Left

// the number of t he Motor Pins
const int RM1 = 8; //IN4 - Right
const int RM2 = 9;  //IN3 - Right
const int LM1 = 5; //IN2 - Left
const int LM2 = 6;  //IN1 - Left
volatile int ticksR = 0;
volatile int ticksL = 0; 


int prevticksR = 0 ;
int prevticksL = 0;
int pwmR = 170;
int pwmL = 170;
int motors_offset = 2;
double time1 =1000;
int diff_ticksR =0;
int diff_ticksL = 0;



void readEncoderR();
void readEncoderL();
void forward();
void backward();
void left();
void right();
void brake();
void drivestraight();

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

  delay(5000);
}


void loop()
{ 
  drivestraight(1000);   
  while(true)
  {}
}

void drivestraight(int dist) 
{
  float turns = dist/33; //33 is circumference of the tyre
  Serial.print("Total Ticks: ");   Serial.println(turns * 375);
  int limit =  turns * 202; //202 tyre resolution
  
  while(ticksR <= limit && ticksL <= limit)
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
    if ( diff_ticksR > diff_ticksL ) {
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
    pwmR = constrain(pwmR, 100,200);
    pwmL = constrain(pwmL, 100,200);
    pwmR+=4;
    
    Serial.print(ticksR); Serial.print("(R)  -  ");Serial.print(ticksL);Serial.print("(L)"); Serial.print(" ---> ");Serial.println(ticksR-ticksL);
    forward(pwmR,pwmL);
    delay(20);
    
  }
  brake();
  delay(2000);
  Serial.print(ticksR); Serial.print("(R)  -  ");Serial.print(ticksL);Serial.print("(L)"); Serial.print(" ---> ");Serial.println(ticksR-ticksL);
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
