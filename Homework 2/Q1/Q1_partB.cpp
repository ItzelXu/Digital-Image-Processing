//
//  main.cpp
//  Q1b
//
//  Created by Deepika Kanade on 2/27/18.
//  Copyright © 2018 Deepika Kanade. All rights reserved.
//

#include <iostream>
#include<stdio.h>
#include<math.h>

using namespace std;

void getLeftembedding(unsigned char* embeddedImage,unsigned char* Imagedata1,int imageHeight,int imageWidth,int finalHeight,int finalWidth)
{
    float Hinv[3][3]={{0.7086,-0.1410,-76.7094},{-0.0175,0.5247,-202.1456},{-0.0000,-0.0005,1.2032}}; //Homography matrix calculated in Matlab
    float W;
    long double b[3];
    for(int i=0;i<finalHeight;i++)
    {
        for(int j=0;j<finalWidth;j++)
        {
            int a[3]={i,j,1};
            W=1/((Hinv[2][0]*i)+(Hinv[2][1]*j)+Hinv[2][2]);
            
            //Matrix multiplication
            int k;
            for(int i=0;i<3;i++)
            {
                b[i]=0;
                for(k=0;k<3;k++)
                {
                    b[i]=b[i]+Hinv[i][k]*a[k];
                }
            }
            
            float x=(float)b[0]*(float)W;  //To divide the first and secong array value by third array value i.e. conversion from homogenous to cartesian co-ordinates
            float y=(float)b[1]*(float)W;
            
            int referencex=floor(x);
            int referencey=floor(y);
            
            float delx=x-referencex;
            float dely=y-referencey;
            
            //Bilinear Interpolation to calculate the intensity values for left image with respect to middle image
            if(referencex > 0 && referencex < imageHeight && referencey > 0 && referencey < imageWidth)
            {
                int topleftImagepixel=(referencex*imageWidth)+referencey;
                int topleftImageLocation=topleftImagepixel*3;
                
                int toprightImagepixel=(referencex*imageWidth)+referencey+1;
                int toprightImageLocation=toprightImagepixel*3;
                
                int bottomleftImagepixel=(referencex*imageWidth)+referencey+imageWidth;
                int bottomleftImageLocation=bottomleftImagepixel*3;
                
                int bottomrightImagepixel=(referencex*imageWidth)+referencey+imageWidth+1;
                int bottomrightImageLocation=bottomrightImagepixel*3;
                
                int embedpixel=(i*finalWidth)+j;
                int embedLocation=embedpixel*3;
                
                embeddedImage[embedLocation+0] = (unsigned char)(Imagedata1[topleftImageLocation+0] * (1 - delx) * (1 - dely) +                              Imagedata1[bottomleftImageLocation+0] * (delx) * (1 - dely)+
                                                                 Imagedata1[toprightImageLocation+0] * (1 - delx) *  (dely)+
                                                                 Imagedata1[bottomrightImageLocation+0] * (delx) * (dely));
                
                embeddedImage[embedLocation+1] = (unsigned char)(Imagedata1[topleftImageLocation+1] * (1 - delx) * (1 - dely) +                           Imagedata1[bottomleftImageLocation+1] * (delx) * (1 - dely)+
                                                                 Imagedata1[toprightImageLocation+1] * (1 - delx) *  (dely)+
                                                                 Imagedata1[bottomrightImageLocation+1] * (delx) * (dely));
                
                embeddedImage[embedLocation+2] = (unsigned char)(Imagedata1[topleftImageLocation+2] * (1 - delx) * (1 - dely) +                           Imagedata1[bottomleftImageLocation+2] * (delx) * (1 - dely)+
                                                                 Imagedata1[toprightImageLocation+2] * (1 - delx) *  (dely) +
                                                                 Imagedata1[bottomrightImageLocation+2] * (delx) * (dely));
            }
        }
    }
}

void getRightembedding(unsigned char* embeddedImage,unsigned char* Imagedata3,int imageHeight,int imageWidth,int finalHeight,int finalWidth)
{
    float Hinv[3][3]=
    {{1.110981575213,0.320334477785,-702.220261254909},
        { 0.001011231121,1.247497630304,-1260.823570050726},
        {-0.000041246898,0.000950951874,0.053206924333}} ;
    long double W;
    long double b[3];
    for(int i=0;i<=finalHeight;i++)
    {
        for(int j=0;j<=finalWidth;j++)
        {
            int a[3]={i,j,1};
            
            W=1/((Hinv[2][0]*i)+(Hinv[2][1]*j)+Hinv[2][2]);
            
            //Matrix multiplication
            int k;
            for(int i=0;i<3;i++)
            {
                b[i]=0;
                for(k=0;k<3;k++)
                {
                    b[i]=b[i]+Hinv[i][k]*a[k];
                }
            }
            
            long double x=b[0]*W;  //To divide the first and secong array value by third array value i.e. conversion from homogenous to cartesian co-ordinates
            long double y=b[1]*W;
            
            int referencex=floor(x);
            int referencey=floor(y);
            
            long double delx=x-referencex;
            long double dely=y-referencey;
            
            //Bilinear Interpolation to calculate the intensity values for right image with respect to middle image
            if(referencex > 0 && referencex < imageHeight && referencey > 0 && referencey < imageWidth)
            {
                int topleftImagepixel=(referencex*imageWidth)+referencey;
                int topleftImageLocation=topleftImagepixel*3;
                
                int toprightImagepixel=(referencex*imageWidth)+referencey+1;
                int toprightImageLocation=toprightImagepixel*3;
                
                int bottomleftImagepixel=(referencex*imageWidth)+referencey+imageWidth;
                int bottomleftImageLocation=bottomleftImagepixel*3;
                
                int bottomrightImagepixel=(referencex*imageWidth)+referencey+imageWidth+1;
                int bottomrightImageLocation=bottomrightImagepixel*3;
                
                int embedpixel=(i*finalWidth)+j;
                int embedLocation=embedpixel*3;
                
                embeddedImage[embedLocation+0] = (unsigned char)(Imagedata3[topleftImageLocation+0] * (1 - delx) * (1 - dely) +                              Imagedata3[bottomleftImageLocation+0] * (delx) * (1 - dely)+
                                                                 Imagedata3[toprightImageLocation+0] * (1 - delx) *  (dely)+
                                                                 Imagedata3[bottomrightImageLocation+0] * (delx) * (dely));
                
                embeddedImage[embedLocation+1] = (unsigned char)(Imagedata3[topleftImageLocation+1] * (1 - delx) * (1 - dely) +                           Imagedata3[bottomleftImageLocation+1] * (delx) * (1 - dely)+
                                                                 Imagedata3[toprightImageLocation+1] * (1 - delx) *  (dely)+
                                                                 Imagedata3[bottomrightImageLocation+1] * (delx) * (dely));
                
                embeddedImage[embedLocation+2] = (unsigned char)(Imagedata3[topleftImageLocation+2] * (1 - delx) * (1 - dely) +                           Imagedata3[bottomleftImageLocation+2] * (delx) * (1 - dely)+
                                                                 Imagedata3[toprightImageLocation+2] * (1 - delx) *  (dely) +
                                                                 Imagedata3[bottomrightImageLocation+2] * (delx) * (dely));
            }
        }
    }
}


void getEmbeddedMiddle(unsigned char* embeddedImage,unsigned char* Imagedata2,int imageHeight,int imageWidth,int finalHeight, int finalWidth,int rowOffset,int colOffset)
{
    int currentPixel=0,embedpixel,embedLocation;
    for(int i=rowOffset;i<rowOffset+imageHeight;i++)
    {
        for(int j=colOffset;j<colOffset+imageWidth;j++)
        {
            embedpixel=(i*finalWidth)+j;
            embedLocation=embedpixel*3;
            embeddedImage[embedLocation+0]=Imagedata2[currentPixel+0];
            embeddedImage[embedLocation+1]=Imagedata2[currentPixel+1];
            embeddedImage[embedLocation+2]=Imagedata2[currentPixel+2];
            currentPixel=currentPixel+3;
        }
    }
}
int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    FILE *file1;
    FILE *file2;
    int bytesPerPixel;
    int imageHeight=500;
    int imageWidth=500;
    
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
        bytesPerPixel = atoi(argv[5]);  //atoi converts string characters into integer
        // Check if size is specified
        if (argc >= 5){
            imageWidth= atoi(argv[6]);
            imageHeight= atoi(argv[7]);
        }
    }
    
    // Allocate image data array
    unsigned char Imagedata1[imageHeight*imageWidth*bytesPerPixel];
    unsigned char Imagedata2[imageHeight*imageWidth*bytesPerPixel];
    unsigned char Imagedata3[imageHeight*imageWidth*bytesPerPixel];
    
    // Read image (filename specified by first argument) into image data matrix where 'rb' indicate reading a binary file
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    if (!(file1=fopen(argv[2],"rb"))) {
        cout << "Cannot open file: " << argv[2] <<endl;
        exit(1);
    }
    if (!(file2=fopen(argv[3],"rb"))) {
        cout << "Cannot open file: " << argv[3] <<endl;
        exit(1);
    }
    // format of fread is fread ( void * ptr, size of each clock of memory, count to indicate memory required, FILE to read from);
    fread(Imagedata1, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fclose(file);
    fread(Imagedata2, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file1);
    fclose(file1);
    fread(Imagedata3, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file2);
    fclose(file2);
    
    ///////////////////////// PROCESSING CODE ///////////////////////
    
    int rowOffset=300; // Keeping a rowoffset and column offset after which the embedding will start
    int colOffset=795;
    
    int finalHeight=1240;
    int finalWidth=2500;
    
    unsigned char* embeddedImage=new unsigned char[finalHeight*finalWidth*bytesPerPixel];
    //Create a black image to be embedded later by the left and right images
    memset(embeddedImage,0,finalWidth*finalHeight*bytesPerPixel*sizeof(unsigned char));
    
    //Embed the middle image into the black image keeping the row and column offset
    getEmbeddedMiddle(embeddedImage,Imagedata2,imageHeight,imageWidth,finalHeight,finalWidth,rowOffset,colOffset);
    
    //Embed the left image into the black image now containing the middle image as well
    getLeftembedding(embeddedImage,Imagedata1,imageHeight,imageWidth,finalHeight,finalWidth);
    
    //Embed the right image into the black image now containing the middle image and left image as well
    getRightembedding(embeddedImage,Imagedata3,imageHeight,imageWidth,finalHeight,finalWidth);
    
    //Embed the middle image back into the embedded image containing the left,right images already embedded
    getEmbeddedMiddle(embeddedImage,Imagedata2,imageHeight,imageWidth,finalHeight,finalWidth,rowOffset,colOffset);
 
    // Write image data (filename specified by fourth argument) from image data matrix
    if (!(file=fopen(argv[4],"wb"))) {
        cout << "Cannot open file: " << argv[4] << endl;
        exit(1);
    }
    fwrite(embeddedImage, sizeof(unsigned char), finalHeight*finalWidth*bytesPerPixel, file);
    fclose(file);
    return 0;
}






