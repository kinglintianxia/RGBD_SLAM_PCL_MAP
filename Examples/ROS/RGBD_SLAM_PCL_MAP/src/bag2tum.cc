/**
* This file is part of ORB-SLAM2.
* king@2018.05.11
* desp: rosbag to tum style png file
*/

#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>

#include<ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>

#include<opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

// king
#include <unistd.h>
#include<iomanip>

// # timestamp filename
std::ofstream f1("./image/rgb.txt");
std::ofstream f2("./image/depth.txt");

class ImageGrabber
{
public:
    ImageGrabber(){}

    void GrabRGBD(const sensor_msgs::ImageConstPtr& msgRGB,const sensor_msgs::ImageConstPtr& msgD);

};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "bag2img");
    ros::start();
    f1 << "# timestamp filename\n";
    f2 << "# timestamp filename\n";
//    if(argc != 3)
//    {
//        cerr << endl << "Usage: ORB_SLAM2_PCL_MAP bag2tum" << endl;
//        ros::shutdown();
//        return 1;
//    }

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    ImageGrabber igb;

    ros::NodeHandle nh;

    message_filters::Subscriber<sensor_msgs::Image> rgb_sub(nh, "/kinect2/qhd/image_color_rect", 1);
    message_filters::Subscriber<sensor_msgs::Image> depth_sub(nh, "/kinect2/qhd/image_depth_rect", 1);
    typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, sensor_msgs::Image> sync_pol;
    message_filters::Synchronizer<sync_pol> sync(sync_pol(10), rgb_sub,depth_sub);
    sync.registerCallback(boost::bind(&ImageGrabber::GrabRGBD,&igb,_1,_2));

    ros::spin();

    ros::shutdown();
    //
    f1.close();
    f2.close();
    return 0;
}

void ImageGrabber::GrabRGBD(const sensor_msgs::ImageConstPtr& msgRGB,const sensor_msgs::ImageConstPtr& msgD)
{
    // Copy the ros image message to cv::Mat.
    cv_bridge::CvImageConstPtr cv_ptrRGB;
    try
    {
        cv_ptrRGB = cv_bridge::toCvShare(msgRGB);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    cv_bridge::CvImageConstPtr cv_ptrD;
    try
    {
        cv_ptrD = cv_bridge::toCvShare(msgD);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    // record
    std::stringstream ss;
    ss << std::setiosflags(std::ios::fixed);  //只有在这项设置后，setprecision才是设置小数的位数。
    ss << std::setprecision(6);
    // rgb
    ss << cv_ptrRGB->header.stamp.toSec();
    std::string rgb_name = "rgb/" + ss.str() + ".png";
    f1 << ss.str() << " " << rgb_name << std::endl;
    // depth
    ss.str("");
    ss << cv_ptrD->header.stamp.toSec();
    std::string depth_name = "depth/" + ss.str() + ".png";
    f2 << ss.str() << " " << depth_name << std::endl;
    //
    rgb_name = "./image/" + rgb_name;
    depth_name = "./image/" + depth_name;
    std::cout << "rgb_name: " << rgb_name << std::endl;
    cv::imwrite(rgb_name, cv_ptrRGB->image);
    cv::imwrite(depth_name, cv_ptrD->image);

}


