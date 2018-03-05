#!/usr/bin/env python

import rospy
import numpy
from geometry_msgs.msg import Point
from std_msgs.msg import Int16
from std_msgs.msg import String
from visualization_msgs.msg import Marker
import tf
import math

def updateLocation(loc, rot):

	# send transform
    q_rot = tf.transformations.quaternion_from_euler(rot[0],rot[1],rot[2])
    br.sendTransform(loc,
         q_rot,
         rospy.Time.now(),
         "scorpion",
         "/odom")

    # send model to rviz
    m = Marker()
    m.header.frame_id = "/odom"
    m.header.seq = 0
    m.header.stamp = rospy.Time.now()
    m.ns = "scorpion"
    m.id = 0
    m.type = 10  # mesh
    m.mesh_resource = "package://scorpion_model/mesh/scorpion_master_0_001.STL";
    m.action = 0  # add/modify

    # scopion_rot = [rot[0] + model_scorpion_offset_rot[0],
    #     rot[1] + model_scorpion_offset_rot[1],
    #     rot[2] + model_scorpion_offset_rot[2]]

    # scopion_rot = [rot[0],
    #     rot[1],
        # rot[2]]
    e_rot = tf.transformations.euler_from_quaternion(rot);
    scopion_rot = [e_rot[0] + model_scorpion_offset_rot[0],
        e_rot[1] + model_scorpion_offset_rot[1],
        e_rot[2] + model_scorpion_offset_rot[2]]


    # scopion_q_rot = tf.transformations.quaternion_from_euler(scopion_rot[0],scopion_rot[1],scopion_rot[2])
    scorpion_q_rot = tf.transformations.quaternion_from_euler(scopion_rot[0], scopion_rot[1], scopion_rot[2]);
    # m.pose.position.x = loc[0]
    # m.pose.position.y = loc[1]
    # m.pose.position.z = loc[2]
    m.pose.orientation.x = scorpion_q_rot[0]
    m.pose.orientation.y = scorpion_q_rot[1]
    m.pose.orientation.z = scorpion_q_rot[2]
    m.pose.orientation.w = scorpion_q_rot[3]

    m.pose.position.x = loc[0] + model_scorpion_offset_loc[0]
    m.pose.position.y = loc[1] + model_scorpion_offset_loc[1]
    m.pose.position.z = loc[2] + model_scorpion_offset_loc[2]
    # m.pose.orientation.x = scopion_q_rot[0]
    # m.pose.orientation.y = scopion_q_rot[1]
    # m.pose.orientation.z = scopion_q_rot[2]
    # m.pose.orientation.w = scopion_q_rot[3]



    m.scale.x = 1
    m.scale.y = 1
    m.scale.z = 1
    m.color.a = 1.0
    m.color.r = 0.5
    m.color.g = 0.5
    m.color.b = 0.5
    pub.publish(m)

### pub / sub ###
pub = rospy.Publisher('scorpion', Marker, queue_size=10)

def main():
    rospy.init_node('scorpion_model')

    global br
    br = tf.TransformBroadcaster()
    listener = tf.TransformListener()
    
    global model_scorpion_offset_loc
    global model_scorpion_offset_rot
    model_scorpion_offset_loc = rospy.get_param('model_scorpion_offset_loc')
    model_scorpion_offset_rot = rospy.get_param('model_scorpion_offset_rot')

    r = rospy.Rate(10)  # 10 Hz
    while not rospy.is_shutdown():
        try:
            (loc,rot) = listener.lookupTransform('/odom', '/base_link', rospy.Time(0))
            updateLocation(loc,rot)
        except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):
            continue

        r.sleep()  # indent less when going back to regular gantry_lib

if __name__ == "__main__":
    main()
