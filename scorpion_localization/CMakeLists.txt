cmake_minimum_required(VERSION 2.8.3)
project(scorpion_localization)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  geometry_msgs
  tf
  sensor_msgs
  nav_msgs
  message_generation
)

include_directories(include
                    ${catkin_INCLUDE_DIRS} )

catkin_package(
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)
