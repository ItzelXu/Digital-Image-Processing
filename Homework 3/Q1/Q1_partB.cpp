//
//  main.cpp
//  Assignment3_Q1b
//
//  Created by Deepika Kanade on 3/17/18.
//  Copyright © 2018 Deepika Kanade. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <cmath>

using namespace std;

void get2Dchannel(double* Imagedata,int imageHeight, int imageWidth,double **rpixel2D, int iterationIndex)
{
    int rowIndex,colIndex;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            rpixel2D[rowIndex][colIndex]=Imagedata[iterationIndex];
            iterationIndex++;
        }
    }
}

void gettensorproduct(double* mask1,double* mask2,double** output)
{
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            output[i][j]=mask1[i]*mask2[j];
        }
    }
}

void getdouble(unsigned char* Imagedata,double* Imagedatanew,int imageDimension)
{
    for(int i=0;i<imageDimension;i++)
    {
        Imagedatanew[i]=(double)(Imagedata[i]);
    }
}

void getLawsfilteredimage(double** rpixel2D,double** filter,int imageHeight,int imageWidth,double** filteredoutputArray)
{
    double** rpixel2Ddummy41=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy41[i]=new double[imageWidth];
        memset(rpixel2Ddummy41[i],0,imageWidth*sizeof(double));
    }
    double** rpixelDCremoved=rpixel2Ddummy41;
    
    for(int i=0;i<imageHeight;i++)
    {
        for(int j=0;j<imageWidth;j++)
        {
            rpixelDCremoved[i][j]=rpixel2D[i][j];
        }
    }
    int i,j;
    int n,x,p,q;
    int Nmean=21;
    double sorted[Nmean*Nmean];
    //Finding the mean for all pixels in the array using a 21x21 window. Keep updating the mean in the mean array
    for(i=0;i<imageHeight;i++)
    {
        for(j=0;j<imageWidth;j++)
        {
            for(x=0;x<Nmean*Nmean;x++)
            {
                sorted[x]=0;
            }
            int k=0;
            for(p=i-(Nmean-1)/2;p<=i+(Nmean-1)/2;p++)
            {
                for(q=j-(Nmean-1)/2;q<=j+(Nmean-1)/2;q++)
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
                    sorted[k]=rpixelDCremoved[rowIndex][colIndex];
                    k++;
                }
            }
            double meanVal=0;
            for(n=0;n<Nmean*Nmean;n++)
            {
                meanVal=meanVal+sorted[n];
            }
            meanVal=meanVal/(21.00*21.00);
            rpixelDCremoved[i][j]=rpixelDCremoved[i][j]-meanVal;
        }
    }
    
    int m=0;
    int N=3;
    double filter1D[N*N];
    
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            filter1D[m]=filter[i][j];
            m++;
        }
    }
    double energyArr[N*N];
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
                    sorted[k]=rpixelDCremoved[rowIndex][colIndex];
                    k++;
                }
            }
            // Finding the energy for each image with respect to all the laws filters
            double energyVal=0;
            for(n=0;n<N*N;n++)
            {
                energyArr[n]=filter1D[n]*sorted[n];
                energyVal=energyVal+energyArr[n];
            }
            filteredoutputArray[i][j]=energyVal;
        }
    }
}

void getEnergy(double** filteredoutput,int imageHeight,int imageWidth,double** energyArray)
{
    int i,j;
    int n,x,p,q;
    int N=21;
    double sorted[N*N];
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
                    sorted[k]=filteredoutput[rowIndex][colIndex];
                    k++;
                }
            }
            double energyVal=0;
            for(n=0;n<N*N;n++)
            {
                energyVal=energyVal+pow(sorted[n],2);
            }
            energyVal=energyVal/(double)(N*N);
            energyArray[i][j]=energyVal;
        }
    }
}

void getKmeansclustering(double** energyArray1,double** energyArray2,double** energyArray3,double** energyArray4,double** energyArray5,double** energyArray6,double** energyArray7,double** energyArray8,double** energyArray9,int* label,int imageHeight,int imageWidth,unsigned char* outputArray)
{
    double** rpixel2Ddummy392=new double* [imageHeight*imageWidth];
    for(int i=0;i<imageHeight*imageWidth;i++)
    {
        rpixel2Ddummy392[i]=new double[9];
        memset(rpixel2Ddummy392[i],0,9*sizeof(double));
    }
    double** featurevector=rpixel2Ddummy392;
    
    double* energyArray1final=new double [imageHeight*imageWidth];
    double* energyArray2final=new double [imageHeight*imageWidth];
    double* energyArray3final=new double [imageHeight*imageWidth];
    double* energyArray4final=new double [imageHeight*imageWidth];
    double* energyArray5final=new double [imageHeight*imageWidth];
    double* energyArray6final=new double [imageHeight*imageWidth];
    double* energyArray7final=new double [imageHeight*imageWidth];
    double* energyArray8final=new double [imageHeight*imageWidth];
    double* energyArray9final=new double [imageHeight*imageWidth];

    
    for(int i=0;i<imageHeight;i++)
    {
        for(int j=0;j<imageWidth;j++)
        {
            energyArray1[i][j]=energyArray1[i][j]/energyArray9[i][j];
            energyArray2[i][j]=energyArray2[i][j]/energyArray9[i][j];
            energyArray3[i][j]=energyArray3[i][j]/energyArray9[i][j];
            energyArray4[i][j]=energyArray4[i][j]/energyArray9[i][j];
            energyArray5[i][j]=energyArray5[i][j]/energyArray9[i][j];
            energyArray6[i][j]=energyArray6[i][j]/energyArray9[i][j];
            energyArray7[i][j]=energyArray7[i][j]/energyArray9[i][j];
            energyArray8[i][j]=energyArray8[i][j]/energyArray9[i][j];
            energyArray9[i][j]=energyArray9[i][j]/energyArray9[i][j];
        }
    }
    int b=0;
    for(int i=0;i<imageHeight;i++)
    {
        for(int j=0;j<imageWidth;j++)
        {
            energyArray1final[b]=energyArray1[i][j];
            energyArray2final[b]=energyArray2[i][j];
            energyArray3final[b]=energyArray3[i][j];
            energyArray4final[b]=energyArray4[i][j];
            energyArray5final[b]=energyArray5[i][j];
            energyArray6final[b]=energyArray6[i][j];
            energyArray7final[b]=energyArray7[i][j];
            energyArray8final[b]=energyArray8[i][j];
            energyArray9final[b]=energyArray9[i][j];
            b++;
        }
    }
    for(int i=0;i<imageHeight*imageWidth;i++)
    {
        featurevector[i][0]=energyArray1final[i];
        featurevector[i][1]=energyArray2final[i];
        featurevector[i][2]=energyArray3final[i];
        featurevector[i][3]=energyArray4final[i];
        featurevector[i][4]=energyArray5final[i];
        featurevector[i][5]=energyArray6final[i];
        featurevector[i][6]=energyArray7final[i];
        featurevector[i][7]=energyArray8final[i];
        featurevector[i][8]=energyArray9final[i];
    }
    //Initializing centroids
    double meanArray[6][9];
    for(int j=0;j<9;j++)
    {
        meanArray[0][j]=featurevector[21095][j];
        meanArray[1][j]=featurevector[24895][j];
        meanArray[2][j]=featurevector[55093][j];
        meanArray[3][j]=featurevector[132891][j];
        meanArray[4][j]=featurevector[222680][j];
        meanArray[5][j]=featurevector[218883][j];
    }
    for(int c=0;c<1;c++)
    {
        memset(label,0,imageHeight*imageWidth*sizeof(int));
        double dist[6];
        double r=0,s=0,t=0,u=0,v=0,w=0;
        for(int i=0;i<imageHeight*imageWidth;i++)
        {
            memset(dist,0,6*sizeof(double));
            //Finding distance of data point from each centroid
            dist[0]=sqrt(pow((featurevector[i][0]-meanArray[0][0]),2)+pow((featurevector[i][1]-meanArray[0][1]),2)+pow((featurevector[i][2]-meanArray[0][2]),2)+pow((featurevector[i][3]-meanArray[0][3]),2)+pow((featurevector[i][4]-meanArray[0][4]),2)+pow((featurevector[i][5]-meanArray[0][5]),2)+pow((featurevector[i][6]-meanArray[0][6]),2)+pow((featurevector[i][7]-meanArray[0][7]),2)+pow((featurevector[i][8]-meanArray[0][8]),2));
            
            dist[1]=sqrt(pow((featurevector[i][0]-meanArray[1][0]),2)+pow((featurevector[i][1]-meanArray[1][1]),2)+pow((featurevector[i][2]-meanArray[1][2]),2)+pow((featurevector[i][3]-meanArray[1][3]),2)+pow((featurevector[i][4]-meanArray[1][4]),2)+pow((featurevector[i][5]-meanArray[1][5]),2)+pow((featurevector[i][6]-meanArray[1][6]),2)+pow((featurevector[i][7]-meanArray[1][7]),2)+pow((featurevector[i][8]-meanArray[1][8]),2));
            
            dist[2]=sqrt(pow((featurevector[i][0]-meanArray[2][0]),2)+pow((featurevector[i][1]-meanArray[2][1]),2)+pow((featurevector[i][2]-meanArray[2][2]),2)+pow((featurevector[i][3]-meanArray[2][3]),2)+pow((featurevector[i][4]-meanArray[2][4]),2)+pow((featurevector[i][5]-meanArray[2][5]),2)+pow((featurevector[i][6]-meanArray[2][6]),2)+pow((featurevector[i][7]-meanArray[2][7]),2)+pow((featurevector[i][8]-meanArray[2][8]),2));
            
            dist[3]=sqrt(pow((featurevector[i][0]-meanArray[3][0]),2)+pow((featurevector[i][1]-meanArray[3][1]),2)+pow((featurevector[i][2]-meanArray[3][2]),2)+pow((featurevector[i][3]-meanArray[3][3]),2)+pow((featurevector[i][4]-meanArray[3][4]),2)+pow((featurevector[i][5]-meanArray[3][5]),2)+pow((featurevector[i][6]-meanArray[3][6]),2)+pow((featurevector[i][7]-meanArray[3][7]),2)+pow((featurevector[i][8]-meanArray[3][8]),2));
            
            dist[4]=sqrt(pow((featurevector[i][0]-meanArray[4][0]),2)+pow((featurevector[i][1]-meanArray[4][1]),2)+pow((featurevector[i][2]-meanArray[4][2]),2)+pow((featurevector[i][3]-meanArray[4][3]),2)+pow((featurevector[i][4]-meanArray[4][4]),2)+pow((featurevector[i][5]-meanArray[4][5]),2)+pow((featurevector[i][6]-meanArray[4][6]),2)+pow((featurevector[i][7]-meanArray[4][7]),2)+pow((featurevector[i][8]-meanArray[4][8]),2));
            
            dist[5]=sqrt(pow((featurevector[i][0]-meanArray[5][0]),2)+pow((featurevector[i][1]-meanArray[5][1]),2)+pow((featurevector[i][2]-meanArray[5][2]),2)+pow((featurevector[i][3]-meanArray[5][3]),2)+pow((featurevector[i][4]-meanArray[5][4]),2)+pow((featurevector[i][5]-meanArray[5][5]),2)+pow((featurevector[i][6]-meanArray[5][6]),2)+pow((featurevector[i][7]-meanArray[5][7]),2)+pow((featurevector[i][8]-meanArray[5][8]),2));
            
            double min=dist[0];
            for(int v=0;v<6;v++)
            {
                if (dist[v]<min)
                    min=dist[v];
            }
            //Determind the minimum distance and assign the data point to that cluster
            if(min==dist[0])
            {
                outputArray[i]=(unsigned char)0;
                label[i]=1;
                r++;
            }
            if(min==dist[1])
            {
                outputArray[i]=(unsigned char)51;
                label[i]=2;
                s++;
            }
            if(min==dist[2])
            {
                outputArray[i]=(unsigned char)102;
                label[i]=3;
                t++;
            }
            if(min==dist[3])
            {
                outputArray[i]=(unsigned char)153;
                label[i]=4;
                u++;
            }
            if(min==dist[4])
            {
                outputArray[i]=(unsigned char)204;
                label[i]=5;
                v++;
            }
            if(min==dist[5])
            {
                outputArray[i]=(unsigned char)255;
                label[i]=6;
                w++;
            }
        }
        //Updating centroids in each iteration
        double sum1[9]={0},sum2[9]={0},sum3[9]={0},sum4[9]={0},sum5[9]={0},sum6[9]={0};
        for(int i=0;i<imageHeight*imageWidth;i++)
        {
            for(int j=0;j<9;j++)
            {
                if(label[i]==1)
                {
                    sum1[j]=sum1[j]+featurevector[i][j];
                }
                else if(label[i]==2)
                {
                    sum2[j]=sum2[j]+featurevector[i][j];
                }
                else if(label[i]==3)
                {
                    sum3[j]=sum3[j]+featurevector[i][j];
                }
                else if(label[i]==4)
                {
                    sum4[j]=sum4[j]+featurevector[i][j];
                }
                else if(label[i]==5)
                {
                    sum5[j]=sum5[j]+featurevector[i][j];
                }
                else if(label[i]==6)
                {
                    sum6[j]=sum6[j]+featurevector[i][j];
                }
            }
        }
        for(int j=0;j<9;j++)
        {
            meanArray[0][j]=(sum1[j]/(double)r);
            meanArray[1][j]=(sum2[j]/(double)s);
            meanArray[2][j]=(sum3[j]/(double)t);
            meanArray[3][j]=(sum4[j]/(double)u);
            meanArray[4][j]=(sum5[j]/(double)v);
            meanArray[5][j]=(sum6[j]/(double)w);
        }
    }
}

void getcombinechannels(double** thresholdedimage,unsigned char* combinedArray,int imageHeight,int imageWidth)
{
    int rowIndex,colIndex,count=0;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            combinedArray[count]=(unsigned char)thresholdedimage[rowIndex][colIndex];
            count++;
        }
    }
}


int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int bytesPerPixel;
    int imageHeight=450;
    int imageWidth=600;
    
    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [bytesPerPixel = 1] [Height=255] [Width=255]" << endl;
        return 0;
    }
    
    // Check if image is grayscale or color
    if (argc < 3){
        bytesPerPixel = 1; // default is grey image
    }
    else {
        bytesPerPixel = atoi(argv[3]);  //atoi converts string characters into integer
        // Check if size is specified
        if (argc >= 4){
            imageWidth= atoi(argv[4]);
            imageHeight= atoi(argv[5]);
        }
    }
    
    // Allocate image data array
    int imageDimension=imageHeight*imageWidth*bytesPerPixel;
    unsigned char Imagedata[imageDimension];
    double  Imagedatanew[imageDimension];
    unsigned char  combinedArray[imageDimension];
    
    // Read image (filename specified by first argument) into image data matrix where 'rb' indicate reading a binary file
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    // format of fread is fread ( void * ptr, size of each clock of memory, count to indicate memory required, FILE to read from);
    fread(Imagedata, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fclose(file);
    
    ///////////////////////// PROCESSING CODE ///////////////////////
    //Declarations for all the arrays
    double E3[3]={-1.0/2.0,0,1.0/2.0};
    double S3[3]={-1.0/2.0,2.0/2.0,-1.0/2.0};
    double L3[3]={1.0/6.0,2.0/6.0,1.0/6.0};
    
    double** rpixel2Ddummy1=new double* [3];
    for(int i=0;i<3;i++)
    {
        rpixel2Ddummy1[i]=new double[3];
        memset(rpixel2Ddummy1[i],0,3*sizeof(double));
    }
    double** E3E3=rpixel2Ddummy1;
    
    double** rpixel2Ddummy11=new double* [3];
    for(int i=0;i<3;i++)
    {
        rpixel2Ddummy11[i]=new double[3];
        memset(rpixel2Ddummy11[i],0,3*sizeof(double));
    }
    double** E3S3=rpixel2Ddummy11;
    
    double** rpixel2Ddummy12=new double* [3];
    for(int i=0;i<3;i++)
    {
        rpixel2Ddummy12[i]=new double[3];
        memset(rpixel2Ddummy12[i],0,3*sizeof(double));
    }
    double** E3L3=rpixel2Ddummy12;
    
    double** rpixel2Ddummy13=new double* [3];
    for(int i=0;i<3;i++)
    {
        rpixel2Ddummy13[i]=new double[3];
        memset(rpixel2Ddummy13[i],0,3*sizeof(double));
    }
    double** S3E3=rpixel2Ddummy13;
    
    double** rpixel2Ddummy14=new double* [3];
    for(int i=0;i<3;i++)
    {
        rpixel2Ddummy14[i]=new double[3];
        memset(rpixel2Ddummy14[i],0,3*sizeof(double));
    }
    double** S3S3=rpixel2Ddummy14;
    
    double** rpixel2Ddummy15=new double* [3];
    for(int i=0;i<3;i++)
    {
        rpixel2Ddummy15[i]=new double[3];
        memset(rpixel2Ddummy15[i],0,3*sizeof(double));
    }
    double** S3L3=rpixel2Ddummy15;
    
    double** rpixel2Ddummy16=new double* [3];
    for(int i=0;i<3;i++)
    {
        rpixel2Ddummy16[i]=new double[3];
        memset(rpixel2Ddummy16[i],0,3*sizeof(double));
    }
    double** L3E3=rpixel2Ddummy16;
    
    double** rpixel2Ddummy17=new double* [3];
    for(int i=0;i<3;i++)
    {
        rpixel2Ddummy17[i]=new double[3];
        memset(rpixel2Ddummy17[i],0,3*sizeof(double));
    }
    double** L3S3=rpixel2Ddummy17;
    
    double** rpixel2Ddummy18=new double* [3];
    for(int i=0;i<3;i++)
    {
        rpixel2Ddummy18[i]=new double[3];
        memset(rpixel2Ddummy18[i],0,3*sizeof(double));
    }
    double** L3L3=rpixel2Ddummy18;
    //
    double** rpixel2Ddummy21=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy21[i]=new double[imageWidth];
        memset(rpixel2Ddummy21[i],0,imageWidth*sizeof(double));
    }
    double** rpixel2D=rpixel2Ddummy21;
    
    double** rpixel2Ddummy22=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy22[i]=new double[imageWidth];
        memset(rpixel2Ddummy22[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput1=rpixel2Ddummy22;

    double** rpixel2Ddummy23=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy23[i]=new double[imageWidth];
        memset(rpixel2Ddummy23[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput2=rpixel2Ddummy23;

    double** rpixel2Ddummy24=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy24[i]=new double[imageWidth];
        memset(rpixel2Ddummy24[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput3=rpixel2Ddummy24;

    double** rpixel2Ddummy25=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy25[i]=new double[imageWidth];
        memset(rpixel2Ddummy25[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput4=rpixel2Ddummy25;

    double** rpixel2Ddummy26=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy26[i]=new double[imageWidth];
        memset(rpixel2Ddummy26[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput5=rpixel2Ddummy26;

    double** rpixel2Ddummy27=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy27[i]=new double[imageWidth];
        memset(rpixel2Ddummy27[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput6=rpixel2Ddummy27;

    double** rpixel2Ddummy28=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy28[i]=new double[imageWidth];
        memset(rpixel2Ddummy28[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput7=rpixel2Ddummy28;

    double** rpixel2Ddummy29=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy29[i]=new double[imageWidth];
        memset(rpixel2Ddummy29[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput8=rpixel2Ddummy29;

    double** rpixel2Ddummy30=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy30[i]=new double[imageWidth];
        memset(rpixel2Ddummy30[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput9=rpixel2Ddummy30;

    double** rpixel2Ddummy31=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy31[i]=new double[imageWidth];
        memset(rpixel2Ddummy31[i],0,imageWidth*sizeof(double));
    }
    double** energyArray1=rpixel2Ddummy31;

    double** rpixel2Ddummy32=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy32[i]=new double[imageWidth];
        memset(rpixel2Ddummy32[i],0,imageWidth*sizeof(double));
    }
    double** energyArray2=rpixel2Ddummy32;

    double** rpixel2Ddummy33=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy33[i]=new double[imageWidth];
        memset(rpixel2Ddummy33[i],0,imageWidth*sizeof(double));
    }
    double** energyArray3=rpixel2Ddummy33;
    
    double** rpixel2Ddummy34=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy34[i]=new double[imageWidth];
        memset(rpixel2Ddummy34[i],0,imageWidth*sizeof(double));
    }
    double** energyArray4=rpixel2Ddummy34;
    
    double** rpixel2Ddummy35=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy35[i]=new double[imageWidth];
        memset(rpixel2Ddummy35[i],0,imageWidth*sizeof(double));
    }
    double** energyArray5=rpixel2Ddummy35;
    
    double** rpixel2Ddummy36=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy36[i]=new double[imageWidth];
        memset(rpixel2Ddummy36[i],0,imageWidth*sizeof(double));
    }
    double** energyArray6=rpixel2Ddummy36;
    
    double** rpixel2Ddummy37=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy37[i]=new double[imageWidth];
        memset(rpixel2Ddummy37[i],0,imageWidth*sizeof(double));
    }
    double** energyArray7=rpixel2Ddummy37;
    
    double** rpixel2Ddummy38=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy38[i]=new double[imageWidth];
        memset(rpixel2Ddummy38[i],0,imageWidth*sizeof(double));
    }
    double** energyArray8=rpixel2Ddummy38;
    
    double** rpixel2Ddummy39=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy39[i]=new double[imageWidth];
        memset(rpixel2Ddummy39[i],0,imageWidth*sizeof(double));
    }
    double** energyArray9=rpixel2Ddummy39;
    
    
    //Get 9 filters using tensor products
    gettensorproduct(E3,E3,E3E3);   //E3E3 Laws filter
    gettensorproduct(E3,S3,E3S3);   //E3S3 Laws filter
    gettensorproduct(E3,L3,E3L3);   //E3W3 Laws filter
    gettensorproduct(S3,E3,S3E3);   //S3E3 Laws filter
    gettensorproduct(S3,S3,S3S3);   //S3S3 Laws filter
    gettensorproduct(S3,L3,S3L3);   //S3W3 Laws filter
    gettensorproduct(L3,E3,L3E3);   //W3E3 Laws filter
    gettensorproduct(L3,S3,L3S3);   //W3S3 Laws filter
    gettensorproduct(L3,L3,L3L3);   //W3W3 Laws filter

    //Converting image in unsigned char to double format for ease of solving
    getdouble(Imagedata,Imagedatanew,imageDimension);
    
    //Convert 1D input image to 2D R channel( the input image is gray scale a image)
    get2Dchannel(Imagedatanew,imageHeight,imageWidth,rpixel2D,0);

    //Get the Laws filtered input images
    getLawsfilteredimage(rpixel2D,E3E3,imageHeight,imageWidth,filteredoutput1);
    getLawsfilteredimage(rpixel2D,E3S3,imageHeight,imageWidth,filteredoutput2);
    getLawsfilteredimage(rpixel2D,E3L3,imageHeight,imageWidth,filteredoutput3);
    getLawsfilteredimage(rpixel2D,S3E3,imageHeight,imageWidth,filteredoutput4);
    getLawsfilteredimage(rpixel2D,S3S3,imageHeight,imageWidth,filteredoutput5);
    getLawsfilteredimage(rpixel2D,S3L3,imageHeight,imageWidth,filteredoutput6);
    getLawsfilteredimage(rpixel2D,L3E3,imageHeight,imageWidth,filteredoutput7);
    getLawsfilteredimage(rpixel2D,L3S3,imageHeight,imageWidth,filteredoutput8);
    getLawsfilteredimage(rpixel2D,L3L3,imageHeight,imageWidth,filteredoutput9);

    //Find the energy for all Laws filtered images
    getEnergy(filteredoutput1,imageHeight,imageWidth,energyArray1);
    getEnergy(filteredoutput2,imageHeight,imageWidth,energyArray2);
    getEnergy(filteredoutput3,imageHeight,imageWidth,energyArray3);
    getEnergy(filteredoutput4,imageHeight,imageWidth,energyArray4);
    getEnergy(filteredoutput5,imageHeight,imageWidth,energyArray5);
    getEnergy(filteredoutput6,imageHeight,imageWidth,energyArray6);
    getEnergy(filteredoutput7,imageHeight,imageWidth,energyArray7);
    getEnergy(filteredoutput8,imageHeight,imageWidth,energyArray8);
    getEnergy(filteredoutput9,imageHeight,imageWidth,energyArray9);

    int *label=new int [imageHeight*imageWidth];
    unsigned char* outputArray=new unsigned char[imageHeight*imageWidth];

    // Apply the K-means clustering algorithm to segment the image into 6 categories
getKmeansclustering(energyArray1,energyArray2,energyArray3,energyArray4,energyArray5,energyArray6,energyArray7,energyArray8,energyArray9,label,imageHeight,imageWidth,outputArray);

    for(int i=0;i<imageHeight*imageWidth*bytesPerPixel;i++)
    {
        combinedArray[i]=outputArray[i];
    }
    
    
    //Combining output array into 1 array
    //getcombinechannels(filteredoutput6,combinedArray,imageHeight,imageWidth);
    
    // Write image data (filename specified by second argument) from image data matrix
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    //fwrite(outputArray, sizeof(unsigned char), imageHeight*imageWidth, file);
    fwrite(combinedArray, sizeof(unsigned char), imageHeight*imageWidth, file);
    //fwrite(luminosityArray, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fclose(file);
    return 0;
}





