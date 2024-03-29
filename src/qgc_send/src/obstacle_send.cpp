
#include <iostream>
using namespace std;
#include <ros/ros.h>  
#include <ros/rate.h>

//some ros msg
//#include "sensor_msgs/Imu.h"
//#include "sensor_msgs/NavSatFix.h"
//#include "mavros_msgs/State.h"
//subscribe the State infomation
//#include "geometry_msgs/Vector3.h"
//#include "geometry_msgs/Quaternion.h"
//#include "../../mavros/mavros_msgs/msg/State.msg"

#include <mavconn/interface.h>
//#include <mavros/mavros.h>
#include <mavlink/v2.0/mavlink_helpers.h>
#include <mavconn/mavlink_dialect.h>


#include <mavros_msgs/Mavlink.h>
#include <mavros_msgs/mavlink_convert.h>




//#include <mavlink/v2.0/common/mavlink_msg_heartbeat.h>
//using namespace mavlink;
using namespace mavconn;
//using namespace mavlink;
int main(int argc, char **argv)  
{  
  ros::init(argc, argv, "qgc_test");
  ros::NodeHandle nh;

  ros::Publisher qgc_pub = nh.advertise<mavros_msgs::Mavlink>("/mavlink/from", 1000);
  ros::Rate loop_rate(10);


  //std::vector<long unsigned int> payload64(7, 0);
  //std::string gcs_url;
  //nh.param<std::string>("gcs_url",gcs_url,"/dev/ttyUSB0:57600");
  //MAVConnInterface::Ptr gcs_link;

  //gcs_link = MAVConnInterface::open_url(gcs_url);


  while(ros::ok())
  {
      

      //heartbeat

      uint8_t system_id = 1;

      uint8_t component_id = 1;
      uint8_t type = 1;
      uint8_t autopilot = 0;
      uint8_t base_mode = 128;
      uint32_t custom_mode = 0;
      uint8_t system_status = 0;

      mavlink::mavlink_message_t heart_beat;
      mavlink::MsgMap map1(heart_beat);
      mavlink::common::msg::HEARTBEAT msg_heartbeat;
      mavros_msgs::Mavlink mvl_heartbeat;
      msg_heartbeat.type = type;
      msg_heartbeat.autopilot = autopilot;
      msg_heartbeat.base_mode = base_mode;
      msg_heartbeat.custom_mode = custom_mode;
      msg_heartbeat.system_status = system_status;

      msg_heartbeat.serialize(map1);
      auto mi_1 = msg_heartbeat.get_message_info();
      mavlink::mavlink_status_t *status_1 = mavlink::mavlink_get_channel_status(mavlink::MAVLINK_COMM_0);
      status_1->flags |= MAVLINK_STATUS_FLAG_OUT_MAVLINK1;
      mavlink::mavlink_finalize_message_buffer(&heart_beat, 1, 1, status_1, mi_1.min_length, mi_1.length, mi_1.crc_extra);
      
      mavros_msgs::mavlink::convert(heart_beat, mvl_heartbeat);


      //my custom msg
      mavlink::mavlink_message_t obtacle_dir;
      mavlink::MsgMap map(obtacle_dir);
      mavlink::common::msg::TARGETDIRECTION obstacle_info;
      mavros_msgs::Mavlink mvl_obtacle;
      obstacle_info.front_center = int32_t(1000);
      obstacle_info.front_left = int32_t(1001);
      obstacle_info.front_right = int32_t(1002);
      obstacle_info.rear_left = int32_t(1003);
      obstacle_info.rear_right = int32_t(1110);
      obstacle_info.back = int32_t(1110);
      obstacle_info.ground = int32_t(1110);
      

      obstacle_info.serialize(map);

      auto mi = obstacle_info.get_message_info();
      mavlink::mavlink_status_t *status = mavlink::mavlink_get_channel_status(mavlink::MAVLINK_COMM_0);
      status->flags |= MAVLINK_STATUS_FLAG_OUT_MAVLINK1;
      mavlink::mavlink_finalize_message_buffer(&obtacle_dir, 1, 1, status, mi.min_length, mi.length, mi.crc_extra);
      
      mavros_msgs::mavlink::convert(obtacle_dir, mvl_obtacle);

     // heart_beat.reset();
     
    //mavros_msgs::mavlink::convert(msg_heartbeat, ml_heartbeat);
    
    //imageprocess imageprocess(argc, argv);
    //gcs_link->send_message_ignore_drop(msg_heartbeat);//link to P900     
    //qgc_pub.publish(mvl_heartbeat);
    qgc_pub.publish(mvl_obtacle);
    ROS_INFO("sending~~");
    ros::spinOnce();  
    loop_rate.sleep();
    
  }


  //image topic

}  
