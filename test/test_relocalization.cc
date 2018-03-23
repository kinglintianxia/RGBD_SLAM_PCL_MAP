#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>

#include<opencv2/core/core.hpp>

#include "System.h"
#include "PangolinViewer.h"
#include "Frame.h"
#include "ORBextractor.h"
#include "Camera.h"
//#include "Initializer.h"
#include "SystemSetting.h"

using namespace std;

void LoadImages(const string &strAssociationFilename, vector<string> &vstrImageFilenamesRGB,
                vector<string> &vstrImageFilenamesD, vector<double> &vTimestamps)
{
    ifstream fAssociation;
    fAssociation.open(strAssociationFilename.c_str());
    while(!fAssociation.eof())
    {
        string s;
        getline(fAssociation,s);
        if(!s.empty())
        {
            stringstream ss;
            ss << s;
            double t;
            string sRGB, sD;
            ss >> t;
            vTimestamps.push_back(t);
            ss >> sRGB;
            vstrImageFilenamesRGB.push_back(sRGB);
            ss >> t;
            ss >> sD;
            vstrImageFilenamesD.push_back(sD);

        }
    }
}

int main(int argc, char **argv)
{

    const string voc_file = "/home/king/Documents/king/ORB_SLAM2_MAP/Vocabulary/ORBvoc.bin";
	ORB_SLAM2::ORBVocabulary voc;
	voc.loadFromBinaryFile(voc_file);

# if 1	
    const string settings_file = "/home/king/Documents/king/ORB_SLAM2_MAP/Examples/RGB-D/TUM3.yaml";
    const string map_file = "/home/king/Documents/king/ORB_SLAM2_MAP/tum3_map.bin";
    const string img_dir = "/home/king/Documents/image/rgbd_dataset_freiburg3_long_office_household";
#else
	const string settings_file = "/data/slam/amani/3/settings.yaml";
	const string map_file = "/tmp/enac_03.map.bin";
	//const string img_dir = "/data/slam/amani/3/2016_02_19_15_19_50.img/";
	const string img_dir = "/data/slam/amani/3/2016_02_19_15_37_13.img/";
#endif

    // Retrieve paths to images
    vector<string> vstrImageFilenamesRGB;
    vector<string> vstrImageFilenamesD;
    vector<double> vTimestamps;
    string strAssociationFilename = "/home/king/Documents/image/rgbd_dataset_freiburg3_long_office_household/associations.txt";
    LoadImages(strAssociationFilename, vstrImageFilenamesRGB, vstrImageFilenamesD, vTimestamps);

//	ORB_SLAM2::Camera::Load(settings_file);
    ORB_SLAM2::SystemSetting* mSystemSetting = new ORB_SLAM2::SystemSetting(&voc);
    mSystemSetting->LoadSystemSetting(settings_file);
//    mpMap->Load(map_file, mSystemSetting);
    ORB_SLAM2::Map map;
    map.Load(map_file, mSystemSetting);

    int nFeatures = 2000; float fScaleFactor = 1.2;
	int nLevels = 8, fIniThFAST = 20, fMinThFAST = 7;
	ORB_SLAM2::ORBextractor extractor(nFeatures,fScaleFactor,nLevels,fIniThFAST,fMinThFAST);

    ORB_SLAM2::KeyFrameDatabase kfdb(voc);
	for(auto kf: map.GetAllKeyFrames())
	  kfdb.add(kf);

    //
    cv::FileStorage fSettings(settings_file, cv::FileStorage::READ);
    float fx = fSettings["Camera.fx"];
    float fy = fSettings["Camera.fy"];
    float cx = fSettings["Camera.cx"];
    float cy = fSettings["Camera.cy"];

    cv::Mat K = cv::Mat::eye(3,3,CV_32F);
    K.at<float>(0,0) = fx;
    K.at<float>(1,1) = fy;
    K.at<float>(0,2) = cx;
    K.at<float>(1,2) = cy;
//    K.copyTo(mK);

    cv::Mat DistCoef(4,1,CV_32F);
    DistCoef.at<float>(0) = fSettings["Camera.k1"];
    DistCoef.at<float>(1) = fSettings["Camera.k2"];
    DistCoef.at<float>(2) = fSettings["Camera.p1"];
    DistCoef.at<float>(3) = fSettings["Camera.p2"];
    const float k3 = fSettings["Camera.k3"];
    if(k3!=0)
    {
        DistCoef.resize(5);
        DistCoef.at<float>(4) = k3;
    }
//    DistCoef.copyTo(mDistCoef);

//    mbf = fSettings["Camera.bf"];


    //
//	double timestamp = 0.;
    float ThDepth = 40.0;
    float bf = 40.0;

    for (int i=0; i<1000; i++)
    {
        // Read image and depthmap from file
        cv::Mat imRGB = cv::imread(img_dir +"/"+vstrImageFilenamesRGB[i],CV_LOAD_IMAGE_UNCHANGED);
        cv::Mat imD = cv::imread(img_dir +"/"+vstrImageFilenamesD[i],CV_LOAD_IMAGE_UNCHANGED);
        double tframe = vTimestamps[i];
        if(imRGB.empty())
        {
            cerr << endl << "Failed to load image at: "
                 << string(argv[3]) << "/" << vstrImageFilenamesRGB[i] << endl;
            return 1;
        }
//        cv::imshow("img_i:", imRGB);
//        cv::waitKey(100);
        //      ORB_SLAM2::Frame f(img, timestamp, &extractor, &voc, ThDepth);
        ORB_SLAM2::Frame f(imRGB,imD,tframe,&extractor,&voc,K,DistCoef,bf,ThDepth);
        f.ComputeBoW();
        vector<ORB_SLAM2::KeyFrame*> cand = kfdb.DetectRelocalizationCandidates(&f);
        cerr << i << " " << cand.size() << " (";
        for (auto cf: cand)
            cerr << cf->mnId << " ";
        cerr << ")" << endl;
//        cv::destroyAllWindows();
	}

	return 0;
}
