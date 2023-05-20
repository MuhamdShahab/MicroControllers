#include <Astar_swarm.h>
#include <gyro_odo.h>
#include <ArduinoJson.h>


// Core definitions (assuming you have dual-core ESP32)
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

const int COL = (480/30);
const int ROW = (120/30);

const int ENCR = 32;// YELLOW-
const int ENCR1 = 33; // GREEN-
const int ENCL = 34; // YELLOW-
const int ENCL1 = 35; //GREEN-

const int RM = 13; // pwm right motor-
const int LM = 25; // pwm left motor-

// the number of the Motor Pins
const int LM1 = 26; //Left motor 1st terminal-
const int LM2 = 27;  //Left motor 2nd terminal-
const int RM1 = 12; //Right motor 1st terminal-
const int RM2 = 14;  //Right motor 2nd terminal-

//Motor Channel
const int RM_ch = 0; //PWM channel--timer based-
const int LM_ch = 1; //PWM channel--timer based-

const int first_led = 15; //Left motor 1st terminal-
const int second_led = 2;  //Left motor 2nd terminal-
const int third_led = 0; //Right motor 1st terminal-
const int fourth_led = 4;  //Right motor 2nd terminal-
const int fifth_led = 5; //Left motor 1st terminal-  

agent Agent(0,0,'A');

#define RXD2 16
#define TXD2 17


void move_straight(void *parameters) 
{
  while (1) 
  {
//    int val = Agent.motion(30);
//    if(val == 0)
//    {
//      Serial.println("Error: Could'nt Move in Straight Line Sorry. Try Again");
//    }
//    else if(val == 1)
//    {
//      Serial.println("Success: Desired Location Reached.");
//    }
//    else
//    {
//      Serial.println("Failure: Could'nt Reach the Desired Distance.");
//    }
  }
}

int gx[5] = {1,4,9,11,14};
int gy[5] = {3,2,3,0,2};


void take_turn(void *parameters) 
{

 
  while (1) 
  {
    Agent.take_turn(90);
  }
}


int origin_x = 0;
int origin_y = 0;
int start_x = origin_x;
int start_y = origin_y;
int init_goal_x = 1;
int init_goal_y = 3;
int goal_x = 1;
int goal_y = 3;
int left_lim = 0;
int right_lim = 16;
int current_angle = 0;
int timp = 0;
float onset = 0.0;
bool onces = true;
float zero_offset = 7.3;

int** my_obstacles(int** arr)
{
  arr = place_obstacle(arr,2,2);
  arr = place_obstacle(arr,4,1);
  arr = place_obstacle(arr,8,1);
  arr = place_obstacle(arr,13,2);
  return arr;
}

void A_star(void *parameters) 
{ 
    delay(15000);
  while (1) 
  {
    if(onces)
    {
      int ** arena =  getmap(1,COL,ROW); //generate first param map like 1.
      arena = my_obstacles(arena);//place your obstacles
      astar_result apath = Astar(start_x,start_y,goal_x,goal_y,arena); //gives a 2D array of feasible path with number of coordinates
      Serial.println("A-star found the following path: "); printmap(apath.getpath(),apath.getsize(),2); //prints the coordinates with getsize number of columns and 2(x and y cordinates)
      
      Serial.print("We have to traverse the following number of coordinates: "); Serial.println(apath.getsize());
      if(apath.getsuccess() == true)
      {
        for(int i = 0; i < apath.getsize(); i++)
        {
          if(apath.getpath()[0][i]== goal_x && apath.getpath()[1][i] == goal_y)
          {
           Serial.println("Goal Reached by Robot !!!!!!!!");  
          }
          else
          {
            if(apath.getpath()[0][i] == apath.getpath()[0][i+1] && apath.getpath()[1][i+1] == (apath.getpath()[1][i] + 1)) // F
            {
              current_angle = 0 - current_angle;
              Serial.print("Take Turn for "); Serial.print(current_angle);
              Serial.println(" & Move Distance of 30 cm");
              if(current_angle == 0)
              {
                onset = 0;
                //skip turning;
              }
              else
              {
                Agent.take_turn(current_angle);
              }
              Agent.motion(30 - onset);
            }
            else if((apath.getpath()[0][i+1] == apath.getpath()[0][i] + 1) && (apath.getpath()[1][i+1] == (apath.getpath()[1][i] + 1))) // FL
            {
              current_angle = 45 - current_angle;
              Serial.print("Take Turn for ");Serial.print(current_angle);
              Serial.println(" & Move Distance of 42.42 cm");
              if(current_angle == 0)
              {
                onset = 3.238;
                //skip turning;
              }
              else
              {
                Agent.take_turn(current_angle);
              }
              Agent.motion(42.42 - onset);
            }
            else if((apath.getpath()[0][i+1] == apath.getpath()[0][i] - 1) && (apath.getpath()[1][i+1] == (apath.getpath()[1][i] + 1))) // FR
            {
              current_angle = -45 - current_angle;
              Serial.print("Take Turn for ");Serial.print(current_angle);
              Serial.println(" & Move Distance of 42.42 cm");
              if(current_angle == 0)
              {
                onset = 3.228;
                //skip turning;
              }
              else
              {
                Agent.take_turn(current_angle);
              }
              Agent.motion(42.42 - onset);
            }
            else if((apath.getpath()[0][i+1] == apath.getpath()[0][i] - 1) && (apath.getpath()[1][i+1] == (apath.getpath()[1][i]))) // L
            {
              current_angle = -90 - current_angle;
              Serial.print("Take Turn for ");Serial.print(current_angle);
              Serial.println(" & Move Distance of 30 cm");
              if(current_angle == 0)
              {
                onset = 7.3;
                //skip turning;
              }
              else
              {
                Agent.take_turn(current_angle);
              }
              Agent.motion(30 - onset);
            }
            else if((apath.getpath()[0][i+1] == apath.getpath()[0][i] + 1) && (apath.getpath()[1][i+1] == (apath.getpath()[1][i]))) // R
            {
              onset = 7.3;
              current_angle = 90 - current_angle;
              Serial.print("Take Turn for ");Serial.print(current_angle);
              Serial.println(" & Move Distance of 30 cm");
              if(current_angle == 0)
              {
                onset = 0;
                //skip turning;
              }
              else
              {
                Agent.take_turn(current_angle);
              }
              Agent.motion(30 - onset);
            }
            else if((apath.getpath()[0][i+1] == apath.getpath()[0][i] + 1) && (apath.getpath()[1][i+1] == (apath.getpath()[1][i]-1))) // BL 
            {
              current_angle = 135 - current_angle;
              Serial.print("Take Turn for ");Serial.print(current_angle);
             Serial.println(" & Move Distance of 42.42 cm");
              if(current_angle == 0)
              {
                onset = 0;
                //skip turning;
              }
              else
              {
                Agent.take_turn(current_angle);
              }
              Agent.motion(42.42 - onset);
            }
            else if((apath.getpath()[0][i] == apath.getpath()[0][i+1]) && (apath.getpath()[1][i+1] == (apath.getpath()[1][i]-1))) // B
            {
              current_angle = 180 - current_angle;
              Serial.print("Take Turn for ");Serial.print(current_angle);
              Serial.println(" & Move Distance of 30 cm");
              if(current_angle == 0)
              {
                onset = 0;
                //skip turning;
              }
              else
              {
                Agent.take_turn(current_angle);
              }
              Agent.motion(30 - onset);
            }
            else if((apath.getpath()[0][i+1] == apath.getpath()[0][i] -1) && (apath.getpath()[1][i+1] == (apath.getpath()[1][i]-1))) // BR
            {
              current_angle = -135 - current_angle;
              Serial.print("Take Turn for ");Serial.print(current_angle);
              Serial.println(" & Move Distance of 42.42 cm");
              if(current_angle == 0)
              {
                onset = 0;
                //skip turning;
              }
              else
              {
                Agent.take_turn(current_angle);
              }
              Agent.motion(42.42 - onset);
            }
            else
            {
              Serial.println("Boom !!! Unknown error");
            }
          }
        }  
      }
      else
      {
        Serial.println("No Path Found"); 
      }
      onces = false;
    }
    else
    {
      Serial.println("Waiting for the New Goal point.");
      vTaskDelay(5000/portTICK_PERIOD_MS);
    }
  }
}


//void serial_rec()
//{
//if (Serial2.available() >= 1) {
//    //    digitalWrite(LED_pin, HIGH);
//    Serial.println("Data Received...");
//    String incoming = Serial2.readStringUntil('\n');
//    Serial.println(incoming);
//    //    digitalWrite(LED_pin, LOW);
//  }
// else
// {
//  Serial.println("No data Available"); 
// }
//}

void setup() 
{
  Serial.begin(115200);
  Serial.println("================ ALPHA ================");

  Agent.setup_motors();
  bool gyr = Agent.gyro_detection();  
  bool motr = Agent.motors_detection();    
  
  if(1)
  {
     //Both Found Create the task
//    xTaskCreatePinnedToCore(move_straight, 
//                            "move_straight",
//                            1200, 
//                            NULL,
//                            1,
//                            NULL,
//                            app_cpu);
//    //gyro and motors both found
//    xTaskCreatePinnedToCore(take_turn,
//                            "take_turn",
//                            1200,
//                            NULL,
//                            2,
//                            NULL,
//                            app_cpu);
    xTaskCreatePinnedToCore(A_star,
                            "A_star",
                            1500,
                            NULL,
                            3,
                            NULL,
                            app_cpu);

  delay(10000);
                            
  }
  else
  {
   delay(10000);
    //any one is missing robot is meaning less.
  }

//  Serial.println("Serial Cummunication Enabled");
//  Serial,begin(9600,SERIAL_8N1, RXD2, TXD2);
//  Serial.println("Let's Start");

  //Agent.debug_ticks();

//  Agent.motion(30);
//  Agent.take_turn(180);
//  Agent.take_turn(-90);
//  Agent.motion(7.3);
//  Agent.motion(30);
//   //delay(5000);
//  Agent.take_turn(45);
//   // delay(5000);
//  Agent.motion(30);
//   // delay(5000);
//  Agent.take_turn(-45);
//   // delay(5000);
//  Agent.motion(30);
//   // delay(5000);
//  Agent.take_turn(90);
//   // delay(5000);
//  Agent.motion(30);
//  //  delay(5000);
//  Agent.take_turn(-90);
   // delay(5000);
  vTaskDelete(NULL);
  
}

void loop() 
{
}
