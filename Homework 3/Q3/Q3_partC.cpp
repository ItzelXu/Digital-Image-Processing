#include <stdio.h>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/xfeatures2d.hpp"
#include <math.h>
#include <fstream>
#include <math.h>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

void readme();
int* calculate_l2(Mat descriptor, Mat vocabulary) {
    double *euclideanDist = new double[vocabulary.rows];
    double currentDist = 0.0;
    int index =0;
    int vocabRows = vocabulary.rows;

    int *frequency = new int[vocabRows];
    for (int i = 0; i < vocabRows;i++) {
        frequency[i] =0;
    }
    for (int i =0; i < descriptor.rows; i++) {
        for (int k =0; k < vocabulary.rows;k++) {
            for (int j = 0; j < descriptor.cols; j++) {
                double intDiff = (descriptor.at<float>(i,j)-vocabulary.at<float>(k,j));
                currentDist += pow(intDiff , 2);
            }
            euclideanDist[k] = (sqrt(currentDist));
            currentDist = 0.0;
        }
        double minDist = euclideanDist[0]; index = 0;

        for (int k = 1; k<vocabulary.rows; k++) {
            if (minDist>euclideanDist[k]) {
                minDist = euclideanDist[k];
                index = k;
            }
        }
        frequency[index] += 1;
    }
    return frequency;
}

void writetoFile(string path,int *vocab_img,int numRows) {
    ofstream arrayData;
    arrayData.open(path);
    if (arrayData.is_open()) {
        for (int i = 0; i<numRows; i++) {
            arrayData <<i<<"\t"<<vocab_img[i];
            arrayData << "\n";
        }
        arrayData.close();
    }
    else
    {
        cout << "unable to open file" << endl;
    }
}

double compare_codewords(int *vocab_img,int *ferarri2) {
    double error = 0.0;
    for (int j =0; j < 8;j++) {
        error += abs(vocab_img[j]-ferarri2[j]);
    }
    error =error/8.0;
    return error;
}

int main( int argc, char** argv )
{

    if( argc != 5 )
    { readme();
        return -1; }


    //read my images

    Mat img_1 = imread( argv[3], CV_LOAD_IMAGE_COLOR );//Ferrari_1
    Mat img_2 = imread( argv[4], CV_LOAD_IMAGE_COLOR );//Ferrari_2
    Mat img_3 = imread( argv[1], CV_LOAD_IMAGE_COLOR  );//Optimus prime
    Mat img_4 = imread( argv[2], CV_LOAD_IMAGE_COLOR  );//Bumblebee

    if( !img_1.data || !img_2.data || !img_3.data || !img_4.data)
    {
        std::cout<< " Images not found. Check the path for the images " << std::endl;
        return -1;
    }


    //sift.compute() which computes the descriptors from the keypoints

    int minHessian = 400;


    Ptr<SIFT> Siftdetector = SIFT::create(minHessian);

    std::vector<KeyPoint> keypoints_1, keypoints_2;
    Mat descriptors_1, descriptors_2;
    Mat image_keypoints_1, image_keypoints_2;

    Siftdetector->detect(img_1, keypoints_1);
    Siftdetector->detect(img_2, keypoints_2);


    //Now find the descriptors
    Siftdetector->compute(img_1, keypoints_1, descriptors_1);
    Siftdetector->compute(img_2, keypoints_2, descriptors_2);

    ////Now for Bag of words you need all the sift features of all the images

    //////pre - BAG OF WORDS//////////////////////////////////////////////////////
    vector<KeyPoint> keypoints_3, keypoints_4;
    Mat img_keypoints_3; Mat img_keypoints_4;
    Mat descriptors_3, descriptors_4;

    Siftdetector->detect(img_3, keypoints_3);
    Siftdetector->detect(img_4, keypoints_4);

    Siftdetector->compute(img_3, keypoints_3, descriptors_3);
    Siftdetector->compute(img_4, keypoints_4, descriptors_4);

    ////// BAG OF WORDS//////////////////////////////////////////////////////
    BOWKMeansTrainer BGOWORDS(8);  //you need 8 clusters
    BGOWORDS.add(descriptors_1);   //adds descriptors to a training set
    BGOWORDS.add(descriptors_3);
    BGOWORDS.add(descriptors_4);

    Mat vocabulary = BGOWORDS.cluster(); //k means clustering 

    //in vocabulary each row will be a descriptor belonging to that image number

    // declare variables
    int *vocab_img1 = new int[vocabulary.rows];
    int *vocab_img2 = new int[vocabulary.rows];
    int *vocab_img3 = new int[vocabulary.rows];
    int *vocab_img4 = new int[vocabulary.rows];

    vocab_img1 = calculate_l2(descriptors_1,vocabulary);
    vocab_img2 = calculate_l2(descriptors_2, vocabulary);
    vocab_img3 = calculate_l2(descriptors_3, vocabulary);
    vocab_img4 = calculate_l2(descriptors_4, vocabulary);

    writetoFile("/Users/deepikakanade/Documents/histCount1.txt", vocab_img1, 8);
    writetoFile("/Users/deepikakanade/Documents/histCount2.txt", vocab_img2, 8);
    writetoFile("/Users/deepikakanade/Documents/histCount3.txt", vocab_img3, 8);
    writetoFile("/Users/deepikakanade/Documents/histCount4.txt", vocab_img4, 8);


    //Now compare ferrari 2 codewords with others

    double error1 = compare_codewords(vocab_img1,vocab_img2);
    double error2 = compare_codewords(vocab_img3, vocab_img2);
    double error3 = compare_codewords(vocab_img4, vocab_img2);
    cout<<endl;
    cout << "Error with respect to Ferrari 1 is: "<<error1 << endl;
    cout << "Error with respect to Optimus Prime is: "<<error2 << endl;
    cout << "Error with respect to Bumblebee is: "<<error3 << endl;

    double minimum;
    double min1=min(error1,error2);
    double min2=min(min1,error3);
    minimum=min2;

    cout<<endl<<"Final Conclusion is: ";
    if (minimum==error1)
        cout<<"Ferrari 2 is close to Ferrari 1";
    if (minimum==error2)
        cout<<"Ferrari 2 is close to Optimus Prime";
    if (minimum==error3)
        cout<<"Ferrari 2 is close to Bumblebee";
    waitKey();

    return 0 ;

}


void readme()
{ std::cout << " Usage: ./SURF_FlannMatcher <img1> <img2>" << std::endl; }
