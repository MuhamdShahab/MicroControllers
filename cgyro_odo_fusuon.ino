bool once = true;
  while(once)
  {
    if(gyro)
    {
      float prev_angle = mpu6050.bhaluuZangle();
      Serial.print("  Previous Angle: ");
      Serial.print(mpu6050.bhaluuZangle());
      vTaskDelay(200/ portTICK_PERIOD_MS);
      Serial.print("  Current Angle: ");
      Serial.println(mpu6050.bhaluuZangle());
      if(mpu6050.bhaluuZangle() - prev_angle > 0)
      {
        left_pos = 1;
        if(console)
        {
        Serial.println("  Gyroscope: For Left turn Angle is Positive.");
        }
        else
        {}
      }
      else if(mpu6050.bhaluuZangle() - prev_angle < 0)
      {
        left_pos = 0;
        if(console)
        {
          Serial.println("  Gyroscope: For Left turn Angle is Negative.");
        }
        else
        {}
      }
      else
      {
        left_pos = 1; //usually turning left the angle is positive
        if(console)
        {
          Serial.println("  Warning: For Left turn Angle is Positive Generally.");
        }
        else
        {}
      }
    }
    else
    {
      Serial.println("  Gyroscope: Direction Check for Gyroscope not allowed.");
      vTaskDelay(200/ portTICK_PERIOD_MS);
    }
    once = false;
  }
