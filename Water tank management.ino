#include <NewPing.h>
int check=0;

void setup() 
{
  Serial.begin(9600);
  pinMode(8,OUTPUT);
  //use other pin as ground.
}

class water_level
{
  private:
    float avg;
    float percentlevel;
  public:
    water_level(int TPIN,int EPIN,float UMAX) // Constructor declaration
    {
      avg=0;
      NewPing sonar(TPIN, EPIN, UMAX);
      for(int i=0; i<=10;i++)
      {
       float temp= sonar.ping_cm();
       while(temp<=1)
          temp= sonar.ping_cm();
       avg+=temp;
      }
       avg/=10;
    }
    float getavg()
    {
      return avg;
    }
    float getpercentlevel(float tank,float off)
    {
      float percent=tank-getavg()-off;
      percent*=100;
      percent/=tank;
      return percent;
    }
};

void loop() 
{
  //for reservior-->ground wala
  water_level ground=water_level(3,2,200);//Trigpin, Echopin,Max distance
  float low=ground.getpercentlevel(200,0);
  Serial.println(low);//tank height, offset from top


  //for roof tanks create more objects in case of mutiple tanks
  //water_level roof1=water_level(5,4,200);//Trigpin, Echopin,Max distance
  //float up=roof1.getpercentlevel(200,0);
  //Serial.println(up);//tank height, offset from top
  
  //motor(up,low);
}



void motor(float up, float low)
{
if(up>=90)
 {
    check=0;
    digitalWrite(8, LOW);
    Serial.println("MOTOR OFF");
 }
else if(check==1 && up<=90 && up>=30)
 {
    if(low>=10)
    {
   digitalWrite(8, HIGH);
  Serial.println("MOTOR ON");
    }
    else
    {
   digitalWrite(8, LOW);
  Serial.println("MOTOR OFF");
    }
    
 }
 else if(up<=30)
 {
    if(low>=10)
    {
   digitalWrite(8, HIGH);
  Serial.println("MOTOR ON");
    }
    else
    {
   digitalWrite(8, LOW);
 Serial.println("MOTOR OFF");
    }
    check=1;
 }
 else
 {
    digitalWrite(8, LOW);
    Serial.println("MOTOR OFF");
 }
}
