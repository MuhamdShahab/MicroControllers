//Reach at https://www.linkedin.com/in/muhammad-shahab-2773a61a5/
//Github https://github.com/MuhamdShahab/
#include<NewPing.h>
#include <math.h>
using namespace std;
NewPing sonar(8,9,200);//function(echopin,trigpin,maxrange)

//Variables starts here....
double dist;
int tacho=7;// input ticks
int steps=0; // ticks

double check=0;//ticks changes
double angle;// best angle
double tempangle;// best angle
double angle1;// angle to change orientation to move to goal
double angle2;// angle for changing orrientation to straight after reaching goal
double dtc=0; // distace to circle
double dtg=0;
double dto=0;
double limit;//max ticks allowed in that degree
int control=0;
double dtig; //inclined distance to goal;


//Variable Ends Here....

//SettingUP starts here....

void setup()
{
Serial.begin(9600); //BAUD RATE SET
pinMode(13,OUTPUT);   //left motors forward Declaration
pinMode(12,OUTPUT);   //left motors reverse Declaration
pinMode(11,OUTPUT);   //right motors forward Declaration
pinMode(10,OUTPUT);   //right Reverse Declaration
pinMode(6,OUTPUT);   //BUZZER
pinMode(5,OUTPUT);   //GREEN LED
pinMode(4,OUTPUT);   //Red Right LED
pinMode(3,OUTPUT);   //Red RIGHT LED
pinMode(tacho,INPUT_PULLUP);
}
//SettingUP ends here....


//Loop starts here....

void obstacle_detect()//Obstacle detection for the car
{
dist=sonar.ping_cm();
Serial.println(dist);//shows distance on Monitor
}
void seedha(void);
void khabay(void);
void pichay(void);
void sajay(void);
void khalja(void);
void RPO(void);
void maneuver(void);


void loop() ////////////////////////////////////////////////////////////////////////////////    LOOP       ////////////////////////////////////////////////////////////////
{
obstacle_detect();
if(dist>50 || dist==0){  //move forward
    seedha();
    check=0;
    control=1;
    digitalWrite(5, HIGH);
}
else{
  if(control==1){
    khalja();
      Serial.print("Origin: ");
      Serial.println("0.00,0.00");
      Serial.print("Obstacle: ");
      Serial.print(dist);
      Serial.println(",0.00 ");
      Serial.print("Goal: ");
      Serial.print(2*dist);
      Serial.println(",0.00");
      digitalWrite(5, LOW);
      pinMode(6,HIGH);//BUZZER HIGH
      dto=dist;
      dtg=2*dist;
      digitalWrite(5, LOW);
    RPO();
  }
}
//RPO();
}


//Changes orientation....................






  
  //Car Directions/controls Function starts here....
void seedha()
{
  digitalWrite(13,HIGH);
  digitalWrite(11,HIGH);
  digitalWrite(12,LOW);
  digitalWrite(10,LOW);
  
}
void pichay()
{
  digitalWrite(12,HIGH);
  digitalWrite(10,HIGH);
  digitalWrite(13,LOW);
  digitalWrite(11,LOW);
}
void khabay()
{
  digitalWrite(13,HIGH);
  digitalWrite(10,HIGH);
  digitalWrite(12,LOW);
  digitalWrite(11,LOW);
}
void sajay()
{
  digitalWrite(13,LOW);
  digitalWrite(10,LOW);
  digitalWrite(12,HIGH);
  digitalWrite(11,HIGH);
}
void khalja()
{
  digitalWrite(13,LOW);
  digitalWrite(12,LOW);
  digitalWrite(11,LOW);
  digitalWrite(10,LOW);
}
//Car Direction/control ends here....



////////////////////////////////////////////////// Kachra starts here /////////////////////
void RPO(){
  
  control=2;
  double Xc,Yc;   //current_points
  double Xo,Yo; //obstacle_points
  double Xt,Yt;   //target_points
  double tempX=0;
  double tempY=0;
  double tempJ=0;
  double tempD=0;
  int count=0;
  int ikonsi;
  double alpho,Uo,alpht,Ut;
  Xc=0;
  Yc=0;
  Xo=0.1*dist;
  Yo=0;
  Xt=(2*Xo);
  Yt=0;
  double stepsize=0.40*(sqrt(pow(Xc-Xo,2)+pow(Yc-Yo,2)));
  Serial.print("StepSize: ");
  Serial.println(stepsize);
  alpho=1;
  Uo=4;
  alpht=1;
  Ut=4;
  double DTG;
  DTG=sqrt(pow(Xc-Xt,2)+pow(Yc-Yt,2));
  double Jobs = alpho*(exp(-Uo*(pow(Xc-Xo,2)+pow(Yc-Yo,2))));
  double Jgoal = -alpht*(exp(-Ut*(pow(Xc-Xt,2)+pow(Yc-Yt,2))));
  double Jtotal= Jobs+Jgoal;
  double NPTS=30;
    double step_deg=360/NPTS;
    
    if(control==2){
  //Serial.print("Control: ");
  //Serial.println(control);
    for(int i=1; i<=NPTS;i++){
      //Artificial points
      double Xnew=Xc+stepsize*(cos(M_PI*(step_deg*i/180)));  
      double Ynew=Yc+stepsize*(sin(M_PI*(step_deg*i/180)));
      //New DTG
      double DTGnew=sqrt(pow(Xnew-Xt,2)+pow(Ynew-Yt,2));
     //New Potential
      double Jobsnew = alpho*(exp(-Uo*(pow(Xnew-Xo,2)+pow(Ynew-Yo,2))));
    double Jgoalnew = -alpht*(exp(-Ut*(pow(Xnew-Xt,2)+pow(Ynew-Yt,2))));
    double Jtotalnew= Jobsnew+Jgoalnew;
    double ErrJ=Jtotalnew-Jtotal;
    double ErrD=DTGnew-DTG;
    if(ErrJ<0 && ErrD<0){
    if(ErrJ<tempJ && ErrD < tempD)
    {
        tempX=Xnew;
        tempY=Ynew;
        count++;
        ikonsi=i;
        
    }
    tempJ=ErrJ;
    tempD=ErrD;
    }

    control=3;
   }
      angle=(ikonsi*(360/NPTS));
  }
  maneuver();///ROBOT PERFORMS ACTIONSSSSSSSSSSSSS
}


void orientation(double angel)
{
  check=0;
  limit=(angel/360);
  limit=(limit*80);
  Serial.print("Angle Ticks: ");
  Serial.println(limit);
  while(check<=limit){
  steps=0;
   while(steps<1){
   if(digitalRead(tacho))
   {
    steps=steps+1;
    while(digitalRead(tacho))
    {
      khabay();
    }
   }
    else{
     khabay();
    }
  }
  //khalja();
  check++;
}
  control=4;
}

void moveforwardtillticks(double d2c)
{
  check=0;
  d2c= d2c/21;//check how many rotation requires
  Serial.print("Cycles: ");
  Serial.println(d2c);
  d2c=d2c*20;// calculte ticks needed for that distance
 // d2c=d2c+5;                 //////////////////////////////////to  move to circle with safe distance precautionary///
  d2c=d2c;
  Serial.print("Distance Ticks: ");
  Serial.println(d2c);
  while(check<=d2c){
  steps=0;
   while(steps<1){
   if(digitalRead(tacho))
   {
    steps=steps+1;
    while(digitalRead(tacho))
    {
      seedha();
    }
   }
    else{
     seedha();
    }
  }
  check++;
}if (control==4)
  control=5;
 else
    control=7;
}

double deg2rad(double degree)
{
    double pi = 3.14159265359;
    return (degree * (pi / 180));
}
double rad2deg(double radian)
{
    double pi = 3.14159;
    return(radian * (180 / pi));
}
double angleddistancetogoal(double angele)
{
  double lineG;
  
  lineG=sqrt(pow(dtg,2)+pow(dtc,2)-(2*dtg*dtc*cos(deg2rad(angele))));
  Serial.print("Inclined Distance to GOAL ");
  Serial.println(lineG);
  return lineG;
}

void calculation(double angel)
{
  dto=dist;
  dtg=2*dto;
  dtc=0.4*dto;
  dtig=angleddistancetogoal(angel);
  tempangle= ((pow(dtc,2)+pow(dtig,2)-pow(dtg,2))/(2*dtig*dtc));
  angle1=rad2deg(acos(tempangle));
  Serial.print("Angle1 in Radians ");
  Serial.println(angle1);
  angle2=180-angle-angle1;
  Serial.print("Angle2 in Radians ");
  Serial.println(angle2);

  
  
  
}
void orientationx2(double angel)
{
  check=0;
  tempangle=angle+angle2;
  limit=(tempangle/360);
  limit=(limit*80);
  Serial.print("Direction to Goal: ");
  Serial.println(tempangle);
  Serial.print("Angle Ticks: ");
  Serial.println(limit);
  while(check<=limit){
  steps=0;
   while(steps<1){
   if(digitalRead(tacho))
   {
    steps=steps+1;
    while(digitalRead(tacho))
    {
      sajay();
    }
   }
    else{
     sajay();
    }
  }
  check++;
}
  control=6;
}

void maneuver()
{
if(control==3)
{
  digitalWrite(4, HIGH);
  Serial.print("Angle: ");
  Serial.println(angle);
  khalja();
  digitalWrite(6,HIGH);
  delay(1000);
  digitalWrite(6,LOW);
  orientation(angle);
}
if(control==4)
{
   dtc=0.4*(dist);
  Serial.print("DTC: ");
  Serial.println(dtc);
   khalja();
    delay(1000);
 moveforwardtillticks(dtc);
 digitalWrite(4, LOW);
}
if(control==5)
{
  digitalWrite(3, HIGH);
  calculation(angle);
   khalja();
  delay(1000);
  orientationx2(angle2);
}
if(control==6)
{
  khalja();
  delay(1000);
  moveforwardtillticks(dtig);
  //Serial.println("I am going to Goal lalalalaa");
  digitalWrite(3, LOW);
}
if(control==7)
{
  digitalWrite(4, HIGH);
  khalja();
  delay(1000);
 orientation(angle2);
  //Serial.println("I am going to Goal lalalalaa");
  digitalWrite(4, LOW);
}
  khalja();
  digitalWrite(6,HIGH);
  delay(1000);
  digitalWrite(6,LOW);
  while(1)
  {
   khalja(); 
   digitalWrite(5, HIGH);
  
  }
  }
