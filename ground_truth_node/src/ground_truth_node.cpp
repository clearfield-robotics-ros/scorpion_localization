#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include "sensor_msgs/NavSatFix.h"

int currentSeq = 0;
double avgLong, avgLat, avgAlt;

void fixCallback(const sensor_msgs::NavSatFixConstPtr msg)
{
	avgLat 	= ((avgLat * currentSeq) + msg->latitude) / (currentSeq + 1);
	avgLong = ((avgLong * currentSeq) + msg->longitude) / (currentSeq + 1);
	avgAlt 	= ((avgAlt * currentSeq) + msg->altitude) / (currentSeq + 1);

	currentSeq++;
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "ground_truth_node");
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("fix", 100, fixCallback);
	ros::Publisher avgPub = n.advertise<sensor_msgs::NavSatFix>("average/fix",10);
	ros::Rate loop_rate(5);

	while (ros::ok())
	{
		sensor_msgs::NavSatFix avgFix;
		avgFix.latitude 	= avgLat;
		avgFix.longitude 	= avgLong;
		avgFix.altitude 	= avgAlt;

		avgPub.publish(avgFix);
		ros::spinOnce();
		loop_rate.sleep();
	}

	return 0;
}