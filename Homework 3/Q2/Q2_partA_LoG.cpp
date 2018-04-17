//
//  main.cpp
//  Assignment3_Q2aSobel
//
//  Created by Deepika Kanade on 3/14/18.
//  Copyright © 2018 Deepika Kanade. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <math.h>

using namespace std;


void get2Dchannel(unsigned char* Imagedata,int imageHeight, int imageWidth,unsigned char **rpixel2D, int iterationIndex)
{
    int rowIndex,colIndex;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            rpixel2D[rowIndex][colIndex]=Imagedata[iterationIndex];
            iterationIndex=iterationIndex+3;
        }
    }
}

void getRGBtoGrayscale(unsigned char** rpixel2D,unsigned char** gpixel2D,unsigned char** bpixel2D,unsigned char** luminosityArray,int imageHeight,int imageWidth)
{
    int rowIndex=0,colIndex=0;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            luminosityArray[rowIndex][colIndex]= (unsigned char) (0.21*rpixel2D[rowIndex][colIndex]+0.72*gpixel2D[rowIndex][colIndex]+0.07*bpixel2D[rowIndex][colIndex]);
        }
    }
}

void getGradient(unsigned char** luminosityArray,int imageHeight,int imageWidth,int** magnitude)
{
    int i,j;
    int n,x,p,q,LOG=0;
    int N=9;
    int sorted[N*N],LOGValue[N*N];
    //Defining LOG mask
    double maskArray[81]={0,1,1,2,2,2,1,1,0,1,2,4,5,5,5,4,2,1,1,4,5,3,0,3,5,4,1,2,5,3,-12,-24,-12,3,5,2,2,5,0,-24,-40,-24,0,5,2,2,5,3,-12,-24,-12,3,5,2,1,4,5,3,0,3,5,4,1,1,2,4,5,5,5,4,2,1,0,1,1,2,2,2,1,1,0};
    for(i=0;i<imageHeight;i++)
    {
        for(j=0;j<imageWidth;j++)
        {
            for(x=0;x<N*N;x++)
            {
                sorted[x]=0;
            }
            int k=0;
            for(p=i-(N-1)/2;p<=i+(N-1)/2;p++)
            {
                for(q=j-(N-1)/2;q<=j+(N-1)/2;q++)
                {
                    int rowIndex=p;
                    int colIndex=q;
                    if(rowIndex<0)
                    {
                        rowIndex=0;
                    }
                    if(rowIndex>imageHeight-1)
                    {
                        rowIndex=imageHeight-1;
                    }
                    if(colIndex<0)
                    {
                        colIndex=0;
                    }
                    if(colIndex>imageWidth-1)
                    {
                        colIndex=imageWidth-1;
                    }
                    sorted[k]=(int)luminosityArray[rowIndex][colIndex];
                    k++;
                }
            }
            LOG=0;
            for(n=0;n<N*N;n++)
            {
                LOGValue[n]=(int)maskArray[n]*sorted[n];
                LOG=LOG+LOGValue[n];
            }
            magnitude[i][j]=LOG;
        }
    }
}

void getnormalization(int** magnitude, int imageHeight,int imageWidth,int** normalizedArray)
{
    int rowIndex,colIndex;
    int max=magnitude[0][0];
    int min=magnitude[0][0];
    
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            if(max<magnitude[rowIndex][colIndex])
            {
                max=magnitude[rowIndex][colIndex];
            }
            if(min>magnitude[rowIndex][colIndex])
            {
                min=magnitude[rowIndex][colIndex];
            }
        }
    }
    //Normalizing values between 0 and 255
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            normalizedArray[rowIndex][colIndex]=(int)(255*((double)(magnitude[rowIndex][colIndex]-min)/(double)(max-min)));
        }
    }
}

void getintensity(int** normalizedArray,int imageHeight,int imageWidth,int* intensity)
{
    int k,rowIndex,colIndex;
    for(k=0;k<=255;k++)
    {
        for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
        {
            for(colIndex=0;colIndex<imageWidth;colIndex++)
            {
                if (normalizedArray[rowIndex][colIndex]==k)
                {
                    intensity[k]=intensity[k]+1;
                }
            }
        }
    }
}

void getthreshold(int** normalizedArray,int imageHeight,int imageWidth,int threshold,unsigned char** thresholdedImage,int** threethresholdedImage)
{
    int rowIndex,colIndex;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            if(normalizedArray[rowIndex][colIndex]<122)   //Generating a ternary map by using knee point selection method to decide the thresholds
            {
                thresholdedImage[rowIndex][colIndex]=64;
                threethresholdedImage[rowIndex][colIndex]=-1;
            }
            if(normalizedArray[rowIndex][colIndex]>=122 && normalizedArray[rowIndex][colIndex]<=156)
            {
                thresholdedImage[rowIndex][colIndex]=128;
                threethresholdedImage[rowIndex][colIndex]=0;
            }
            if(normalizedArray[rowIndex][colIndex]>156)
            {
                thresholdedImage[rowIndex][colIndex]=192;
                threethresholdedImage[rowIndex][colIndex]=1;
            }
        }
    }
}

void getZerocrossings(int** thresholdedImage,int imageHeight,int imageWidth,unsigned char** outputImage)
{
    int i,j;
    int x,p,q;
    int N=3;
    int sorted[N*N];
    for(i=0;i<imageHeight;i++)
    {
        for(j=0;j<imageWidth;j++)
        {
            if(thresholdedImage[i][j]==0)
            {
            for(x=0;x<N*N;x++)
            {
                sorted[x]=0;
            }
            int k=0;
            for(p=i-(N-1)/2;p<=i+(N-1)/2;p++)
            {
                for(q=j-(N-1)/2;q<=j+(N-1)/2;q++)
                {
                    int rowIndex=p;
                    int colIndex=q;
                    if(rowIndex<0)
                    {
                        rowIndex=0;
                    }
                    if(rowIndex>imageHeight-1)
                    {
                        rowIndex=imageHeight-1;
                    }
                    if(colIndex<0)
                    {
                        colIndex=0;
                    }
                    if(colIndex>imageWidth-1)
                    {
                        colIndex=imageWidth-1;
                    }
                    sorted[k]=(int)thresholdedImage[rowIndex][colIndex];
                    k++;
                }
            }
                //If zero corssing is detected, it is considered as an edge
            if(sorted[0]==1 || sorted[1]==1 || sorted[2]==1 || sorted[3]==1 || sorted[5]==1 || sorted[6]==1 || sorted[7]==1 || sorted[8]==1 || sorted[0]==-1 || sorted[1]==-1 || sorted[2]==-1 || sorted[3]==-1 || sorted[5]==-1 || sorted[6]==-1 || sorted[7]==-1 || sorted[8]==-1)
            {
                outputImage[i][j]=0;
            }
            else
            {
                outputImage[i][j]=255;
            }
            }
            else
            {
                outputImage[i][j]=255;
            }
        }
    }
}


void getcombinechannels(unsigned char** thresholdedimage,unsigned char* combinedArray,int imageHeight,int imageWidth)
{
    int rowIndex,colIndex,count=0;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            combinedArray[count]=thresholdedimage[rowIndex][colIndex];
            count++;
        }
    }
}

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int bytesPerPixel;
    int imageHeight=500;
    int imageWidth=500;
    string fileName;
    
    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [bytesPerPixel = 1] [Height=255] [Width=255]" << endl;
        return 0;
    }
    
    // Check if image is grayscale or color
    if (argc < 4){
        bytesPerPixel = 1; // default is grey image
    }
    else {
        bytesPerPixel = atoi(argv[3]);  //atoi converts string characters into integer
        // Check if size is specified
        if (argc >= 5){
            imageWidth= atoi(argv[4]);
            imageHeight= atoi(argv[5]);
            fileName=argv[6];
        }
    }
    
    // Allocate image data array
    int imageDimension=imageHeight*imageWidth*bytesPerPixel;
    unsigned char Imagedata[imageDimension];
    unsigned char combinedArray[imageDimension];
    unsigned char combinedArrayGrayImage[imageDimension];
    
    // Read image (filename specified by first argument) into image data matrix where 'rb' indicate reading a binary file
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    // format of fread is fread ( void * ptr, size of each clock of memory, count to indicate memory required, FILE to read from);
    fread(Imagedata, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fclose(file);
    
    ///////////////////////// PROCESSING CODE ///////////////////////
    //Decalrations for all the arrays
    unsigned char** rpixel2Ddummy1=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy1[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy1[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** rpixel2D=rpixel2Ddummy1;
    
    unsigned char** gpixel2Ddummy1=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        gpixel2Ddummy1[i]=new unsigned char[imageWidth];
        memset(gpixel2Ddummy1[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** gpixel2D=gpixel2Ddummy1;
    
    unsigned char** bpixel2Ddummy1=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        bpixel2Ddummy1[i]=new unsigned char[imageWidth];
        memset(bpixel2Ddummy1[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** bpixel2D=bpixel2Ddummy1;
    
    unsigned char** bpixel2Ddummy12=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        bpixel2Ddummy12[i]=new unsigned char[imageWidth];
        memset(bpixel2Ddummy12[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** luminosityArray=bpixel2Ddummy12;
    
    int** bpixel2Ddummy121=new int* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        bpixel2Ddummy121[i]=new int[imageWidth];
        memset(bpixel2Ddummy121[i],0,imageWidth*sizeof(int));
    }
    int** magnitude=bpixel2Ddummy121;
    
    int** bpixel2Ddummy11=new int* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        bpixel2Ddummy11[i]=new int[imageWidth];
        memset(bpixel2Ddummy11[i],0,imageWidth*sizeof(int));
    }
    int** normalizedArray=bpixel2Ddummy11;
    
    unsigned char** bpixel2Ddummy119=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        bpixel2Ddummy119[i]=new unsigned char[imageWidth];
        memset(bpixel2Ddummy119[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** thresholdedImage=bpixel2Ddummy119;
    
    int** bpixel2Ddummy180=new int* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        bpixel2Ddummy180[i]=new int[imageWidth];
        memset(bpixel2Ddummy180[i],0,imageWidth*sizeof(int));
    }
    int** threethresholdedImage=bpixel2Ddummy180;
    
    unsigned char** bpixel2Ddummy1191=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        bpixel2Ddummy1191[i]=new unsigned char[imageWidth];
        memset(bpixel2Ddummy1191[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** outputImage=bpixel2Ddummy1191;
    
    //Seperating R,G,B channels from the input image
    get2Dchannel(Imagedata,imageHeight,imageWidth,rpixel2D,0);
    get2Dchannel(Imagedata,imageHeight,imageWidth,gpixel2D,1);
    get2Dchannel(Imagedata,imageHeight,imageWidth,bpixel2D,2);
    
    //Convert R,G,B image to gray scale image using luminosity method-- Gray value= 0.21R + 0.72G + 0.07B
    getRGBtoGrayscale(rpixel2D,gpixel2D,bpixel2D,luminosityArray,imageHeight,imageWidth);

    //Use the LoG Filter
    getGradient(luminosityArray,imageHeight,imageWidth,magnitude);

    //Normalize the values in the range 0 to 255
    getnormalization(magnitude,imageHeight,imageWidth,normalizedArray);


    int intensity[256]={0};
    getintensity(normalizedArray,imageHeight,imageWidth,intensity);

    int intensityfinal[256]={0};
    intensityfinal[0]=intensity[0];
    for(int i=0;i<256;i++)
    {
        if(i>0)
        {
            intensityfinal[i]=intensityfinal[i-1]+intensity[i];
        }
    }

    int threshold=(0.90)*(imageHeight*imageWidth);  //set the threshold so that top 10% pixels are considered as an edge
    
    //threshold the LoG filtered image to get a ternary image
    getthreshold(normalizedArray,imageHeight,imageWidth,threshold,thresholdedImage,threethresholdedImage);

    //Detect zero crossings to get a binary image indicating presence or absence of edge
    getZerocrossings(threethresholdedImage,imageHeight,imageWidth,outputImage);

    //Combining output image in one array
    getcombinechannels(thresholdedImage,combinedArrayGrayImage,imageHeight,imageWidth);
    getcombinechannels(outputImage,combinedArray,imageHeight,imageWidth);
    
    
    FILE *histFile;
    histFile=fopen(fileName.c_str(),"w");

    if(histFile!=nullptr)
    {
        for(int i=0;i<=255;i++)
        {
            fprintf(histFile,"%d\t%d\n",i,intensity[i]);
        }
    }
    fclose(histFile);

    // Write image data (filename specified by second argument) from image data matrix
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(combinedArray, sizeof(unsigned char), imageHeight*imageWidth, file);
    //fwrite(combinedArrayGrayImage, sizeof(unsigned char), imageHeight*imageWidth, file);
    fclose(file);
    return 0;
}



