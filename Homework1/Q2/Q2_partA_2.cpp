#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int bytesPerPixel = 3;
    int imageHeight = 300;
    int imageWidth = 400;
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
    unsigned char Imagedata[imageHeight*imageWidth*bytesPerPixel];
    
    // Read image (filename specified by first argument) into image data matrix where 'rb' indicate reading a binary file
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    // format of fread is fread ( void * ptr, size of each clock of memory, count to indicate memory required, FILE to read from);
    fread(Imagedata, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fclose(file);
    
    ///////////////////////// PROCESSING CODE ///////////////////////
    
    
    unsigned int rPixel[imageHeight*imageWidth];
    unsigned int gPixel[imageHeight*imageWidth];
    unsigned int bPixel[imageHeight*imageWidth];
    unsigned char rPixelNew[imageHeight*imageWidth];
    unsigned char gPixelNew[imageHeight*imageWidth];
    unsigned char bPixelNew[imageHeight*imageWidth];
    unsigned char histogramArray[imageHeight*imageWidth*bytesPerPixel];
    unsigned int intensityR[256]={0};
    unsigned int intensityG[256]={0};
    unsigned int intensityB[256]={0};
    unsigned int intensityRcount[256]={0};
    unsigned int intensityGcount[256]={0};
    unsigned int intensityBcount[256]={0};
    unsigned int intensityRfinal[256]={0};
    unsigned int intensityGfinal[256]={0};
    unsigned int intensityBfinal[256]={0};
    unsigned char intensityRnew[imageWidth*imageHeight];
    unsigned char intensityGnew[imageHeight*imageWidth];
    unsigned char intensityBnew[imageHeight*imageWidth];
    unsigned int pR[imageHeight*imageWidth];
    unsigned int pG[imageHeight*imageWidth];
    unsigned int pB[imageHeight*imageWidth];
    int i,j,k;
    
    //Conversion of 1D input image into seperate R,G,B channels
    for (i = 0, j = 0; i < imageHeight*imageWidth && j < imageHeight*imageWidth* 3; i++, j = j + 3)
    {
        rPixel[i] = Imagedata[j];
        gPixel[i]= Imagedata[j+1];
        bPixel[i]= Imagedata[j+2];
    }
    
    //Finding the count of each intensity value for R Channel and storing the pixel locations for each intensity of R channel
    int m=0;
    j=0;
    for(k=0;k<=255;k++)
    {
        for(i=0;i<imageHeight*imageWidth;i++)
        {
            if (rPixel[i] == k)
            {
                pR[m]=i;
                intensityR[k] = intensityR[k]+1;
                intensityRnew[j]=k;
                j++;
                m++;
            }
        }
    }
    
    //Finding the count of each intensity value for G Channel and storing the pixel locations for each intensity of G channel
    m=0;
    j=0;
    for(k=0;k<=255;k++)
    {
        for(i=0;i<imageHeight*imageWidth;i++)
        {
            if (gPixel[i]==k)
            {
                pG[m]=i;
                intensityG[k]=intensityG[k]+1;
                intensityGnew[j]=k;
                j++;
                m++;
            }
        }
    }
    
    //Finding the count of each intensity value for B Channel and storing the pixel locations for each intensity of B channel
    m=0;
    j=0;
    for(k=0;k<=255;k++)
    {
        for(i=0;i<imageHeight*imageWidth;i++)
        {
            if (bPixel[i]==k)
            {
                pB[m]=i;
                intensityB[k]=intensityB[k]+1;
                intensityBnew[j]=k;
                j++;
                m++;
            }
        }
    }
    
    //Defining the buckt size to be total pixels/ Total number if intensities
    float bucketSize=(float)(imageHeight*imageWidth)/(float)256;
    bucketSize=ceil(bucketSize);
    
    //Putting pixels into each bucket by traversing upto bucket size for R,G,B channels- This step will equalize the histogram by putting equal number of pixels in each bin
    int l=0;
    for(i=0;i<imageHeight*imageWidth;i++)
    {
        if (i/bucketSize==0)
        {
            intensityRnew[i]=0;
            intensityGnew[i]=0;
            intensityBnew[i]=0;
            l++;
        }
        else if(i/bucketSize!=l)
        {
            intensityRnew[i]=l-1;
            intensityGnew[i]=l-1;
            intensityBnew[i]=l-1;
        }
        else
        {
            intensityRnew[i]=l;
            intensityGnew[i]=l;
            intensityBnew[i]=l;
            l++;
        }
    }
    
    //Finding the count of intensities for new intensity values after putting them into buckets
    for(k=0;k<=255;k++)
    {
        for(i=0;i<imageHeight*imageWidth;i++)
        {
            if (intensityRnew[i]==k)
            {
                intensityRcount[k]=intensityRcount[k]+1;
            }
            if (intensityGnew[i]==k)
            {
                intensityGcount[k]=intensityGcount[k]+1;
            }
            if (intensityBnew[i]==k)
            {
                intensityBcount[k]=intensityBcount[k]+1;
            }
        }
    }
    
    //finding thr cdf of the intensity values
    for(i=0;i<=255;i++)
    {
        if(i==0)
        {
            intensityRfinal[i]=intensityRcount[i];
            intensityGfinal[i]=intensityGcount[i];
            intensityBfinal[i]=intensityBcount[i];
        }
        else
        {
            intensityRfinal[i]=intensityRfinal[i-1]+intensityRcount[i];
            intensityGfinal[i]=intensityGfinal[i-1]+intensityGcount[i];
            intensityBfinal[i]=intensityBfinal[i-1]+intensityBcount[i];
        }
        
    }
    //Replacing the new intensity values at the original pixel locations in the image
    for(i=0;i<imageHeight*imageWidth;i++)
    {
        rPixelNew[pR[i]]=intensityRnew[i];
        gPixelNew[pG[i]]=intensityGnew[i];
        bPixelNew[pB[i]]=intensityBnew[i];
    }
    
    //Combining the R,G,B channels to get a equalized 1D image using cumulative probability based method
    for (i = 0, j = 0; i < imageHeight*imageWidth && j < imageHeight*imageWidth* 3; i++, j = j + 3)
    {
        histogramArray[j] = rPixelNew[i];
        histogramArray[j+1]= gPixelNew[i];
        histogramArray[j+2]= bPixelNew[i];
    }
    
    //Writing contents into a file to find the histogram of the file
    FILE *histFile;
    histFile=fopen(fileName.c_str(),"w");
    
    if(histFile!=nullptr)
    {
        for(int i=0;i<=255;i++)
        {
            fprintf(histFile,"%d\t%d\t%d\t%d\n",i,intensityRfinal[i],intensityGfinal[i],intensityBfinal[i]);
        }
    }
    fclose(histFile);
    
    // Write image data (filename specified by second argument) from image data matrix
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    //copies contents from file pointed by imagedata to the filename given in the input
    fwrite(histogramArray, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    //fwrite(gPixel, sizeof(unsigned int), imageHeight*imageWidth, file);
    fclose(file);
    
    
    return 0;
}




