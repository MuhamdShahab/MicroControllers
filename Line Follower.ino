//Reach at https://www.linkedin.com/in/muhammad-shahab-2773a61a5/
//Github https://github.com/MuhamdShahab/
#include <LiquidCrystal.h>
#define IR1 23 //IR Sensor1 array
#define IR2 25 //IR Sensor2 array
#define IR3 27 //IR Sensor3 array
#define IR4 29 //IR Sensor4 array
#define IR5 31 //IR Sensor5 array
#define IR6 2  //IR Senor Obstacle
const byte interruptPin1 = IR6;

const int voltageSensor = A0; //Voltage sensor PIN
const int currentSensor = A1; //Current sensor PIN

const int rs = 6, en = 5, d4 = 33, d5 = 35, d6 = 37, d7 = 39;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void loopi()
{
  lcd.setCursor(0,0);           //sets the cursor at row 0 column 0
  lcd.print("16x2 LCD MODULE"); // prints 16x2 LCD MODULE
  lcd.setCursor(2,1);           //sets the cursor at row 1 column 2
  lcd.print("HELLO WORLD");     // prints HELLO WORLD
}




float vOUT = 0.0;
float vIN = 0.0;
float R1 = 30000.0;
float R2 = 7500.0;
int value = 0;
float average = 0;

//var for RPM
unsigned long start_time = 0;
unsigned long end_time = 0;
int steps=0;
float steps_old=0;
float temp=0;
float RPM;

void setup() {
  Serial.begin(9600);
  pinMode(IR1,INPUT);//IR Declaration 1
  pinMode(IR2,INPUT);//IR Declaration 2
  pinMode(IR3,INPUT);//IR Declaration 3
  pinMode(IR4,INPUT);//IR Declaration 4
  pinMode(IR5,INPUT);//IR Declaration 5
  pinMode(IR6,INPUT);//IR Declaration 5
  pinMode(13,OUTPUT);//sajay motors forward Declaration
  pinMode(12,OUTPUT);//sajay motors reverse Declaration
  pinMode(11,OUTPUT);//right motors forward Declaration
  pinMode(10,OUTPUT);//right Reverse Declaration
  lcd.begin(16, 2);
  lcd.print("welcome");
  pinMode(interruptPin1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin1), interrupt1, CHANGE);

}
void interrupt1() 
{
 steps++;
}

void check()
{
  Serial.println(digitalRead(IR1));
  Serial.println(digitalRead(IR2));
  Serial.println(digitalRead(IR3));
  Serial.println(digitalRead(IR4));
  Serial.println(digitalRead(IR5));
  Serial.println("Ends");
}

void displayy()
{
 lcd.print("RPM|C-Amp|Volts");
 lcd.setCursor(0, 1);
 lcd.print(RPM);
 lcd.print('|');
  lcd.print(current());
 lcd.print('|');
 lcd.print(voltage());
Serial.print("RPM: ");
Serial.println(RPM);
Serial.print("Voltage: ");
Serial.println(voltage());

}


void car_control()
{
  
if (digitalRead(IR1)==1&& digitalRead(IR2)==1 && digitalRead(IR3)==0 && digitalRead(IR4)==1 && digitalRead(IR5)==1)
  forward();
else if (digitalRead(IR1)==1&& digitalRead(IR2)==1 && digitalRead(IR3)==0 && digitalRead(IR4)==0 && digitalRead(IR5)==1)
  sajay();
else if (digitalRead(IR1)==1&& digitalRead(IR2)==1 && digitalRead(IR3)==0 && digitalRead(IR4)==0 && digitalRead(IR5)==0)
  sajay();
else if (digitalRead(IR1)==1&& digitalRead(IR2)==1 && digitalRead(IR3)==1 && digitalRead(IR4)==0 && digitalRead(IR5)==1)
  sajay();
else if (digitalRead(IR1)==1&& digitalRead(IR2)==1 && digitalRead(IR3)==1 && digitalRead(IR4)==0 && digitalRead(IR5)==0)
  sajay();
else if (digitalRead(IR1)==1&& digitalRead(IR2)==1 && digitalRead(IR3)==1 && digitalRead(IR4)==1 && digitalRead(IR5)==0)
  sajay();
else if (digitalRead(IR1)==1&& digitalRead(IR2)==0 && digitalRead(IR3)==0 && digitalRead(IR4)==1 && digitalRead(IR5)==1)
  khabay();
else if (digitalRead(IR1)==0&& digitalRead(IR2)==0 && digitalRead(IR3)==0 && digitalRead(IR4)==1 && digitalRead(IR5)==1)
  khabay();
else if (digitalRead(IR1)==1&& digitalRead(IR2)==0 && digitalRead(IR3)==1 && digitalRead(IR4)==1 && digitalRead(IR5)==1)
  khabay();
else if (digitalRead(IR1)==0&& digitalRead(IR2)==0 && digitalRead(IR3)==1 && digitalRead(IR4)==1 && digitalRead(IR5)==1)
  khabay();
else if (digitalRead(IR1)==0&& digitalRead(IR2)==1 && digitalRead(IR3)==1 && digitalRead(IR4)==1 && digitalRead(IR5)==1)
  khabay();
else if (digitalRead(IR1)==1&& digitalRead(IR2)==0 && digitalRead(IR3)==1 && digitalRead(IR4)==1 && digitalRead(IR5)==1)
  khabay();
else if (digitalRead(IR1)==1&& digitalRead(IR2)==1 && digitalRead(IR3)==1 && digitalRead(IR4)==0 && digitalRead(IR5)==1)
sajay();
else
  stopp();

}
void loop() {
  
  speedcheck();
  //loopi();
  //forward();
}

float voltage()// Voltage Function....
{
  value = analogRead(voltageSensor);
  vOUT = (value * 5.0) / 1024.0;
  vIN = vOUT / (R2/(R1+R2));
  return vIN;
}

float current() { //Current Function....
 
  average=0;
  average = average + (.044 * analogRead(currentSensor) -3.78); 
  return average;
}


//Car Directions/controls Function starts here....
void forward()
{
  analogWrite(13,100);
  analogWrite(12,0);
  analogWrite(11,100);
  analogWrite(10,0);
}

void sajay()
{
  analogWrite(13,0);
  analogWrite(12,100);
  analogWrite(11,100);
  analogWrite(10,0);
}
void khabay()
{
  analogWrite(13,100);
  analogWrite(12,0);
  analogWrite(11,0);
  analogWrite(10,100);
}
void stopp()
{
  analogWrite(13,0);
  analogWrite(12,0);
  analogWrite(11,0);
  analogWrite(10,0);
}

void speedcheck()
{
 start_time=millis();
 end_time=start_time+1000;
while(millis()<end_time)
 {
   car_control();
   displayy();
 }
    temp=steps-steps_old;
    steps_old=steps;
    RPM=(temp/20);
    RPM=RPM*60;
    //RPM=0;
}
