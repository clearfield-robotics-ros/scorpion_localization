#!/usr/bin/env python

import rospy
import tf
from nav_msgs.msg import Odometry

def publish_world(data):
	global br
	br.sendTransform((data.pose.pose.position.x*1000.0, data.pose.pose.position.y*1000.0, data.pose.pose.position.z*1000.0),
            			(data.pose.pose.orientation.x, data.pose.pose.orientation.y, data.pose.pose.orientation.z, data.pose.pose.orientation.w),
            			rospy.Time(0),
            			"base_link",
            			"world")

def main():
	rospy.init_node('odom2world')
	rospy.Subscriber('odometry_filtered', Odometry, publish_world)
	global br
	br  		= tf.TransformBroadcaster()
	# listener  	= tf.TransformListener()
	r 			= rospy.Rate(30)

	# trans = [0.0, 0.0, 0.0]
	# rot   = [0.0, 0.0, 0.0, 1.0]

	while not rospy.is_shutdown():
		#TODO
		#subscirber to odometry msg and publish tf
		pass

 		# try:
 		# 	(trans,rot) = listener.lookupTransform('odom', 'base_link', rospy.Time(0))
 		# 	br.sendTransform((0.0, 0.0, 0.0),
 		# 					(0.0, 0.0, 0.0, 1.0),
 		# 					rospy.Time.now(),
 		# 					"odom",
 		# 					"world")
 		# 	br.sendTransform((trans[0]*1000.0,
   #                      trans[1]*1000.0,
   #                      trans[2]*1000.0),
   #          			rot,
   #          			rospy.Time(0),
   #          			"base_link",
   #          			"world")
 		# except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):
 			# continue

        # br.sendTransform((trans[0]*1000.0,
        #                 trans[1]*1000.0,
        #                 trans[2]*1000.0),
        #     			rot,
        #     			rospy.Time(0),
        #     			"base_link",
            			# "world")
        # br.sendTransform((0,0,0),tf.transformations.quaternion_from_euler(0,0,0),rospy.Time(0),"base_link", "test")

        # br.sendTransform((0,
        #                 0,
        #                 0),
        #     tf.transformations.quaternion_from_euler(0,
        #                                             0,
        #                                             0),
        #     rospy.Time.now(),
        #     "md",
        #     "sensor_head")


    	r.sleep()
	return

if __name__ == '__main__':
	main()