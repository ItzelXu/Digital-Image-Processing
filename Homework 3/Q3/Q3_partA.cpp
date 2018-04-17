#include <stdio.h>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace cv::xfeatures2d;

void readme();
/** @function main */
int main( int argc, char** argv )
{
    if( argc != 3 )
    { readme(); return -1; }

    Mat img_1 = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    Mat img_2 = imread( argv[2], CV_LOAD_IMAGE_GRAYSCALE);

    if( !img_1.data || !img_2.data )
    { std::cout<< " --(!) Error reading images " << std::endl; return -1; }

    //-- Step 1: Detect the keypoints using SURF Detector
    int minHessian = 500;

    //Ptr<SURF> detector = SURF::create(minHessian);
    Ptr<SIFT> detector = SIFT::create( minHessian );

    std::vector<KeyPoint> keypoints_1, keypoints_2;

    detector->detect( img_1, keypoints_1 );
    detector->detect( img_2, keypoints_2 );

    //-- Draw keypoints
    Mat img_keypoints_1; Mat img_keypoints_2;

    drawKeypoints( img_1, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
    drawKeypoints( img_2, keypoints_2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT );

    //-- Show detected (drawn) keypoints
    imwrite("/Users/deepikakanade/Desktop/Sift_Image11.jpg", img_keypoints_1 );
    imwrite("/Users/deepikakanade/Desktop/Sift_Image21.jpg", img_keypoints_2 );

    waitKey();

    return 0 ;

}

/** @function readme */
void readme()
{ std::cout << " Usage: ./SURF_detector <img1> <img2>" << std::endl; }
