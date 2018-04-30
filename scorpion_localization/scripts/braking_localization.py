#!/usr/bin/env python

import rospy
import tf
from nav_msgs.msg import Odometry
from geometry_msgs.msg import PoseWithCovarianceStamped
from std_msgs.msg import Int16
from sensor_msgs.msg import NavSatFix
from robot_localization.srv import SetPose
import math
from geopy import Point
from geopy.distance import distance, VincentyDistance

def update_brakes(data):
    global past_brake_flag
    global brake_flag
    past_brake_flag     = brake_flag
    brake_flag          = data.data

    global pub_mine_flag
    if (past_brake_flag == 0 and brake_flag == 1):
        pub_mine_flag   = 1
    else:
        pub_mine_flag   = 0


def update_pose(data):
    global pose_to_set
    if (brake_flag == 0):
        pose_to_set.header        = data.header
        pose_to_set.pose          = data.pose
        #change time
        pose_to_set.header.stamp  = rospy.Time.now()
    else:
        pass


def update_gps(data):
    global gps_to_set
    gps_to_set = data


def get_mine_location():
    quat            = pose_to_set.pose.pose.orientation
    quat            = (quat.x, quat.y, quat.z, quat.w)
    euler           = tf.transformations.euler_from_quaternion(quat)
    #roll, pitch, yaw

    # R               = 6378.1
    bearing         = euler[2]
    d               = 0.00115 #km
    # distMiles       = 0.0007145768711 #miles
    lat1            = gps_to_set.latitude
    lon1            = gps_to_set.longitude
    # latlong         = VincentyDistance(kilometers=d).destination(Point(lat1, lon1), bearing).format_decimal()
    destination     = VincentyDistance(kilometers=d).destination(Point(lat1, lon1), bearing)
    
    global mine_gps
    mine_gps        = gps_to_set
    mine_gps.latitude   = destination.latitude
    mine_gps.longitude  = destination.longitude
    # print latlong[0]
    # print latlong[1]
    # print mine_gps.latitude
    # print mine_gps.longitude
    return


def main():
    global pose_to_set, gps_to_set
    pose_to_set     = PoseWithCovarianceStamped()
    gps_to_set      = NavSatFix()
    global brake_flag
    brake_flag      = 0
    global pub_mine_flag
    pub_mine_flag   = 0
    global mine_gps
    mine_gps        = NavSatFix()

    rospy.init_node('braking_localization')
    rospy.Subscriber('/odometry/filtered', Odometry, update_pose)
    rospy.Subscriber('/gps/filtered', NavSatFix, update_gps)
    rospy.Subscriber('/braking_desired_state', Int16, update_brakes)
    rospy.wait_for_service('/set_pose')
    set_pose = rospy.ServiceProxy('/set_pose', SetPose)

    mine_location_pub   = rospy.Publisher('/gps/mines', NavSatFix, queue_size=10)

    r           = rospy.Rate(30)
    while not rospy.is_shutdown():
        if (brake_flag == 1):
            set_pose(pose_to_set)
        if (pub_mine_flag == 1):
            # mine_location_pub.publish(gps_to_set) #this publishes the scorpion gps
            get_mine_location()
            mine_location_pub.publish(mine_gps) #this publises the mine after vincenty distance offset
            pub_mine_flag = 0
        r.sleep()
    return

if __name__ == '__main__':
    main()