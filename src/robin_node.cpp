#include "ros/ros.h"
#include "robin/robin_publisher.h"
#include "robin/robin_subscriber.h"
#include "robin/structs.h"
#include "robin/TestStruct_foo.h"
#include "std_msgs/Float64.h"
int main(int argc, char **argv)
{
  ros::init(argc, argv, "robin");
  RobinPublisher<double, std_msgs::Float64> double_to_ros("double_to_ros");
  RobinPublisher<TestStruct_foo, robin::TestStruct_foo> struct_to_ros_1("struct_to_ros_1");
  RobinSubscriber<double, std_msgs::Float64> double_to_codesys("double_to_codesys");
  RobinSubscriber<TestStruct_foo, robin::TestStruct_foo> struct_to_codesys_1("struct_to_codesys_1");
  ros::spin();
  return 0;
}
