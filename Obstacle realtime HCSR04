//Reach at https://www.linkedin.com/in/muhammad-shahab-2773a61a5/
//Github https://github.com/MuhamdShahab/
#include <NewPing.h>

#define F_TRIGGER_PIN  13  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define F_ECHO_PIN     12  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define L_TRIGGER_PIN  11  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define L_ECHO_PIN     10  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define R_TRIGGER_PIN  9  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define R_ECHO_PIN     8  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 250 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.


int ave=0;

int front()
{
int fdis=0;
 while(ave<2)
 {
   NewPing sonar(F_TRIGGER_PIN, F_ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance. 
   fdis+=sonar.ping_cm(); // Send ping, get distance in cm and print result (0 = outside set distance range)
   ave++;
    }
   fdis=fdis/ave;
   ave=0;
  // Serial.print("Straight: ");Serial.println(fdis);
   delay(20);
   return fdis;
}
int left()
{
int ldis=0;
 while(ave<2)
 {
   NewPing sonar(L_TRIGGER_PIN, L_ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance. 
   ldis+=sonar.ping_cm(); // Send ping, get distance in cm and print result (0 = outside set distance range)
   ave++;
    }
    ldis=ldis/ave;
    ave=0;
  // Serial.print("Left: ");Serial.println(ldis);
   delay(20);
   return ldis;
}
int right()
{
int rdis=0;
 while(ave<2)
 {
   NewPing sonar(R_TRIGGER_PIN, R_ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance. 
   rdis+=sonar.ping_cm(); // Send ping, get distance in cm and print result (0 = outside set distance range)
   ave++;
   }
   rdis=rdis/ave;
   ave=0;
  // Serial.print("Right: ");Serial.println(rdis);
   delay(20);
   return rdis;
 }

void orientation_right()
{
  
}
void orientation_left()
{
  
}
void motion(int fdiss, int ldiss, int rdiss )
{
    while(fdiss>rdiss && fdiss>ldiss)
    {
      forwardd();
       Serial.println("FRONT");
      motion(front(),left(),right());
 
    }
    while(rdiss>fdiss && rdiss>ldiss)
    {
      orientation_right();
       Serial.println("orientation_right");
      motion(front(),left(),right());
 
    }
    while(ldiss>fdiss && ldiss>rdiss)
    {
      orientation_left();
       Serial.println("orientation_left");
      motion(front(),left(),right());
    
    }
      stopp();
}

void setup() {
  Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.
  pinMode(7,OUTPUT);   //left motors forward
  pinMode(6,OUTPUT);   //left motors reverse
  pinMode(5,OUTPUT);   //right motors forward
  pinMode(4,OUTPUT);   //right motors reverse
}

void loop() 
{
motion(front(),left(),right());
}



//direction function starts hereeeeeee
void forwardd()
{
  digitalWrite(7,HIGH);
  digitalWrite(6,LOW);
  digitalWrite(5,HIGH);
  digitalWrite(4,LOW);
}
void backwardd()
{
  digitalWrite(7,LOW);
  digitalWrite(6,HIGH);
  digitalWrite(5,LOW);
  digitalWrite(4,HIGH);
}
void rightt()
{
  digitalWrite(7,LOW);
  digitalWrite(6,HIGH);
  digitalWrite(5,HIGH);
  digitalWrite(4,LOW);
}
void leftt()
{
  digitalWrite(7,HIGH);
  digitalWrite(6,LOW);
  digitalWrite(4,LOW);
  digitalWrite(4,HIGH); 
}
void stopp()
{
  digitalWrite(7,LOW);
  digitalWrite(6,LOW);
  digitalWrite(5,LOW);
  digitalWrite(4,LOW);
}
