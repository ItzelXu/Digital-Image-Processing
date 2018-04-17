#include <iostream>
#include <stdlib.h>
#include <stdio.h>
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

void getcombinechannels(unsigned char** rpixel,unsigned char* combinedArray,int imageHeight,int imageWidth,int iterationIndex)
{
    int rowIndex,colIndex,count=0;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            combinedArray[count+iterationIndex]=rpixel[rowIndex][colIndex];
            count=count+3;
        }
    }
}

void getfloydmatrix(unsigned char **rPixel2D,int imageHeight,int imageWidth,unsigned char **rPixel2Dnew)
{
    int i,j;
    float error;
    for(i=0;i<imageHeight;i++)
    {
        if (i%2==0)  // left to right traversal for even rows
        {
            for(j=0;j<imageWidth;j++)
            {
                if (rPixel2D[i][j]<=127)
                {
                    rPixel2Dnew[i][j]=0;
                }
                else
                {
                    rPixel2Dnew[i][j]=255;
                }
                //Error calculation where error=original value - thresholded value
                error=rPixel2D[i][j]-rPixel2Dnew[i][j];
                //Error Diffusion to future pixels
                if(j<imageWidth-1)
                {
                    rPixel2D[i][j+1]=rPixel2D[i][j+1]+((int)((error*(float)(7.00/16.00))));
                }
                if(i<imageHeight-1 && j>0)
                {
                    rPixel2D[i+1][j-1]=rPixel2D[i+1][j-1]+(int)((error*(float)(3.00/16.00)));
                }
                if(i<imageHeight-1)
                {
                    rPixel2D[i+1][j]=rPixel2D[i+1][j]+(int)((error*(float)(5.00/16.00)));
                }
                if(i<imageHeight-1 && j<imageWidth-1)
                {
                    rPixel2D[i+1][j+1]=rPixel2D[i+1][j+1]+(int)((error*(float)(1.0/16.00)));
                }
            }
        }
        else if(i%2==1)  //right to left traversal for odd rows
        {
            for(j=imageWidth-1;j>=0;j--)
            {
                if (rPixel2D[i][j]<127)
                {
                    rPixel2Dnew[i][j]=0;
                }
                else
                {
                    rPixel2Dnew[i][j]=255;
                }
                //Error calculation where error=original value - thresholded value
                error=rPixel2D[i][j]-rPixel2Dnew[i][j];
                //Error Diffusion to future pixels
                if(j>0)
                {
                    rPixel2D[i][j-1]=rPixel2D[i][j-1]+((int)((error*(float)(7.00/16.00))));
                }
                if(i<imageHeight-1 && j>0)
                {
                    rPixel2D[i+1][j-1]=rPixel2D[i+1][j-1]+(int)((error*(float)(1.00/16.00)));
                }
                if(i<imageHeight-1)
                {
                    rPixel2D[i+1][j]=rPixel2D[i+1][j]+(int)((error*(float)(5.00/16.00)));
                }
                if(i<imageHeight-1 && j<imageWidth-1)
                {
                    rPixel2D[i+1][j+1]=rPixel2D[i+1][j+1]+(int)((error*(float)(3.00/16.00)));
                }
            }
        }
    }
}

void getMBVQ(unsigned char **rpixel,unsigned char** gpixel,unsigned char** bpixel,unsigned char **cpixel,unsigned char** mpixel, unsigned char** ypixel,unsigned char** cpixelnew,unsigned char** mpixelnew,unsigned char**ypixelnew, int imageHeight,int imageWidth)
{
    int rowIndex,colIndex,dist1,dist2,dist3,dist4,min1,min2,min3,minValue,errorC,errorM,errorY;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        if(rowIndex%2==0)
        {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            if(((int)rpixel[rowIndex][colIndex]+(int)gpixel[rowIndex][colIndex]) > 255)
            {
                if(((int)gpixel[rowIndex][colIndex]+(int)bpixel[rowIndex][colIndex]) > 255)
                {
                    if(((int)rpixel[rowIndex][colIndex]+(int)gpixel[rowIndex][colIndex]+(int)bpixel[rowIndex][colIndex]) > 510)
                    {
                        //CMYW tetrahedron
                        //Finding euclidean distances with respect to each point in the tetrahedron
                        dist1=(sqrt(pow((mpixel[rowIndex][colIndex]-0),2)+pow((cpixel[rowIndex][colIndex])-255,2)+pow((ypixel[rowIndex][colIndex]-0),2)));//C
                        dist2=(sqrt(pow((mpixel[rowIndex][colIndex]-255),2)+pow((cpixel[rowIndex][colIndex])-0,2)+pow((ypixel[rowIndex][colIndex]-0),2)));//M
                        dist3=(sqrt(pow((mpixel[rowIndex][colIndex]-0),2)+pow((cpixel[rowIndex][colIndex])-0,2)+pow((ypixel[rowIndex][colIndex]-255),2)));//Y
                        dist4=(sqrt(pow((mpixel[rowIndex][colIndex]-0),2)+pow((cpixel[rowIndex][colIndex])-0,2)+pow((ypixel[rowIndex][colIndex]-0),2)));//W
                        min1=min(dist1,dist2); // Finding the minimum distance
                        min2=min(min1,dist3);
                        min3=min(min2,dist4);
                        minValue=min3;
                        //Assigning the intensoty values at the point having closest distance from the data point
                        if(minValue==dist1)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)0;
                        }
                        if(minValue==dist2)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)0;
                        }
                        if(minValue==dist3)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)255;
                        }
                        if(minValue==dist4)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)0;
                        }
                        //Calculating error-- error= original value - value assigned according to closest distance (calculating error seperately for three channels)
                        errorC=cpixel[rowIndex][colIndex]-cpixelnew[rowIndex][colIndex];
                        errorM=mpixel[rowIndex][colIndex]-mpixelnew[rowIndex][colIndex];
                        errorY=ypixel[rowIndex][colIndex]-ypixelnew[rowIndex][colIndex];
                        //Error diffusion to future pixels
                        if(colIndex<imageWidth-1)
                        {
                            cpixel[rowIndex][colIndex+1]=cpixel[rowIndex][colIndex+1]+((int)((errorC*(float)(7.00/16.00))));
                            mpixel[rowIndex][colIndex+1]=mpixel[rowIndex][colIndex+1]+((int)((errorM*(float)(7.00/16.00))));
                            ypixel[rowIndex][colIndex+1]=ypixel[rowIndex][colIndex+1]+((int)((errorY*(float)(7.00/16.00))));
                        }
                        if(rowIndex<imageHeight-1 && colIndex>0)
                        {
                            cpixel[rowIndex][colIndex+1]=cpixel[rowIndex][colIndex+1]+((int)((errorC*(float)(3.00/16.00))));
                            mpixel[rowIndex][colIndex+1]=mpixel[rowIndex][colIndex+1]+((int)((errorM*(float)(3.00/16.00))));
                            ypixel[rowIndex][colIndex+1]=ypixel[rowIndex][colIndex+1]+((int)((errorY*(float)(3.00/16.00))));
                        }
                        if(rowIndex<imageHeight-1)
                        {
                            cpixel[rowIndex][colIndex+1]=cpixel[rowIndex][colIndex+1]+((int)((errorC*(float)(5.00/16.00))));
                            mpixel[rowIndex][colIndex+1]=mpixel[rowIndex][colIndex+1]+((int)((errorM*(float)(5.00/16.00))));
                            ypixel[rowIndex][colIndex+1]=ypixel[rowIndex][colIndex+1]+((int)((errorY*(float)(5.00/16.00))));
                        }
                        if(rowIndex<imageHeight-1 && colIndex<imageWidth-1)
                        {
                            cpixel[rowIndex][colIndex+1]=cpixel[rowIndex][colIndex+1]+((int)((errorC*(float)(1.00/16.00))));
                            mpixel[rowIndex][colIndex+1]=mpixel[rowIndex][colIndex+1]+((int)((errorM*(float)(1.00/16.00))));
                            ypixel[rowIndex][colIndex+1]=ypixel[rowIndex][colIndex+1]+((int)((errorY*(float)(1.00/16.00))));
                        }
                    }
                    else
                    {
                        //MYGC tetrahedron
                        dist1=(sqrt(pow((mpixel[rowIndex][colIndex]-255),2)+pow((cpixel[rowIndex][colIndex])-0,2)+pow((ypixel[rowIndex][colIndex]-0),2)));//M
                        dist2=(sqrt(pow((mpixel[rowIndex][colIndex]-0),2)+pow((cpixel[rowIndex][colIndex])-0,2)+pow((ypixel[rowIndex][colIndex]-255),2)));//Y
                        dist3=(sqrt(pow((mpixel[rowIndex][colIndex]-0),2)+pow((cpixel[rowIndex][colIndex])-255,2)+pow((ypixel[rowIndex][colIndex]-255),2)));//G
                        dist4=(sqrt(pow((mpixel[rowIndex][colIndex]-0),2)+pow((cpixel[rowIndex][colIndex])-255,2)+pow((ypixel[rowIndex][colIndex]-0),2)));//C
                        min1=min(dist1,dist2);
                        min2=min(min1,dist3);
                        min3=min(min2,dist4);
                        minValue=min3;
                        if(minValue==dist1)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)0;
                        }
                        if(minValue==dist2)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)255;
                        }
                        if(minValue==dist3)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)255;
                        }
                        if(minValue==dist4)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)0;
                        }
                        errorC=cpixel[rowIndex][colIndex]-cpixelnew[rowIndex][colIndex];
                        errorM=mpixel[rowIndex][colIndex]-mpixelnew[rowIndex][colIndex];
                        errorY=ypixel[rowIndex][colIndex]-ypixelnew[rowIndex][colIndex];
                        if(colIndex<imageWidth-1)
                        {
                            cpixel[rowIndex][colIndex+1]=cpixel[rowIndex][colIndex+1]+((int)((errorC*(float)(7.00/16.00))));
                            mpixel[rowIndex][colIndex+1]=mpixel[rowIndex][colIndex+1]+((int)((errorM*(float)(7.00/16.00))));
                            ypixel[rowIndex][colIndex+1]=ypixel[rowIndex][colIndex+1]+((int)((errorY*(float)(7.00/16.00))));
                        }
                        if(rowIndex<imageHeight-1 && colIndex>0)
                        {
                            cpixel[rowIndex][colIndex+1]=cpixel[rowIndex][colIndex+1]+((int)((errorC*(float)(3.00/16.00))));
                            mpixel[rowIndex][colIndex+1]=mpixel[rowIndex][colIndex+1]+((int)((errorM*(float)(3.00/16.00))));
                            ypixel[rowIndex][colIndex+1]=ypixel[rowIndex][colIndex+1]+((int)((errorY*(float)(3.00/16.00))));
                        }
                        if(rowIndex<imageHeight-1)
                        {
                            cpixel[rowIndex][colIndex+1]=cpixel[rowIndex][colIndex+1]+((int)((errorC*(float)(5.00/16.00))));
                            mpixel[rowIndex][colIndex+1]=mpixel[rowIndex][colIndex+1]+((int)((errorM*(float)(5.00/16.00))));
                            ypixel[rowIndex][colIndex+1]=ypixel[rowIndex][colIndex+1]+((int)((errorY*(float)(5.00/16.00))));
                        }
                        if(rowIndex<imageHeight-1 && colIndex<imageWidth-1)
                        {
                            cpixel[rowIndex][colIndex+1]=cpixel[rowIndex][colIndex+1]+((int)((errorC*(float)(1.00/16.00))));
                            mpixel[rowIndex][colIndex+1]=mpixel[rowIndex][colIndex+1]+((int)((errorM*(float)(1.00/16.00))));
                            ypixel[rowIndex][colIndex+1]=ypixel[rowIndex][colIndex+1]+((int)((errorY*(float)(1.00/16.00))));
                        }
                    }
                }
                else //RGMY tetrahedron
                {
                    dist1=(sqrt(pow((mpixel[rowIndex][colIndex]-255),2)+pow((cpixel[rowIndex][colIndex])-0,2)+pow((ypixel[rowIndex][colIndex]-255),2)));//R
                    dist2=(sqrt(pow((mpixel[rowIndex][colIndex]-0),2)+pow((cpixel[rowIndex][colIndex])-255,2)+pow((ypixel[rowIndex][colIndex]-255),2)));//G
                    dist3=(sqrt(pow((mpixel[rowIndex][colIndex]-255),2)+pow((cpixel[rowIndex][colIndex])-0,2)+pow((ypixel[rowIndex][colIndex]-0),2)));//M
                    dist4=(sqrt(pow((mpixel[rowIndex][colIndex]-0),2)+pow((cpixel[rowIndex][colIndex])-0,2)+pow((ypixel[rowIndex][colIndex]-255),2)));//Y
                    min1=min(dist1,dist2);
                    min2=min(min1,dist3);
                    min3=min(min2,dist4);
                    minValue=min3;
                    if(minValue==dist1)
                    {
                        cpixelnew[rowIndex][colIndex]=(unsigned char)0;
                        mpixelnew[rowIndex][colIndex]=(unsigned char)255;
                        ypixelnew[rowIndex][colIndex]=(unsigned char)0;
                    }
                    if(minValue==dist2)
                    {
                        cpixelnew[rowIndex][colIndex]=(unsigned char)255;
                        mpixelnew[rowIndex][colIndex]=(unsigned char)0;
                        ypixelnew[rowIndex][colIndex]=(unsigned char)255;
                    }
                    if(minValue==dist3)
                    {
                        cpixelnew[rowIndex][colIndex]=(unsigned char)0;
                        mpixelnew[rowIndex][colIndex]=(unsigned char)255;
                        ypixelnew[rowIndex][colIndex]=(unsigned char)0;
                    }
                    if(minValue==dist4)
                    {
                        cpixelnew[rowIndex][colIndex]=(unsigned char)0;
                        mpixelnew[rowIndex][colIndex]=(unsigned char)0;
                        ypixelnew[rowIndex][colIndex]=(unsigned char)255;
                    }
                    errorC=cpixel[rowIndex][colIndex]-cpixelnew[rowIndex][colIndex];
                    errorM=mpixel[rowIndex][colIndex]-mpixelnew[rowIndex][colIndex];
                    errorY=ypixel[rowIndex][colIndex]-ypixelnew[rowIndex][colIndex];
                    if(colIndex<imageWidth-1)
                    {
                        cpixel[rowIndex][colIndex+1]=cpixel[rowIndex][colIndex+1]+((int)((errorC*(float)(7.00/16.00))));
                        mpixel[rowIndex][colIndex+1]=mpixel[rowIndex][colIndex+1]+((int)((errorM*(float)(7.00/16.00))));
                        ypixel[rowIndex][colIndex+1]=ypixel[rowIndex][colIndex+1]+((int)((errorY*(float)(7.00/16.00))));
                    }
                    if(rowIndex<imageHeight-1 && colIndex>0)
                    {
                        cpixel[rowIndex][colIndex+1]=cpixel[rowIndex][colIndex+1]+((int)((errorC*(float)(3.00/16.00))));
                        mpixel[rowIndex][colIndex+1]=mpixel[rowIndex][colIndex+1]+((int)((errorM*(float)(3.00/16.00))));
                        ypixel[rowIndex][colIndex+1]=ypixel[rowIndex][colIndex+1]+((int)((errorY*(float)(3.00/16.00))));
                    }
                    if(rowIndex<imageHeight-1)
                    {
                        cpixel[rowIndex][colIndex+1]=cpixel[rowIndex][colIndex+1]+((int)((errorC*(float)(5.00/16.00))));
                        mpixel[rowIndex][colIndex+1]=mpixel[rowIndex][colIndex+1]+((int)((errorM*(float)(5.00/16.00))));
                        ypixel[rowIndex][colIndex+1]=ypixel[rowIndex][colIndex+1]+((int)((errorY*(float)(5.00/16.00))));
                    }
                    if(rowIndex<imageHeight-1 && colIndex<imageWidth-1)
                    {
                        cpixel[rowIndex][colIndex+1]=cpixel[rowIndex][colIndex+1]+((int)((errorC*(float)(1.00/16.00))));
                        mpixel[rowIndex][colIndex+1]=mpixel[rowIndex][colIndex+1]+((int)((errorM*(float)(1.00/16.00))));
                        ypixel[rowIndex][colIndex+1]=ypixel[rowIndex][colIndex+1]+((int)((errorY*(float)(1.00/16.00))));
                    }

                }
            }
            else
            {
                if(!((gpixel[rowIndex][colIndex]+bpixel[rowIndex][colIndex]) > 255))
                {
                    if(!((rpixel[rowIndex][colIndex]+gpixel[rowIndex][colIndex]+bpixel[rowIndex][colIndex]) > 255))
                    {
                        //KRGB tetrahedron
                        dist1=(sqrt(pow((mpixel[rowIndex][colIndex]-255),2)+pow((cpixel[rowIndex][colIndex])-255,2)+pow((ypixel[rowIndex][colIndex]-255),2)));//K
                        dist2=(sqrt(pow((mpixel[rowIndex][colIndex]-255),2)+pow((cpixel[rowIndex][colIndex])-0,2)+pow((ypixel[rowIndex][colIndex]-255),2)));//R
                        dist3=(sqrt(pow((mpixel[rowIndex][colIndex]-0),2)+pow((cpixel[rowIndex][colIndex])-255,2)+pow((ypixel[rowIndex][colIndex]-255),2)));//G
                        dist4=(sqrt(pow((mpixel[rowIndex][colIndex]-255),2)+pow((cpixel[rowIndex][colIndex])-255,2)+pow((ypixel[rowIndex][colIndex]-0),2)));//B
                        min1=min(dist1,dist2);
                        min2=min(min1,dist3);
                        min3=min(min2,dist4);
                        minValue=min3;
                        if(minValue==dist1)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)255;
                        }
                        if(minValue==dist2)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)255;
                        }
                        if(minValue==dist3)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)255;
                        }
                        if(minValue==dist4)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)0;
                        }
                        errorC=cpixel[rowIndex][colIndex]-cpixelnew[rowIndex][colIndex];
                        errorM=mpixel[rowIndex][colIndex]-mpixelnew[rowIndex][colIndex];
                        errorY=ypixel[rowIndex][colIndex]-ypixelnew[rowIndex][colIndex];
                        if(colIndex<imageWidth-1)
                        {
                            cpixel[rowIndex][colIndex+1]=cpixel[rowIndex][colIndex+1]+((int)((errorC*(float)(7.00/16.00))));
                            mpixel[rowIndex][colIndex+1]=mpixel[rowIndex][colIndex+1]+((int)((errorM*(float)(7.00/16.00))));
                            ypixel[rowIndex][colIndex+1]=ypixel[rowIndex][colIndex+1]+((int)((errorY*(float)(7.00/16.00))));
                        }
                        if(rowIndex<imageHeight-1 && colIndex>0)
                        {
                            cpixel[rowIndex][colIndex+1]=cpixel[rowIndex][colIndex+1]+((int)((errorC*(float)(3.00/16.00))));
                            mpixel[rowIndex][colIndex+1]=mpixel[rowIndex][colIndex+1]+((int)((errorM*(float)(3.00/16.00))));
                            ypixel[rowIndex][colIndex+1]=ypixel[rowIndex][colIndex+1]+((int)((errorY*(float)(3.00/16.00))));
                        }
                        if(rowIndex<imageHeight-1)
                        {
                            cpixel[rowIndex][colIndex+1]=cpixel[rowIndex][colIndex+1]+((int)((errorC*(float)(5.00/16.00))));
                            mpixel[rowIndex][colIndex+1]=mpixel[rowIndex][colIndex+1]+((int)((errorM*(float)(5.00/16.00))));
                            ypixel[rowIndex][colIndex+1]=ypixel[rowIndex][colIndex+1]+((int)((errorY*(float)(5.00/16.00))));
                        }
                        if(rowIndex<imageHeight-1 && colIndex<imageWidth-1)
                        {
                            cpixel[rowIndex][colIndex+1]=cpixel[rowIndex][colIndex+1]+((int)((errorC*(float)(1.00/16.00))));
                            mpixel[rowIndex][colIndex+1]=mpixel[rowIndex][colIndex+1]+((int)((errorM*(float)(1.00/16.00))));
                            ypixel[rowIndex][colIndex+1]=ypixel[rowIndex][colIndex+1]+((int)((errorY*(float)(1.00/16.00))));
                        }

                        
                    }
                    else
                    {
                        //RGBM tetrahedron
                        dist1=(sqrt(pow((mpixel[rowIndex][colIndex]-255),2)+pow((cpixel[rowIndex][colIndex])-0,2)+pow((ypixel[rowIndex][colIndex]-255),2)));//R
                        dist2=(sqrt(pow((mpixel[rowIndex][colIndex]-0),2)+pow((cpixel[rowIndex][colIndex])-255,2)+pow((ypixel[rowIndex][colIndex]-255),2)));//G
                        dist3=(sqrt(pow((mpixel[rowIndex][colIndex]-255),2)+pow((cpixel[rowIndex][colIndex])-255,2)+pow((ypixel[rowIndex][colIndex]-0),2)));//B
                        dist4=(sqrt(pow((mpixel[rowIndex][colIndex]-255),2)+pow((cpixel[rowIndex][colIndex])-0,2)+pow((ypixel[rowIndex][colIndex]-0),2)));//M
                        min1=min(dist1,dist2);
                        min2=min(min1,dist3);
                        min3=min(min2,dist4);
                        minValue=min3;
                        if(minValue==dist1)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)255;
                        }
                        if(minValue==dist2)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)255;
                        }
                        if(minValue==dist3)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)0;
                        }
                        if(minValue==dist4)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)0;
                        }
                        errorC=cpixel[rowIndex][colIndex]-cpixelnew[rowIndex][colIndex];
                        errorM=mpixel[rowIndex][colIndex]-mpixelnew[rowIndex][colIndex];
                        errorY=ypixel[rowIndex][colIndex]-ypixelnew[rowIndex][colIndex];
                        if(colIndex<imageWidth-1)
                        {
                            cpixel[rowIndex][colIndex+1]=cpixel[rowIndex][colIndex+1]+((int)((errorC*(float)(7.00/16.00))));
                            mpixel[rowIndex][colIndex+1]=mpixel[rowIndex][colIndex+1]+((int)((errorM*(float)(7.00/16.00))));
                            ypixel[rowIndex][colIndex+1]=ypixel[rowIndex][colIndex+1]+((int)((errorY*(float)(7.00/16.00))));
                        }
                        if(rowIndex<imageHeight-1 && colIndex>0)
                        {
                            cpixel[rowIndex][colIndex+1]=cpixel[rowIndex][colIndex+1]+((int)((errorC*(float)(3.00/16.00))));
                            mpixel[rowIndex][colIndex+1]=mpixel[rowIndex][colIndex+1]+((int)((errorM*(float)(3.00/16.00))));
                            ypixel[rowIndex][colIndex+1]=ypixel[rowIndex][colIndex+1]+((int)((errorY*(float)(3.00/16.00))));
                        }
                        if(rowIndex<imageHeight-1)
                        {
                            cpixel[rowIndex][colIndex+1]=cpixel[rowIndex][colIndex+1]+((int)((errorC*(float)(5.00/16.00))));
                            mpixel[rowIndex][colIndex+1]=mpixel[rowIndex][colIndex+1]+((int)((errorM*(float)(5.00/16.00))));
                            ypixel[rowIndex][colIndex+1]=ypixel[rowIndex][colIndex+1]+((int)((errorY*(float)(5.00/16.00))));
                        }
                        if(rowIndex<imageHeight-1 && colIndex<imageWidth-1)
                        {
                            cpixel[rowIndex][colIndex+1]=cpixel[rowIndex][colIndex+1]+((int)((errorC*(float)(1.00/16.00))));
                            mpixel[rowIndex][colIndex+1]=mpixel[rowIndex][colIndex+1]+((int)((errorM*(float)(1.00/16.00))));
                            ypixel[rowIndex][colIndex+1]=ypixel[rowIndex][colIndex+1]+((int)((errorY*(float)(1.00/16.00))));
                        }

                    }
                }
                else
                {
                    //CMGB tetrahedron
                    dist1=(sqrt(pow((mpixel[rowIndex][colIndex]-0),2)+pow((cpixel[rowIndex][colIndex])-255,2)+pow((ypixel[rowIndex][colIndex]-0),2)));//C
                    dist2=(sqrt(pow((mpixel[rowIndex][colIndex]-255),2)+pow((cpixel[rowIndex][colIndex])-0,2)+pow((ypixel[rowIndex][colIndex]-0),2)));//M
                    dist3=(sqrt(pow((mpixel[rowIndex][colIndex]-0),2)+pow((cpixel[rowIndex][colIndex])-255,2)+pow((ypixel[rowIndex][colIndex]-255),2)));//G
                    dist4=(sqrt(pow((mpixel[rowIndex][colIndex]-255),2)+pow((cpixel[rowIndex][colIndex])-255,2)+pow((ypixel[rowIndex][colIndex]-0),2)));//B
                    min1=min(dist1,dist2);
                    min2=min(min1,dist3);
                    min3=min(min2,dist4);
                    minValue=min3;
                    if(minValue==dist1)
                    {
                        cpixelnew[rowIndex][colIndex]=(unsigned char)255;
                        mpixelnew[rowIndex][colIndex]=(unsigned char)0;
                        ypixelnew[rowIndex][colIndex]=(unsigned char)0;
                    }
                    if(minValue==dist2)
                    {
                        cpixelnew[rowIndex][colIndex]=(unsigned char)0;
                        mpixelnew[rowIndex][colIndex]=(unsigned char)255;
                        ypixelnew[rowIndex][colIndex]=(unsigned char)0;
                    }
                    if(minValue==dist3)
                    {
                        cpixelnew[rowIndex][colIndex]=(unsigned char)255;
                        mpixelnew[rowIndex][colIndex]=(unsigned char)0;
                        ypixelnew[rowIndex][colIndex]=(unsigned char)255;
                    }
                    if(minValue==dist4)
                    {
                        cpixelnew[rowIndex][colIndex]=(unsigned char)255;
                        mpixelnew[rowIndex][colIndex]=(unsigned char)255;
                        ypixelnew[rowIndex][colIndex]=(unsigned char)0;
                    }
                    errorC=cpixel[rowIndex][colIndex]-cpixelnew[rowIndex][colIndex];
                    errorM=mpixel[rowIndex][colIndex]-mpixelnew[rowIndex][colIndex];
                    errorY=ypixel[rowIndex][colIndex]-ypixelnew[rowIndex][colIndex];
                    if(colIndex<imageWidth-1)
                    {
                        cpixel[rowIndex][colIndex+1]=cpixel[rowIndex][colIndex+1]+((int)((errorC*(float)(7.00/16.00))));
                        mpixel[rowIndex][colIndex+1]=mpixel[rowIndex][colIndex+1]+((int)((errorM*(float)(7.00/16.00))));
                        ypixel[rowIndex][colIndex+1]=ypixel[rowIndex][colIndex+1]+((int)((errorY*(float)(7.00/16.00))));
                    }
                    if(rowIndex<imageHeight-1 && colIndex>0)
                    {
                        cpixel[rowIndex][colIndex+1]=cpixel[rowIndex][colIndex+1]+((int)((errorC*(float)(3.00/16.00))));
                        mpixel[rowIndex][colIndex+1]=mpixel[rowIndex][colIndex+1]+((int)((errorM*(float)(3.00/16.00))));
                        ypixel[rowIndex][colIndex+1]=ypixel[rowIndex][colIndex+1]+((int)((errorY*(float)(3.00/16.00))));
                    }
                    if(rowIndex<imageHeight-1)
                    {
                        cpixel[rowIndex][colIndex+1]=cpixel[rowIndex][colIndex+1]+((int)((errorC*(float)(5.00/16.00))));
                        mpixel[rowIndex][colIndex+1]=mpixel[rowIndex][colIndex+1]+((int)((errorM*(float)(5.00/16.00))));
                        ypixel[rowIndex][colIndex+1]=ypixel[rowIndex][colIndex+1]+((int)((errorY*(float)(5.00/16.00))));
                    }
                    if(rowIndex<imageHeight-1 && colIndex<imageWidth-1)
                    {
                        cpixel[rowIndex][colIndex+1]=cpixel[rowIndex][colIndex+1]+((int)((errorC*(float)(1.00/16.00))));
                        mpixel[rowIndex][colIndex+1]=mpixel[rowIndex][colIndex+1]+((int)((errorM*(float)(1.00/16.00))));
                        ypixel[rowIndex][colIndex+1]=ypixel[rowIndex][colIndex+1]+((int)((errorY*(float)(1.00/16.00))));
                    }

                }
            }
        }
            
    }
        if(rowIndex%2==1)
        {
        for(colIndex=imageWidth-1;colIndex>=0;colIndex--)
        {
            if(((int)rpixel[rowIndex][colIndex]+(int)gpixel[rowIndex][colIndex]) > 255)
            {
                if(((int)gpixel[rowIndex][colIndex]+(int)bpixel[rowIndex][colIndex]) > 255)
                {
                    if(((int)rpixel[rowIndex][colIndex]+(int)gpixel[rowIndex][colIndex]+(int)bpixel[rowIndex][colIndex]) > 510)
                    {
                        //CMYW tetrahedron
                        dist1=(sqrt(pow((mpixel[rowIndex][colIndex]-0),2)+pow((cpixel[rowIndex][colIndex])-255,2)+pow((ypixel[rowIndex][colIndex]-0),2)));//C
                        dist2=(sqrt(pow((mpixel[rowIndex][colIndex]-255),2)+pow((cpixel[rowIndex][colIndex])-0,2)+pow((ypixel[rowIndex][colIndex]-0),2)));//M
                        dist3=(sqrt(pow((mpixel[rowIndex][colIndex]-0),2)+pow((cpixel[rowIndex][colIndex])-0,2)+pow((ypixel[rowIndex][colIndex]-255),2)));//Y
                        dist4=(sqrt(pow((mpixel[rowIndex][colIndex]-0),2)+pow((cpixel[rowIndex][colIndex])-0,2)+pow((ypixel[rowIndex][colIndex]-0),2)));//W
                        min1=min(dist1,dist2);
                        min2=min(min1,dist3);
                        min3=min(min2,dist4);
                        minValue=min3;
                        if(minValue==dist1)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)0;
                        }
                        if(minValue==dist2)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)0;
                        }
                        if(minValue==dist3)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)255;
                        }
                        if(minValue==dist4)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)0;
                        }
                        errorC=cpixel[rowIndex][colIndex]-cpixelnew[rowIndex][colIndex];
                        errorM=mpixel[rowIndex][colIndex]-mpixelnew[rowIndex][colIndex];
                        errorY=ypixel[rowIndex][colIndex]-ypixelnew[rowIndex][colIndex];
                        if(colIndex<imageWidth-1)
                        {
                            cpixel[rowIndex][colIndex-1]=cpixel[rowIndex][colIndex-1]+((int)((errorC*(float)(7.00/16.00))));
                            mpixel[rowIndex][colIndex-1]=mpixel[rowIndex][colIndex-1]+((int)((errorM*(float)(7.00/16.00))));
                            ypixel[rowIndex][colIndex-1]=ypixel[rowIndex][colIndex-1]+((int)((errorY*(float)(7.00/16.00))));
                        }
                        if(rowIndex<imageHeight-1 && colIndex>0)
                        {
                            cpixel[rowIndex+1][colIndex-1]=cpixel[rowIndex+1][colIndex-1]+((int)((errorC*(float)(3.00/16.00))));
                            mpixel[rowIndex+1][colIndex-1]=mpixel[rowIndex+1][colIndex-1]+((int)((errorM*(float)(3.00/16.00))));
                            ypixel[rowIndex+1][colIndex-1]=ypixel[rowIndex+1][colIndex-1]+((int)((errorY*(float)(3.00/16.00))));
                        }
                        if(rowIndex<imageHeight-1)
                        {
                            cpixel[rowIndex+1][colIndex]=cpixel[rowIndex+1][colIndex]+((int)((errorC*(float)(5.00/16.00))));
                            mpixel[rowIndex+1][colIndex]=mpixel[rowIndex+1][colIndex]+((int)((errorM*(float)(5.00/16.00))));
                            ypixel[rowIndex+1][colIndex]=ypixel[rowIndex+1][colIndex]+((int)((errorY*(float)(5.00/16.00))));
                        }
                        if(rowIndex<imageHeight-1 && colIndex<imageWidth-1)
                        {
                            cpixel[rowIndex+1][colIndex+1]=cpixel[rowIndex+1][colIndex+1]+((int)((errorC*(float)(1.00/16.00))));
                            mpixel[rowIndex+1][colIndex+1]=mpixel[rowIndex+1][colIndex+1]+((int)((errorM*(float)(1.00/16.00))));
                            ypixel[rowIndex+1][colIndex+1]=ypixel[rowIndex+1][colIndex+1]+((int)((errorY*(float)(1.00/16.00))));
                        }
                    }
                    else
                    {
                        //MYGC tetrahedron
                        dist1=(sqrt(pow((mpixel[rowIndex][colIndex]-255),2)+pow((cpixel[rowIndex][colIndex])-0,2)+pow((ypixel[rowIndex][colIndex]-0),2)));//M
                        dist2=(sqrt(pow((mpixel[rowIndex][colIndex]-0),2)+pow((cpixel[rowIndex][colIndex])-0,2)+pow((ypixel[rowIndex][colIndex]-255),2)));//Y
                        dist3=(sqrt(pow((mpixel[rowIndex][colIndex]-0),2)+pow((cpixel[rowIndex][colIndex])-255,2)+pow((ypixel[rowIndex][colIndex]-255),2)));//G
                        dist4=(sqrt(pow((mpixel[rowIndex][colIndex]-0),2)+pow((cpixel[rowIndex][colIndex])-255,2)+pow((ypixel[rowIndex][colIndex]-0),2)));//C
                        min1=min(dist1,dist2);
                        min2=min(min1,dist3);
                        min3=min(min2,dist4);
                        minValue=min3;
                        if(minValue==dist1)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)0;
                        }
                        if(minValue==dist2)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)255;
                        }
                        if(minValue==dist3)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)255;
                        }
                        if(minValue==dist4)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)0;
                        }
                        errorC=cpixel[rowIndex][colIndex]-cpixelnew[rowIndex][colIndex];
                        errorM=mpixel[rowIndex][colIndex]-mpixelnew[rowIndex][colIndex];
                        errorY=ypixel[rowIndex][colIndex]-ypixelnew[rowIndex][colIndex];
                        if(colIndex<imageWidth-1)
                        {
                            cpixel[rowIndex][colIndex-1]=cpixel[rowIndex][colIndex-1]+((int)((errorC*(float)(7.00/16.00))));
                            mpixel[rowIndex][colIndex-1]=mpixel[rowIndex][colIndex-1]+((int)((errorM*(float)(7.00/16.00))));
                            ypixel[rowIndex][colIndex-1]=ypixel[rowIndex][colIndex-1]+((int)((errorY*(float)(7.00/16.00))));
                        }
                        if(rowIndex<imageHeight-1 && colIndex>0)
                        {
                            cpixel[rowIndex+1][colIndex-1]=cpixel[rowIndex+1][colIndex-1]+((int)((errorC*(float)(3.00/16.00))));
                            mpixel[rowIndex+1][colIndex-1]=mpixel[rowIndex+1][colIndex-1]+((int)((errorM*(float)(3.00/16.00))));
                            ypixel[rowIndex+1][colIndex-1]=ypixel[rowIndex+1][colIndex-1]+((int)((errorY*(float)(3.00/16.00))));
                        }
                        if(rowIndex<imageHeight-1)
                        {
                            cpixel[rowIndex+1][colIndex]=cpixel[rowIndex+1][colIndex]+((int)((errorC*(float)(5.00/16.00))));
                            mpixel[rowIndex+1][colIndex]=mpixel[rowIndex+1][colIndex]+((int)((errorM*(float)(5.00/16.00))));
                            ypixel[rowIndex+1][colIndex]=ypixel[rowIndex+1][colIndex]+((int)((errorY*(float)(5.00/16.00))));
                        }
                        if(rowIndex<imageHeight-1 && colIndex<imageWidth-1)
                        {
                            cpixel[rowIndex+1][colIndex+1]=cpixel[rowIndex+1][colIndex+1]+((int)((errorC*(float)(1.00/16.00))));
                            mpixel[rowIndex+1][colIndex+1]=mpixel[rowIndex+1][colIndex+1]+((int)((errorM*(float)(1.00/16.00))));
                            ypixel[rowIndex+1][colIndex+1]=ypixel[rowIndex+1][colIndex+1]+((int)((errorY*(float)(1.00/16.00))));
                        }
                    }
                }
                else //RGMY tetrahedron
                {
                    dist1=(sqrt(pow((mpixel[rowIndex][colIndex]-255),2)+pow((cpixel[rowIndex][colIndex])-0,2)+pow((ypixel[rowIndex][colIndex]-255),2)));//R
                    dist2=(sqrt(pow((mpixel[rowIndex][colIndex]-0),2)+pow((cpixel[rowIndex][colIndex])-255,2)+pow((ypixel[rowIndex][colIndex]-255),2)));//G
                    dist3=(sqrt(pow((mpixel[rowIndex][colIndex]-255),2)+pow((cpixel[rowIndex][colIndex])-0,2)+pow((ypixel[rowIndex][colIndex]-0),2)));//M
                    dist4=(sqrt(pow((mpixel[rowIndex][colIndex]-0),2)+pow((cpixel[rowIndex][colIndex])-0,2)+pow((ypixel[rowIndex][colIndex]-255),2)));//Y
                    min1=min(dist1,dist2);
                    min2=min(min1,dist3);
                    min3=min(min2,dist4);
                    minValue=min3;
                    if(minValue==dist1)
                    {
                        cpixelnew[rowIndex][colIndex]=(unsigned char)0;
                        mpixelnew[rowIndex][colIndex]=(unsigned char)255;
                        ypixelnew[rowIndex][colIndex]=(unsigned char)0;
                    }
                    if(minValue==dist2)
                    {
                        cpixelnew[rowIndex][colIndex]=(unsigned char)255;
                        mpixelnew[rowIndex][colIndex]=(unsigned char)0;
                        ypixelnew[rowIndex][colIndex]=(unsigned char)255;
                    }
                    if(minValue==dist3)
                    {
                        cpixelnew[rowIndex][colIndex]=(unsigned char)0;
                        mpixelnew[rowIndex][colIndex]=(unsigned char)255;
                        ypixelnew[rowIndex][colIndex]=(unsigned char)0;
                    }
                    if(minValue==dist4)
                    {
                        cpixelnew[rowIndex][colIndex]=(unsigned char)0;
                        mpixelnew[rowIndex][colIndex]=(unsigned char)0;
                        ypixelnew[rowIndex][colIndex]=(unsigned char)255;
                    }
                    errorC=cpixel[rowIndex][colIndex]-cpixelnew[rowIndex][colIndex];
                    errorM=mpixel[rowIndex][colIndex]-mpixelnew[rowIndex][colIndex];
                    errorY=ypixel[rowIndex][colIndex]-ypixelnew[rowIndex][colIndex];
                    if(colIndex<imageWidth-1)
                    {
                        cpixel[rowIndex][colIndex-1]=cpixel[rowIndex][colIndex-1]+((int)((errorC*(float)(7.00/16.00))));
                        mpixel[rowIndex][colIndex-1]=mpixel[rowIndex][colIndex-1]+((int)((errorM*(float)(7.00/16.00))));
                        ypixel[rowIndex][colIndex-1]=ypixel[rowIndex][colIndex-1]+((int)((errorY*(float)(7.00/16.00))));
                    }
                    if(rowIndex<imageHeight-1 && colIndex>0)
                    {
                        cpixel[rowIndex+1][colIndex-1]=cpixel[rowIndex+1][colIndex-1]+((int)((errorC*(float)(3.00/16.00))));
                        mpixel[rowIndex+1][colIndex-1]=mpixel[rowIndex+1][colIndex-1]+((int)((errorM*(float)(3.00/16.00))));
                        ypixel[rowIndex+1][colIndex-1]=ypixel[rowIndex+1][colIndex-1]+((int)((errorY*(float)(3.00/16.00))));
                    }
                    if(rowIndex<imageHeight-1)
                    {
                        cpixel[rowIndex+1][colIndex]=cpixel[rowIndex+1][colIndex]+((int)((errorC*(float)(5.00/16.00))));
                        mpixel[rowIndex+1][colIndex]=mpixel[rowIndex+1][colIndex]+((int)((errorM*(float)(5.00/16.00))));
                        ypixel[rowIndex+1][colIndex]=ypixel[rowIndex+1][colIndex]+((int)((errorY*(float)(5.00/16.00))));
                    }
                    if(rowIndex<imageHeight-1 && colIndex<imageWidth-1)
                    {
                        cpixel[rowIndex+1][colIndex+1]=cpixel[rowIndex+1][colIndex+1]+((int)((errorC*(float)(1.00/16.00))));
                        mpixel[rowIndex+1][colIndex+1]=mpixel[rowIndex+1][colIndex+1]+((int)((errorM*(float)(1.00/16.00))));
                        ypixel[rowIndex+1][colIndex+1]=ypixel[rowIndex+1][colIndex+1]+((int)((errorY*(float)(1.00/16.00))));
                    }
                    
                }
            }
            else
            {
                if(!((gpixel[rowIndex][colIndex]+bpixel[rowIndex][colIndex]) > 255))
                {
                    if(!((rpixel[rowIndex][colIndex]+gpixel[rowIndex][colIndex]+bpixel[rowIndex][colIndex]) > 255))
                    {
                        //KRGB tetrahedron
                        dist1=(sqrt(pow((mpixel[rowIndex][colIndex]-255),2)+pow((cpixel[rowIndex][colIndex])-255,2)+pow((ypixel[rowIndex][colIndex]-255),2)));//K
                        dist2=(sqrt(pow((mpixel[rowIndex][colIndex]-255),2)+pow((cpixel[rowIndex][colIndex])-0,2)+pow((ypixel[rowIndex][colIndex]-255),2)));//R
                        dist3=(sqrt(pow((mpixel[rowIndex][colIndex]-0),2)+pow((cpixel[rowIndex][colIndex])-255,2)+pow((ypixel[rowIndex][colIndex]-255),2)));//G
                        dist4=(sqrt(pow((mpixel[rowIndex][colIndex]-255),2)+pow((cpixel[rowIndex][colIndex])-255,2)+pow((ypixel[rowIndex][colIndex]-0),2)));//B
                        min1=min(dist1,dist2);
                        min2=min(min1,dist3);
                        min3=min(min2,dist4);
                        minValue=min3;
                        if(minValue==dist1)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)255;
                        }
                        if(minValue==dist2)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)255;
                        }
                        if(minValue==dist3)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)255;
                        }
                        if(minValue==dist4)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)0;
                        }
                        errorC=cpixel[rowIndex][colIndex]-cpixelnew[rowIndex][colIndex];
                        errorM=mpixel[rowIndex][colIndex]-mpixelnew[rowIndex][colIndex];
                        errorY=ypixel[rowIndex][colIndex]-ypixelnew[rowIndex][colIndex];
                        if(colIndex<imageWidth-1)
                        {                            cpixel[rowIndex][colIndex-1]=cpixel[rowIndex][colIndex-1]+((int)((errorC*(float)(7.00/16.00))));
                            mpixel[rowIndex][colIndex-1]=mpixel[rowIndex][colIndex-1]+((int)((errorM*(float)(7.00/16.00))));
                            ypixel[rowIndex][colIndex-1]=ypixel[rowIndex][colIndex-1]+((int)((errorY*(float)(7.00/16.00))));
                        }
                        if(rowIndex<imageHeight-1 && colIndex>0)
                        {
                            cpixel[rowIndex+1][colIndex-1]=cpixel[rowIndex+1][colIndex-1]+((int)((errorC*(float)(3.00/16.00))));
                            mpixel[rowIndex+1][colIndex-1]=mpixel[rowIndex+1][colIndex-1]+((int)((errorM*(float)(3.00/16.00))));
                            ypixel[rowIndex+1][colIndex-1]=ypixel[rowIndex+1][colIndex-1]+((int)((errorY*(float)(3.00/16.00))));
                        }
                        if(rowIndex<imageHeight-1)
                        {
                            cpixel[rowIndex+1][colIndex]=cpixel[rowIndex+1][colIndex]+((int)((errorC*(float)(5.00/16.00))));
                            mpixel[rowIndex+1][colIndex]=mpixel[rowIndex+1][colIndex]+((int)((errorM*(float)(5.00/16.00))));
                            ypixel[rowIndex+1][colIndex]=ypixel[rowIndex+1][colIndex]+((int)((errorY*(float)(5.00/16.00))));
                        }
                        if(rowIndex<imageHeight-1 && colIndex<imageWidth-1)
                        {
                            cpixel[rowIndex+1][colIndex+1]=cpixel[rowIndex+1][colIndex+1]+((int)((errorC*(float)(1.00/16.00))));
                            mpixel[rowIndex+1][colIndex+1]=mpixel[rowIndex+1][colIndex+1]+((int)((errorM*(float)(1.00/16.00))));
                            ypixel[rowIndex+1][colIndex+1]=ypixel[rowIndex+1][colIndex+1]+((int)((errorY*(float)(1.00/16.00))));
                        }
                        
                        
                    }
                    else
                    {
                        //RGBM tetrahedron
                        dist1=(sqrt(pow((mpixel[rowIndex][colIndex]-255),2)+pow((cpixel[rowIndex][colIndex])-0,2)+pow((ypixel[rowIndex][colIndex]-255),2)));//R
                        dist2=(sqrt(pow((mpixel[rowIndex][colIndex]-0),2)+pow((cpixel[rowIndex][colIndex])-255,2)+pow((ypixel[rowIndex][colIndex]-255),2)));//G
                        dist3=(sqrt(pow((mpixel[rowIndex][colIndex]-255),2)+pow((cpixel[rowIndex][colIndex])-255,2)+pow((ypixel[rowIndex][colIndex]-0),2)));//B
                        dist4=(sqrt(pow((mpixel[rowIndex][colIndex]-255),2)+pow((cpixel[rowIndex][colIndex])-0,2)+pow((ypixel[rowIndex][colIndex]-0),2)));//M
                        min1=min(dist1,dist2);
                        min2=min(min1,dist3);
                        min3=min(min2,dist4);
                        minValue=min3;
                        if(minValue==dist1)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)255;
                        }
                        if(minValue==dist2)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)255;
                        }
                        if(minValue==dist3)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)0;
                        }
                        if(minValue==dist4)
                        {
                            cpixelnew[rowIndex][colIndex]=(unsigned char)0;
                            mpixelnew[rowIndex][colIndex]=(unsigned char)255;
                            ypixelnew[rowIndex][colIndex]=(unsigned char)0;
                        }
                        errorC=cpixel[rowIndex][colIndex]-cpixelnew[rowIndex][colIndex];
                        errorM=mpixel[rowIndex][colIndex]-mpixelnew[rowIndex][colIndex];
                        errorY=ypixel[rowIndex][colIndex]-ypixelnew[rowIndex][colIndex];
                        if(colIndex<imageWidth-1)
                        {
                            cpixel[rowIndex][colIndex-1]=cpixel[rowIndex][colIndex-1]+((int)((errorC*(float)(7.00/16.00))));
                            mpixel[rowIndex][colIndex-1]=mpixel[rowIndex][colIndex-1]+((int)((errorM*(float)(7.00/16.00))));
                            ypixel[rowIndex][colIndex-1]=ypixel[rowIndex][colIndex-1]+((int)((errorY*(float)(7.00/16.00))));
                        }
                        if(rowIndex<imageHeight-1 && colIndex>0)
                        {
                            cpixel[rowIndex+1][colIndex-1]=cpixel[rowIndex+1][colIndex-1]+((int)((errorC*(float)(3.00/16.00))));
                            mpixel[rowIndex+1][colIndex-1]=mpixel[rowIndex+1][colIndex-1]+((int)((errorM*(float)(3.00/16.00))));
                            ypixel[rowIndex+1][colIndex-1]=ypixel[rowIndex+1][colIndex-1]+((int)((errorY*(float)(3.00/16.00))));
                        }
                        if(rowIndex<imageHeight-1)
                        {
                            cpixel[rowIndex+1][colIndex]=cpixel[rowIndex+1][colIndex]+((int)((errorC*(float)(5.00/16.00))));
                            mpixel[rowIndex+1][colIndex]=mpixel[rowIndex+1][colIndex]+((int)((errorM*(float)(5.00/16.00))));
                            ypixel[rowIndex+1][colIndex]=ypixel[rowIndex+1][colIndex]+((int)((errorY*(float)(5.00/16.00))));
                        }
                        if(rowIndex<imageHeight-1 && colIndex<imageWidth-1)
                        {
                            cpixel[rowIndex+1][colIndex+1]=cpixel[rowIndex+1][colIndex+1]+((int)((errorC*(float)(1.00/16.00))));
                            mpixel[rowIndex+1][colIndex+1]=mpixel[rowIndex+1][colIndex+1]+((int)((errorM*(float)(1.00/16.00))));
                            ypixel[rowIndex+1][colIndex+1]=ypixel[rowIndex+1][colIndex+1]+((int)((errorY*(float)(1.00/16.00))));
                        }
                    }
                }
                else
                {
                    //CMGB tetrahedron
                    dist1=(sqrt(pow((mpixel[rowIndex][colIndex]-0),2)+pow((cpixel[rowIndex][colIndex])-255,2)+pow((ypixel[rowIndex][colIndex]-0),2)));//C
                    dist2=(sqrt(pow((mpixel[rowIndex][colIndex]-255),2)+pow((cpixel[rowIndex][colIndex])-0,2)+pow((ypixel[rowIndex][colIndex]-0),2)));//M
                    dist3=(sqrt(pow((mpixel[rowIndex][colIndex]-0),2)+pow((cpixel[rowIndex][colIndex])-255,2)+pow((ypixel[rowIndex][colIndex]-255),2)));//G
                    dist4=(sqrt(pow((mpixel[rowIndex][colIndex]-255),2)+pow((cpixel[rowIndex][colIndex])-255,2)+pow((ypixel[rowIndex][colIndex]-0),2)));//B
                    min1=min(dist1,dist2);
                    min2=min(min1,dist3);
                    min3=min(min2,dist4);
                    minValue=min3;
                    if(minValue==dist1)
                    {
                        cpixelnew[rowIndex][colIndex]=(unsigned char)255;
                        mpixelnew[rowIndex][colIndex]=(unsigned char)0;
                        ypixelnew[rowIndex][colIndex]=(unsigned char)0;
                    }
                    if(minValue==dist2)
                    {
                        cpixelnew[rowIndex][colIndex]=(unsigned char)0;
                        mpixelnew[rowIndex][colIndex]=(unsigned char)255;
                        ypixelnew[rowIndex][colIndex]=(unsigned char)0;
                    }
                    if(minValue==dist3)
                    {
                        cpixelnew[rowIndex][colIndex]=(unsigned char)255;
                        mpixelnew[rowIndex][colIndex]=(unsigned char)0;
                        ypixelnew[rowIndex][colIndex]=(unsigned char)255;
                    }
                    if(minValue==dist4)
                    {
                        cpixelnew[rowIndex][colIndex]=(unsigned char)255;
                        mpixelnew[rowIndex][colIndex]=(unsigned char)255;
                        ypixelnew[rowIndex][colIndex]=(unsigned char)0;
                    }
                    errorC=cpixel[rowIndex][colIndex]-cpixelnew[rowIndex][colIndex];
                    errorM=mpixel[rowIndex][colIndex]-mpixelnew[rowIndex][colIndex];
                    errorY=ypixel[rowIndex][colIndex]-ypixelnew[rowIndex][colIndex];
                    if(colIndex<imageWidth-1)
                    {
                        cpixel[rowIndex][colIndex-1]=cpixel[rowIndex][colIndex-1]+((int)((errorC*(float)(7.00/16.00))));
                        mpixel[rowIndex][colIndex-1]=mpixel[rowIndex][colIndex-1]+((int)((errorM*(float)(7.00/16.00))));
                        ypixel[rowIndex][colIndex-1]=ypixel[rowIndex][colIndex-1]+((int)((errorY*(float)(7.00/16.00))));
                    }
                    if(rowIndex<imageHeight-1 && colIndex>0)
                    {
                        cpixel[rowIndex+1][colIndex-1]=cpixel[rowIndex+1][colIndex-1]+((int)((errorC*(float)(3.00/16.00))));
                        mpixel[rowIndex+1][colIndex-1]=mpixel[rowIndex+1][colIndex-1]+((int)((errorM*(float)(3.00/16.00))));
                        ypixel[rowIndex+1][colIndex-1]=ypixel[rowIndex+1][colIndex-1]+((int)((errorY*(float)(3.00/16.00))));
                    }
                    if(rowIndex<imageHeight-1)
                    {
                        cpixel[rowIndex+1][colIndex]=cpixel[rowIndex+1][colIndex]+((int)((errorC*(float)(5.00/16.00))));
                        mpixel[rowIndex+1][colIndex]=mpixel[rowIndex+1][colIndex]+((int)((errorM*(float)(5.00/16.00))));
                        ypixel[rowIndex+1][colIndex]=ypixel[rowIndex+1][colIndex]+((int)((errorY*(float)(5.00/16.00))));
                    }
                    if(rowIndex<imageHeight-1 && colIndex<imageWidth-1)
                    {
                        cpixel[rowIndex+1][colIndex+1]=cpixel[rowIndex+1][colIndex+1]+((int)((errorC*(float)(1.00/16.00))));
                        mpixel[rowIndex+1][colIndex+1]=mpixel[rowIndex+1][colIndex+1]+((int)((errorM*(float)(1.00/16.00))));
                        ypixel[rowIndex+1][colIndex+1]=ypixel[rowIndex+1][colIndex+1]+((int)((errorY*(float)(1.00/16.00))));
                    }
                }
            }
        }
        }
    }
}
void getCMY(unsigned char** rpixel2D,unsigned char** Cpixel2D,int imageHeight,int imageWidth)
{
    int rowIndex,colIndex;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            Cpixel2D[rowIndex][colIndex]=(unsigned char)(255-(int)rpixel2D[rowIndex][colIndex]);
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
    unsigned char combinedFloyd[imageHeight*imageWidth*bytesPerPixel];
    unsigned char combinedMBVQ[imageHeight*imageWidth*bytesPerPixel];
    
    unsigned char** rpixel2Ddummy=new  unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** rpixel2D=rpixel2Ddummy;
    
    unsigned char** rpixel2Ddummy4=new  unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy4[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy4[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** gpixel2D=rpixel2Ddummy4;
    
    unsigned char** rpixel2Ddummy5=new  unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy5[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy5[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** bpixel2D=rpixel2Ddummy5;
    
    unsigned char** rpixel2Ddummy1=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy1[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy1[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** rpixel2Dfloyd=rpixel2Ddummy1;
    
    unsigned char** rpixel2Ddummy12=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy12[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy12[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** gpixel2Dfloyd=rpixel2Ddummy12;
    
    unsigned char** rpixel2Ddummy19=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy19[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy19[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** bpixel2Dfloyd=rpixel2Ddummy19;
    
    unsigned char** rpixel2Ddummy122=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy122[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy122[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** rpixel2DMBVQ=rpixel2Ddummy122;
    
    unsigned char** rpixel2Ddummy123=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy123[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy123[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** gpixel2DMBVQ=rpixel2Ddummy123;
    
    unsigned char** rpixel2Ddummy193=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy193[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy193[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** bpixel2DMBVQ=rpixel2Ddummy193;
    
    unsigned char** rpixel2Ddummy2=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy2[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy2[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** cpixel=rpixel2Ddummy2;
    
    unsigned char** rpixel2Ddummy6=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy6[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy6[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** mpixel=rpixel2Ddummy6;
    
    unsigned char** rpixel2Ddummy45=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy45[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy45[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** ypixel=rpixel2Ddummy45;
    
    unsigned char** rpixel2Ddummy23=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy23[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy23[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** cpixelcopy=rpixel2Ddummy23;
    
    unsigned char** rpixel2Ddummy63=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy63[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy63[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** mpixelcopy=rpixel2Ddummy63;
    
    unsigned char** rpixel2Ddummy453=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy453[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy453[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** ypixelcopy=rpixel2Ddummy453;
    
    unsigned char** rpixel2Ddummy25=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy25[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy25[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** cpixelfloyd=rpixel2Ddummy25;
    
    unsigned char** rpixel2Ddummy66=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy66[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy66[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** mpixelfloyd=rpixel2Ddummy66;
    
    unsigned char** rpixel2Ddummy49=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy49[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy49[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** ypixelfloyd=rpixel2Ddummy49;
    
    unsigned char** rpixel2Ddummy254=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy254[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy254[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** cpixelnew=rpixel2Ddummy254;
    
    unsigned char** rpixel2Ddummy664=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy664[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy664[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** mpixelnew=rpixel2Ddummy664;
    
    unsigned char** rpixel2Ddummy493=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy493[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy493[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** ypixelnew=rpixel2Ddummy493;
    //Converting the 1D input image into R,G,B channels
    get2Dchannel(Imagedata,imageHeight,imageWidth,rpixel2D,0);
    get2Dchannel(Imagedata,imageHeight,imageWidth,gpixel2D,1);
    get2Dchannel(Imagedata,imageHeight,imageWidth,bpixel2D,2);
    
    //Getting C,M,Y values wherein C=255 - R, M=255 - G, Y=255 - B
    getCMY(rpixel2D,cpixel,imageHeight,imageWidth);
    getCMY(gpixel2D,mpixel,imageHeight,imageWidth);
    getCMY(bpixel2D,ypixel,imageHeight,imageWidth);
    
    //Seperable Error diffusion using floyd filter
    getfloydmatrix(cpixel,imageHeight,imageWidth,cpixelfloyd);
    getfloydmatrix(mpixel,imageHeight,imageWidth,mpixelfloyd);
    getfloydmatrix(ypixel,imageHeight,imageWidth,ypixelfloyd);
    
    //Converting C,M,Y values back to R,G,B values
    getCMY(cpixelfloyd,rpixel2Dfloyd,imageHeight,imageWidth);
    getCMY(mpixelfloyd,gpixel2Dfloyd,imageHeight,imageWidth);
    getCMY(ypixelfloyd,bpixel2Dfloyd,imageHeight,imageWidth);
    
    //Combining R,G,B channels into a single output image for seperable error diffusion method
    getcombinechannels(rpixel2Dfloyd,combinedFloyd,imageHeight,imageWidth,0);
    getcombinechannels(gpixel2Dfloyd,combinedFloyd,imageHeight,imageWidth,1);
    getcombinechannels(bpixel2Dfloyd,combinedFloyd,imageHeight,imageWidth,2);
    
    //Getting C,M,Y values wherein C=255 - R, M=255 - G, Y=255 - B
    getCMY(rpixel2D,cpixelcopy,imageHeight,imageWidth);
    getCMY(gpixel2D,mpixelcopy,imageHeight,imageWidth);
    getCMY(bpixel2D,ypixelcopy,imageHeight,imageWidth);
    
    //Performing MBVQ error diffusion
    getMBVQ(rpixel2D,gpixel2D,bpixel2D,cpixelcopy,mpixelcopy,ypixelcopy,cpixelnew,mpixelnew,ypixelnew,imageHeight,imageWidth);
    
    //Converting C,M,Y back to R,G,B values
    getCMY(cpixelnew,rpixel2DMBVQ,imageHeight,imageWidth);
    getCMY(mpixelnew,gpixel2DMBVQ,imageHeight,imageWidth);
    getCMY(ypixelnew,bpixel2DMBVQ,imageHeight,imageWidth);
    
    //Combining R,G,B channels into a single output image for MBVQ error diffusion method
    getcombinechannels(rpixel2DMBVQ,combinedMBVQ,imageHeight,imageWidth,0);
    getcombinechannels(gpixel2DMBVQ,combinedMBVQ,imageHeight,imageWidth,1);
    getcombinechannels(bpixel2DMBVQ,combinedMBVQ,imageHeight,imageWidth,2);
    
    // Write image data (filename specified by second argument) from image data matrix
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    //fwrite(combinedMBVQ, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fwrite(combinedFloyd, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fclose(file);
    return 0;
}





