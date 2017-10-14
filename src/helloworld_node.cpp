#include <ros/ros.h>
#include <sensor_msgs/Range.h>
#include <std_msgs/Int16.h>

void uslCallback(sensor_msgs::Range::ConstPtr uslMsg, sensor_msgs::Range* usl){
  *usl = *uslMsg;
}

void usfCallback(sensor_msgs::Range::ConstPtr usfMsg, sensor_msgs::Range* usf){
  *usf = *usfMsg;
}

void usrCallback(sensor_msgs::Range::ConstPtr usrMsg, sensor_msgs::Range* usr){
  *usr = *usrMsg;
}


int main(int argc, char **argv)
{
  // init this node
  ros::init(argc, argv, "helloworld_node");
  // get ros node handle
  ros::NodeHandle nh;

  // sensor message container
  sensor_msgs::Range usr, usf, usl;
  std_msgs::Int16 motor, steering;

  // generate subscriber for sensor messages
  ros::Subscriber usrSub = nh.subscribe<sensor_msgs::Range>("/uc_bridge/usr", 10, boost::bind(usrCallback, _1, &usr));
  ros::Subscriber uslSub = nh.subscribe<sensor_msgs::Range>("/uc_bridge/usl", 10, boost::bind(uslCallback, _1, &usl));
  ros::Subscriber usfSub = nh.subscribe<sensor_msgs::Range>("/uc_bridge/usf", 10, boost::bind(usfCallback, _1, &usf));

  // generate control message publisher
  ros::Publisher motorCtrl = nh.advertise<std_msgs::Int16>("/uc_bridge/set_motor_level_msg", 1);
  ros::Publisher steeringCtrl = nh.advertise<std_msgs::Int16>("/uc_bridge/set_steering_level_msg", 1);


  ROS_INFO("Hello world!");

  // Loop starts here:
  ros::Rate loop_rate(25);
  while (ros::ok())
  {
    if(usr.range<0.3 && usl.range>=0.3){
      steering.data = -750;
      motor.data = 300;
    }else if(usl.range<0.3 && usr.range>=0.3){
      steering.data = 750;
      motor.data = 300;
    }else if(usl.range>0.5 && usr.range>0.5){
      steering.data = 0;
      motor.data = 300;
    }else{
      steering.data = 0;
      motor.data = 0;
    }
    if(usf.range<0.3){
      motor.data = 0;
      steering.data = 0;
    }

    motorCtrl.publish(motor);
    steeringCtrl.publish(steering);

    ros::spinOnce();
    loop_rate.sleep();
  }

  ros::spin();
}
