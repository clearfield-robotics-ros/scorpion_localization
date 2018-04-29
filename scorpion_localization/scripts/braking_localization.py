#!/usr/bin/env python

import rospy
import tf
from nav_msgs.msg import Odometry
from geometry_msgs.msg import PoseWithCovarianceStamped
from std_msgs.msg import Int16
from robot_localization.srv import SetPose

def update_brakes(data):
    global brake_flag
    brake_flag = data.data

def update_pose(data):
    global pose_to_set
    if (brake_flag == 0):
        pose_to_set.header        = data.header
        pose_to_set.pose          = data.pose
        #change time
        pose_to_set.header.stamp  = rospy.Time.now()
    else:
        pass

def main():
    global pose_to_set
    pose_to_set   = PoseWithCovarianceStamped()
    global brake_flag
    brake_flag    = 0

    rospy.init_node('braking_localization')
    rospy.Subscriber('/odometry/filtered', Odometry, update_pose)
    rospy.Subscriber('/braking_desired_state', Int16, update_brakes)
    rospy.wait_for_service('/set_pose')
    set_pose = rospy.ServiceProxy('/set_pose', SetPose)

    r           = rospy.Rate(30)
    while not rospy.is_shutdown():
        if (brake_flag == 1):
            # print("setting pose.")
            set_pose(pose_to_set)
        else:
            pass
        r.sleep()
    return

if __name__ == '__main__':
    main()