
#include <stdio.h>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/xfeatures2d.hpp"

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

void readme();

int main( int argc, char** argv )
{
    if( argc != 3 )
    { readme();
        return -1; }

    Mat img_1 = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    Mat img_2 = imread( argv[2], CV_LOAD_IMAGE_GRAYSCALE);

    if( !img_1.data || !img_2.data )
    {
        std::cout<< " Images not found. Check the path for the images " << std::endl;
        return -1;
    }

    //-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
    int minHessian = 300;

    //Ptr<SURF> detector = SURF::create(minHessian);
    Ptr<SIFT> detector = SIFT::create();

    std::vector<KeyPoint> keypoints_1, keypoints_2;
    Mat descriptors_1, descriptors_2;

    detector->detectAndCompute( img_1, Mat(), keypoints_1, descriptors_1 );
    detector->detectAndCompute( img_2, Mat(), keypoints_2, descriptors_2 );

    //Brute Force Matcher
    BFMatcher matcher;
    vector<DMatch> matches;
    matcher.match(descriptors_1,descriptors_2,matches);
    Mat matchDespcriptor;
    double max_dist = 0; double min_dist = 100;
    //-- Quick calculation of max and min distances between keypoints
    for( int i = 0; i < descriptors_1.rows; i++ )
    {
        double dist = matches[i].distance;
        if( dist < min_dist )
            min_dist = dist;
        if( dist > max_dist )
            max_dist = dist;
    }

    //-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
    //-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
    //-- small)

    std::vector< DMatch > good_matches;
    for( int i = 0; i < descriptors_1.rows; i++ )
    {
        if( matches[i].distance <= max(2*min_dist, 0.2) )
        {
            good_matches.push_back( matches[i]);
        }
    }
    drawMatches(img_1,keypoints_1,img_2,keypoints_2,good_matches,matchDespcriptor);
    imwrite("/Users/deepikakanade/Desktop/Goodmatches_SiftBF400algo.jpg",matchDespcriptor);


    //-- Step 2: Matching descriptor vectors using FLANN matcher
//    FlannBasedMatcher matcher;
//    std::vector< DMatch > matches;
//    matcher.match( descriptors_1, descriptors_2, matches );matches

    //-- Show detected matches
    imwrite( "/Users/deepikakanade/Desktop/Goodmatches_SiftFl500.jpg", img_matches );

    return 0;
}


void readme()
{
    std::cout << " Usage: ./SURF_FlannMatcher <img1> <img2>" << std::endl;
}
