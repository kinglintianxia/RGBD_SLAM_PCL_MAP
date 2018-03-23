#include "Viewer.h"
#include "Map.h"
#include "MapDrawer.h"
#include <iostream>
#include <chrono>

int main(int argc, char** argv)
{
    // Vocabulary
    ORB_SLAM2::ORBVocabulary voc;
    voc.loadFromBinaryFile("/home/king/Documents/king/ORB_SLAM2_MAP/Vocabulary/ORBvoc.bin");
    // setting file
    std::string setting_file;
    if (argc > 2)
        setting_file = std::string(argv[1]);
    else
        setting_file = "/home/king/Documents/king/ORB_SLAM2_MAP/Examples/Stereo/KITTI00-02.yaml";
    // Map
    ORB_SLAM2::Map map;
    std::string map_name;
    if (argc > 2)
        map_name = std::string(argv[2]);
    else
        map_name = "/home/king/Documents/king/ORB_SLAM2_MAP/map/kitti00_map.bin";
    // Load map ...
    ORB_SLAM2::SystemSetting* systemSetting = new ORB_SLAM2::SystemSetting(&voc);
    systemSetting->LoadSystemSetting(setting_file);
    std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();    // record time
    map.Load(map_name, systemSetting);
    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
    double tim = std::chrono::duration_cast<std::chrono::duration<double>> (t1-t0).count();
    std::cout << "Load map use: " << tim << std::endl;
    // viewer
    ORB_SLAM2::Viewer viewer(&map, setting_file);
    viewer.Run();
    return 0;
}


