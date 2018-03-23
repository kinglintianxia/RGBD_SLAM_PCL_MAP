# catkin_ws
export ROS_PACKAGE_PATH=${ROS_PACKAGE_PATH}:/home/king/Documents/king/ORB_SLAM2_PCL_MAP/Examples/ROS

echo "Building ROS nodes"

cd Examples/ROS/ORB_SLAM2_PCL_MAP
mkdir build
cd build
cmake .. -DROS_BUILD_TYPE=Release
make -j4
