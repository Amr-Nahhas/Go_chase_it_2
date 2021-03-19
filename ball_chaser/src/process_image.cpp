#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

ros::ServiceClient client;

void move_robot_forward()
{

    ROS_INFO_STREAM("Moving robot forward");

    ball_chaser::DriveToTarget srv;
    srv.request.linear_x  = 0.5;
    srv.request.angular_z  = 0;

    if (!client.call(srv))
        ROS_ERROR("Failed to call service forward_move");
}

void stop_robot()
{
    ROS_INFO_STREAM("stop robot");

    ball_chaser::DriveToTarget srv;
    srv.request.linear_x  = 0;
    srv.request.angular_z  = 0;

    if (!client.call(srv))
        ROS_ERROR("Failed to call service forward_move");
}

void move_robot_right()
{
    ROS_INFO_STREAM("Moving robot right");

    ball_chaser::DriveToTarget srv;
    srv.request.linear_x  = 0;
    srv.request.angular_z  = -0.5;

    if (!client.call(srv))
        ROS_ERROR("Failed to call service forward_move");
}


void move_robot_left()
{
    ROS_INFO_STREAM("Moving robot left");

    ball_chaser::DriveToTarget srv;
    srv.request.linear_x  = 0;
    srv.request.angular_z  = 0.5;

    if (!client.call(srv))
        ROS_ERROR("Failed to call service forward_move");
}


void process_image_callback(const sensor_msgs::Image img)
{
  int white_pixel = 255;
  bool white_pixel_exist = false;
  int direction_definer=0;
  for (int i = 0; i < img.height * img.width * 3 ; i++) {
        if (img.data[i] - white_pixel == 0 & img.data[i+1] - white_pixel == 0 & img.data[i+2] - white_pixel == 0) {
          		  white_pixel_exist = true;
	  		  direction_definer=i % img.step;
          		  break;
        }
    }

  if (white_pixel_exist == true){
	if (direction_definer < img.step / 3){
        move_robot_left();}
	else if (direction_definer >= img.step /3 && direction_definer <= 2 * (img.step / 3)){
	move_robot_forward();}
	else{
	move_robot_right();}
    }
}

int main(int argc, char** argv)
{

    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    ros::spin();

    return 0;
}
