#include <stdlib.h>
#include <string.h>
#include <webots/motor.h>
#include <webots/range_finder.h>
#include <webots/robot.h>
#include <webots/utils/system.h>
#define SPEED 4
#define TIME_STEP 32


int main() {
  WbDeviceTag range_finder, left_motor, right_motor;
  int range_finder_width, range_finder_height;
  int left_speed, right_speed, i;
  float distance;

  wb_robot_init();

  range_finder = wb_robot_get_device("range finder");
  wb_range_finder_enable(range_finder, TIME_STEP);
  range_finder_width = wb_range_finder_get_width(range_finder);
  range_finder_height = wb_range_finder_get_height(range_finder);

  
  left_motor = wb_robot_get_device("leftmotor");
  right_motor = wb_robot_get_device("rightmotor");
  wb_motor_set_position(left_motor, INFINITY);
  wb_motor_set_position(right_motor, INFINITY);
  wb_motor_set_velocity(left_motor, 0.0);
  wb_motor_set_velocity(right_motor, 0.0);

  while (wb_robot_step(TIME_STEP) != -1) 
  {
 
    const float *image = wb_range_finder_get_range_image(range_finder);

 
    float left_distance = 100;
    float mid_distance = 100;
    float right_distance = 100;


    for (i = 0; i < range_finder_width; i++) 
    {
      distance = wb_range_finder_image_get_depth(image, range_finder_width, i, 40);
    
      if(i < range_finder_width/3)
      {
        if(distance < left_distance)
        {
          left_distance = distance;
        }
      }
      else if (i >= 2*(range_finder_width/3))
      {
        if(distance < right_distance)
        {
          right_distance = distance;
        }
      }
      else
      {
        if(distance < mid_distance)
        {
          mid_distance = distance;
        }
      }
      
    }

    if (mid_distance <= 0.05 && right_distance <= 0.05 && left_distance < 0.05)
    {
      left_speed = SPEED - left_distance;
      right_speed = SPEED - right_distance;
    
    }
    else if (right_distance < 0.08)
    {
      left_speed = SPEED - left_distance;
      right_speed = -(SPEED - right_distance);
    
    }
    else if (left_distance <= 0.08)
    {
      left_speed = -(SPEED - left_distance);
      right_speed = SPEED;
      
    }
    else if (mid_distance > 0.06 && right_distance > 0.05 && left_distance > 0.06)
    {
      left_speed = -(SPEED - left_distance);
      right_speed = -(SPEED - right_distance);

    }


    wb_motor_set_velocity(left_motor, left_speed);
    wb_motor_set_velocity(right_motor, -right_speed);
  }


  wb_robot_cleanup();

  return 0;
}
