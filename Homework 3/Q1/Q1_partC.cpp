//
//  main.cpp
//  Ass3_Q1c
//
//  Created by Deepika Kanade on 3/22/18.
//  Copyright © 2018 Deepika Kanade. All rights reserved.
//
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <fstream> 

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
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<5;j++)
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
    int N=5;
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
            double energyVal=0;
            // Finding the energy for each image with respect to all the laws filters
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

void getKmeansclustering(double** energyArray1,double** energyArray2,double** energyArray3,double** energyArray4,double** energyArray5,double** energyArray6,double** energyArray7,double** energyArray8,double** energyArray9,double** energyArray10,double** energyArray11,double** energyArray12,double** energyArray13,double** energyArray14,double** energyArray15,double** energyArray16,double** energyArray17,double** energyArray18,double** energyArray19,double** energyArray20,double** energyArray21,double** energyArray22,double** energyArray23,double** energyArray24,double** energyArray25,int* label,int imageHeight,int imageWidth,unsigned char* outputArray,string fileName,string fileNameNew)
{
    double** rpixel2Ddummy392=new double* [imageHeight*imageWidth];
    for(int i=0;i<imageHeight*imageWidth;i++)
    {
        rpixel2Ddummy392[i]=new double[25];
        memset(rpixel2Ddummy392[i],0,25*sizeof(double));
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
    double* energyArray10final=new double [imageHeight*imageWidth];
    double* energyArray11final=new double [imageHeight*imageWidth];
    double* energyArray12final=new double [imageHeight*imageWidth];
    double* energyArray13final=new double [imageHeight*imageWidth];
    double* energyArray14final=new double [imageHeight*imageWidth];
    double* energyArray15final=new double [imageHeight*imageWidth];
    double* energyArray16final=new double [imageHeight*imageWidth];
    double* energyArray17final=new double [imageHeight*imageWidth];
    double* energyArray18final=new double [imageHeight*imageWidth];
    double* energyArray19final=new double [imageHeight*imageWidth];
    double* energyArray20final=new double [imageHeight*imageWidth];
    double* energyArray21final=new double [imageHeight*imageWidth];
    double* energyArray22final=new double [imageHeight*imageWidth];
    double* energyArray23final=new double [imageHeight*imageWidth];
    double* energyArray24final=new double [imageHeight*imageWidth];
    double* energyArray25final=new double [imageHeight*imageWidth];

    
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
            energyArray10final[b]=energyArray10[i][j];
            energyArray11final[b]=energyArray11[i][j];
            energyArray12final[b]=energyArray12[i][j];
            energyArray13final[b]=energyArray13[i][j];
            energyArray14final[b]=energyArray14[i][j];
            energyArray15final[b]=energyArray15[i][j];
            energyArray16final[b]=energyArray16[i][j];
            energyArray17final[b]=energyArray17[i][j];
            energyArray18final[b]=energyArray18[i][j];
            energyArray19final[b]=energyArray19[i][j];
            energyArray20final[b]=energyArray20[i][j];
            energyArray21final[b]=energyArray21[i][j];
            energyArray22final[b]=energyArray22[i][j];
            energyArray23final[b]=energyArray23[i][j];
            energyArray24final[b]=energyArray24[i][j];
            energyArray25final[b]=energyArray25[i][j];
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
        featurevector[i][9]=energyArray10final[i];
        featurevector[i][10]=energyArray11final[i];
        featurevector[i][11]=energyArray12final[i];
        featurevector[i][12]=energyArray13final[i];
        featurevector[i][13]=energyArray14final[i];
        featurevector[i][14]=energyArray15final[i];
        featurevector[i][15]=energyArray16final[i];
        featurevector[i][16]=energyArray17final[i];
        featurevector[i][17]=energyArray18final[i];
        featurevector[i][18]=energyArray19final[i];
        featurevector[i][19]=energyArray20final[i];
        featurevector[i][20]=energyArray21final[i];
        featurevector[i][21]=energyArray22final[i];
        featurevector[i][22]=energyArray23final[i];
        featurevector[i][23]=energyArray24final[i];
        featurevector[i][24]=energyArray25final[i];
    }
    //Reducing dimensions using Matlab code
    //Writing values in file for Matlab processing
    FILE *File;
    File=fopen(fileName.c_str(),"w");
    
    if(File!=nullptr)
    {
        for(int i=0;i<imageHeight*imageWidth;i++)
        {
            for(int j=0;j<25;j++)
            {
                fprintf(File,"%lf\t",featurevector[i][j]);
            }
            fprintf(File,"\n");
        }
    }
    fclose(File);
    
    //Reading file from Matlab
    int row=0,col=0;
    double x;
    ifstream inFile;
    inFile.open(fileNameNew);
    
    double** rpixel2Ddummy102311=new double* [imageHeight*imageWidth];
    for(int i=0;i<imageHeight*imageWidth;i++)
    {
        rpixel2Ddummy102311[i]=new double[5];
        memset(rpixel2Ddummy102311[i],0,5*sizeof(double));
    }
    double** EnergyArray=rpixel2Ddummy102311;
    
    if (!inFile) {
        cout << "Unable to open file";
        exit(1); // terminate with error
    }
    while (inFile >> x)
    {
        EnergyArray[row][col]=x;
        col++;
        if(col == 5)
        {
            col=0;
            row++;
        }
    }
    inFile.close();
   
    double meanArray[6][5];
    for(int j=0;j<5;j++)
    {
        meanArray[0][j]=EnergyArray[21095][j];
        meanArray[1][j]=EnergyArray[24895][j];
        meanArray[2][j]=EnergyArray[55093][j];
        meanArray[3][j]=EnergyArray[132891][j];
        meanArray[4][j]=EnergyArray[222680][j];
        meanArray[5][j]=EnergyArray[218883][j];
    }
    for(int c=0;c<20;c++)
    {
        memset(label,0,imageHeight*imageWidth*sizeof(int));
        double dist[6];
        double r=0,s=0,t=0,u=0,v=0,w=0;
        for(int i=0;i<imageHeight*imageWidth;i++)
        {
            memset(dist,0,6*sizeof(double));
            dist[0]=sqrt(pow((EnergyArray[i][0]-meanArray[0][0]),2)+pow((EnergyArray[i][1]-meanArray[0][1]),2)+pow((EnergyArray[i][2]-meanArray[0][2]),2)+pow((EnergyArray[i][3]-meanArray[0][3]),2)+pow((EnergyArray[i][4]-meanArray[0][4]),2));

            dist[1]=sqrt(pow((EnergyArray[i][0]-meanArray[1][0]),2)+pow((EnergyArray[i][1]-meanArray[1][1]),2)+pow((EnergyArray[i][2]-meanArray[1][2]),2)+pow((EnergyArray[i][3]-meanArray[1][3]),2)+pow((EnergyArray[i][4]-meanArray[1][4]),2));

            dist[2]=sqrt(pow((EnergyArray[i][0]-meanArray[2][0]),2)+pow((EnergyArray[i][1]-meanArray[2][1]),2)+pow((EnergyArray[i][2]-meanArray[2][2]),2)+pow((EnergyArray[i][3]-meanArray[2][3]),2)+pow((EnergyArray[i][4]-meanArray[2][4]),2));

            dist[3]=sqrt(pow((EnergyArray[i][0]-meanArray[3][0]),2)+pow((EnergyArray[i][1]-meanArray[3][1]),2)+pow((EnergyArray[i][2]-meanArray[3][2]),2)+pow((EnergyArray[i][3]-meanArray[3][3]),2)+pow((EnergyArray[i][4]-meanArray[3][4]),2));

            dist[4]=sqrt(pow((EnergyArray[i][0]-meanArray[4][0]),2)+pow((EnergyArray[i][1]-meanArray[4][1]),2)+pow((EnergyArray[i][2]-meanArray[4][2]),2)+pow((EnergyArray[i][3]-meanArray[4][3]),2)+pow((EnergyArray[i][4]-meanArray[4][4]),2));

            dist[5]=sqrt(pow((EnergyArray[i][0]-meanArray[5][0]),2)+pow((EnergyArray[i][1]-meanArray[5][1]),2)+pow((EnergyArray[i][2]-meanArray[5][2]),2)+pow((EnergyArray[i][3]-meanArray[5][3]),2)+pow((EnergyArray[i][4]-meanArray[5][4]),2));

            double min=dist[0];
            for(int v=0;v<6;v++)
            {
                if (dist[v]<min)
                    min=dist[v];
            }

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
        //Updating centroids
        double sum1[5]={0},sum2[5]={0},sum3[5]={0},sum4[5]={0},sum5[5]={0},sum6[5]={0};
        for(int i=0;i<imageHeight*imageWidth;i++)
        {
            for(int j=0;j<5;j++)
            {
                if(label[i]==1)
                {
                    sum1[j]=sum1[j]+EnergyArray[i][j];
                }
                else if(label[i]==2)
                {
                    sum2[j]=sum2[j]+EnergyArray[i][j];
                }
                else if(label[i]==3)
                {
                    sum3[j]=sum3[j]+EnergyArray[i][j];
                }
                else if(label[i]==4)
                {
                    sum4[j]=sum4[j]+EnergyArray[i][j];
                }
                else if(label[i]==5)
                {
                    sum5[j]=sum5[j]+EnergyArray[i][j];
                }
                else if(label[i]==6)
                {
                    sum6[j]=sum6[j]+EnergyArray[i][j];
                }
            }
        }
        for(int j=0;j<5;j++)
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
    string fileName;
    string fileNameNew;
    
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
            fileName= argv[6];
            fileNameNew=argv[7];
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
    double L5[5]={1.0/16.0,4.0/16.0,6.0/16.0,4.0/16.0,1.0/16.0};
    double E5[5]={-1.0/6.0,-2.0/6.0,0,2.0/6.0,1.0/6.0};
    double S5[5]={-1.0/4.0,0,2.0/4.0,0,-1.0/4.0};
    double W5[5]={-1.0/6.0,2.0/6.0,0,-2.0/6.0,1.0/6.0};
    double R5[5]={1.0/16.0,-4.0/16.0,6.0/16.0,-4.0/16.0,1.0/16.0};
    
    double** rpixel2Ddummy1=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy1[i]=new double[5];
        memset(rpixel2Ddummy1[i],0,5*sizeof(double));
    }
    double** L5L5=rpixel2Ddummy1;
    
    double** rpixel2Ddummy11=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy11[i]=new double[5];
        memset(rpixel2Ddummy11[i],0,5*sizeof(double));
    }
    double** L5E5=rpixel2Ddummy11;
    
    double** rpixel2Ddummy12=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy12[i]=new double[5];
        memset(rpixel2Ddummy12[i],0,5*sizeof(double));
    }
    double** L5S5=rpixel2Ddummy12;
    
    double** rpixel2Ddummy13=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy13[i]=new double[5];
        memset(rpixel2Ddummy13[i],0,5*sizeof(double));
    }
    double** L5W5=rpixel2Ddummy13;
    
    double** rpixel2Ddummy14=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy14[i]=new double[5];
        memset(rpixel2Ddummy14[i],0,5*sizeof(double));
    }
    double** L5R5=rpixel2Ddummy14;
    
    double** rpixel2Ddummy15=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy15[i]=new double[5];
        memset(rpixel2Ddummy15[i],0,5*sizeof(double));
    }
    double** S5L5=rpixel2Ddummy15;
    
    double** rpixel2Ddummy16=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy16[i]=new double[5];
        memset(rpixel2Ddummy16[i],0,5*sizeof(double));
    }
    double** S5E5=rpixel2Ddummy16;
    
    double** rpixel2Ddummy17=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy17[i]=new double[5];
        memset(rpixel2Ddummy17[i],0,5*sizeof(double));
    }
    double** S5S5=rpixel2Ddummy17;
    
    double** rpixel2Ddummy18=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy18[i]=new double[5];
        memset(rpixel2Ddummy18[i],0,5*sizeof(double));
    }
    double** S5W5=rpixel2Ddummy18;
    
    double** rpixel2Ddummy100=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy100[i]=new double[5];
        memset(rpixel2Ddummy100[i],0,5*sizeof(double));
    }
    double** S5R5=rpixel2Ddummy100;
    
    double** rpixel2Ddummy101=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy101[i]=new double[5];
        memset(rpixel2Ddummy101[i],0,5*sizeof(double));
    }
    double** E5L5=rpixel2Ddummy101;
    
    double** rpixel2Ddummy102=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy102[i]=new double[5];
        memset(rpixel2Ddummy102[i],0,5*sizeof(double));
    }
    double** E5E5=rpixel2Ddummy102;
    
    double** rpixel2Ddummy103=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy103[i]=new double[5];
        memset(rpixel2Ddummy103[i],0,5*sizeof(double));
    }
    double** E5S5=rpixel2Ddummy103;
    
    double** rpixel2Ddummy105=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy105[i]=new double[5];
        memset(rpixel2Ddummy105[i],0,5*sizeof(double));
    }
    double** E5W5=rpixel2Ddummy105;
    
    double** rpixel2Ddummy104=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy104[i]=new double[5];
        memset(rpixel2Ddummy104[i],0,5*sizeof(double));
    }
    double** E5R5=rpixel2Ddummy104;
    
    double** rpixel2Ddummy107=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy107[i]=new double[5];
        memset(rpixel2Ddummy107[i],0,5*sizeof(double));
    }
    double** W5L5=rpixel2Ddummy107;
    
    double** rpixel2Ddummy106=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy106[i]=new double[5];
        memset(rpixel2Ddummy106[i],0,5*sizeof(double));
    }
    double** W5E5=rpixel2Ddummy106;
    
    double** rpixel2Ddummy108=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy108[i]=new double[5];
        memset(rpixel2Ddummy108[i],0,5*sizeof(double));
    }
    double** W5S5=rpixel2Ddummy108;
    
    double** rpixel2Ddummy109=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy109[i]=new double[5];
        memset(rpixel2Ddummy109[i],0,5*sizeof(double));
    }
    double** W5W5=rpixel2Ddummy109;
    
    double** rpixel2Ddummy110=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy110[i]=new double[5];
        memset(rpixel2Ddummy110[i],0,5*sizeof(double));
    }
    double** W5R5=rpixel2Ddummy110;
    
    double** rpixel2Ddummy111=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy111[i]=new double[5];
        memset(rpixel2Ddummy111[i],0,5*sizeof(double));
    }
    double** R5L5=rpixel2Ddummy111;
    
    double** rpixel2Ddummy112=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy112[i]=new double[5];
        memset(rpixel2Ddummy112[i],0,5*sizeof(double));
    }
    double** R5E5=rpixel2Ddummy112;
    
    double** rpixel2Ddummy113=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy113[i]=new double[5];
        memset(rpixel2Ddummy113[i],0,5*sizeof(double));
    }
    double** R5S5=rpixel2Ddummy113;
    
    double** rpixel2Ddummy114=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy114[i]=new double[5];
        memset(rpixel2Ddummy114[i],0,5*sizeof(double));
    }
    double** R5W5=rpixel2Ddummy114;
    
    double** rpixel2Ddummy115=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy115[i]=new double[5];
        memset(rpixel2Ddummy115[i],0,5*sizeof(double));
    }
    double** R5R5=rpixel2Ddummy115;
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
    
    double** rpixel2Ddummy301=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy301[i]=new double[imageWidth];
        memset(rpixel2Ddummy301[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput10=rpixel2Ddummy301;
    
    double** rpixel2Ddummy302=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy302[i]=new double[imageWidth];
        memset(rpixel2Ddummy302[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput11=rpixel2Ddummy302;
    
    double** rpixel2Ddummy303=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy303[i]=new double[imageWidth];
        memset(rpixel2Ddummy303[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput12=rpixel2Ddummy303;
    
    double** rpixel2Ddummy304=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy304[i]=new double[imageWidth];
        memset(rpixel2Ddummy304[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput13=rpixel2Ddummy304;
    
    double** rpixel2Ddummy305=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy305[i]=new double[imageWidth];
        memset(rpixel2Ddummy305[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput14=rpixel2Ddummy305;
    
    double** rpixel2Ddummy306=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy306[i]=new double[imageWidth];
        memset(rpixel2Ddummy306[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput15=rpixel2Ddummy306;
    
    double** rpixel2Ddummy307=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy307[i]=new double[imageWidth];
        memset(rpixel2Ddummy307[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput16=rpixel2Ddummy307;
    
    double** rpixel2Ddummy308=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy308[i]=new double[imageWidth];
        memset(rpixel2Ddummy308[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput17=rpixel2Ddummy308;
    
    double** rpixel2Ddummy316=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy316[i]=new double[imageWidth];
        memset(rpixel2Ddummy316[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput18=rpixel2Ddummy316;
    
    double** rpixel2Ddummy309=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy309[i]=new double[imageWidth];
        memset(rpixel2Ddummy309[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput19=rpixel2Ddummy309;
    
    double** rpixel2Ddummy310=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy310[i]=new double[imageWidth];
        memset(rpixel2Ddummy310[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput20=rpixel2Ddummy310;
    
    double** rpixel2Ddummy311=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy311[i]=new double[imageWidth];
        memset(rpixel2Ddummy311[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput21=rpixel2Ddummy311;
    
    double** rpixel2Ddummy312=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy312[i]=new double[imageWidth];
        memset(rpixel2Ddummy312[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput22=rpixel2Ddummy312;
    
    double** rpixel2Ddummy313=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy313[i]=new double[imageWidth];
        memset(rpixel2Ddummy313[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput23=rpixel2Ddummy313;
    
    double** rpixel2Ddummy314=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy314[i]=new double[imageWidth];
        memset(rpixel2Ddummy314[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput24=rpixel2Ddummy314;
    
    double** rpixel2Ddummy315=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy315[i]=new double[imageWidth];
        memset(rpixel2Ddummy315[i],0,imageWidth*sizeof(double));
    }
    double** filteredoutput25=rpixel2Ddummy315;
    
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
    
    double** rpixel2Ddummy391=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy391[i]=new double[imageWidth];
        memset(rpixel2Ddummy391[i],0,imageWidth*sizeof(double));
    }
    double** energyArray10=rpixel2Ddummy391;
    
    double** rpixel2Ddummy392=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy392[i]=new double[imageWidth];
        memset(rpixel2Ddummy392[i],0,imageWidth*sizeof(double));
    }
    double** energyArray11=rpixel2Ddummy392;
    
    double** rpixel2Ddummy393=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy393[i]=new double[imageWidth];
        memset(rpixel2Ddummy393[i],0,imageWidth*sizeof(double));
    }
    double** energyArray12=rpixel2Ddummy393;
    
    double** rpixel2Ddummy394=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy394[i]=new double[imageWidth];
        memset(rpixel2Ddummy394[i],0,imageWidth*sizeof(double));
    }
    double** energyArray13=rpixel2Ddummy394;
    
    double** rpixel2Ddummy395=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy395[i]=new double[imageWidth];
        memset(rpixel2Ddummy395[i],0,imageWidth*sizeof(double));
    }
    double** energyArray14=rpixel2Ddummy395;
    
    double** rpixel2Ddummy396=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy396[i]=new double[imageWidth];
        memset(rpixel2Ddummy396[i],0,imageWidth*sizeof(double));
    }
    double** energyArray15=rpixel2Ddummy396;
    
    double** rpixel2Ddummy397=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy397[i]=new double[imageWidth];
        memset(rpixel2Ddummy397[i],0,imageWidth*sizeof(double));
    }
    double** energyArray16=rpixel2Ddummy397;
    
    double** rpixel2Ddummy398=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy398[i]=new double[imageWidth];
        memset(rpixel2Ddummy398[i],0,imageWidth*sizeof(double));
    }
    double** energyArray17=rpixel2Ddummy398;
    
    double** rpixel2Ddummy399=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy399[i]=new double[imageWidth];
        memset(rpixel2Ddummy399[i],0,imageWidth*sizeof(double));
    }
    double** energyArray18=rpixel2Ddummy399;
    
    double** rpixel2Ddummy3911=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy3911[i]=new double[imageWidth];
        memset(rpixel2Ddummy3911[i],0,imageWidth*sizeof(double));
    }
    double** energyArray19=rpixel2Ddummy3911;
    
    double** rpixel2Ddummy3912=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy3912[i]=new double[imageWidth];
        memset(rpixel2Ddummy3912[i],0,imageWidth*sizeof(double));
    }
    double** energyArray20=rpixel2Ddummy3912;
    
    double** rpixel2Ddummy3913=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy3913[i]=new double[imageWidth];
        memset(rpixel2Ddummy3913[i],0,imageWidth*sizeof(double));
    }
    double** energyArray21=rpixel2Ddummy3913;
    
    double** rpixel2Ddummy3914=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy3914[i]=new double[imageWidth];
        memset(rpixel2Ddummy3914[i],0,imageWidth*sizeof(double));
    }
    double** energyArray22=rpixel2Ddummy3914;
    
    double** rpixel2Ddummy3915=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy3915[i]=new double[imageWidth];
        memset(rpixel2Ddummy3915[i],0,imageWidth*sizeof(double));
    }
    double** energyArray23=rpixel2Ddummy3915;
    
    double** rpixel2Ddummy3916=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy3916[i]=new double[imageWidth];
        memset(rpixel2Ddummy3916[i],0,imageWidth*sizeof(double));
    }
    double** energyArray24=rpixel2Ddummy3916;
    
    double** rpixel2Ddummy3917=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy3917[i]=new double[imageWidth];
        memset(rpixel2Ddummy3917[i],0,imageWidth*sizeof(double));
    }
    double** energyArray25=rpixel2Ddummy3917;
    
    //Get 25 filters using tensor products
    gettensorproduct(L5,L5,L5L5);
    gettensorproduct(L5,E5,L5E5);
    gettensorproduct(L5,S5,L5S5);
    gettensorproduct(L5,W5,L5W5);
    gettensorproduct(L5,R5,L5R5);
    gettensorproduct(E5,L5,E5L5);
    gettensorproduct(E5,E5,E5E5);
    gettensorproduct(E5,S5,E5S5);
    gettensorproduct(E5,W5,E5W5);
    gettensorproduct(E5,R5,E5R5);
    gettensorproduct(S5,L5,S5L5);
    gettensorproduct(S5,E5,S5E5);
    gettensorproduct(S5,S5,S5S5);
    gettensorproduct(S5,W5,S5W5);
    gettensorproduct(S5,R5,S5R5);
    gettensorproduct(W5,L5,W5L5);
    gettensorproduct(W5,E5,W5E5);
    gettensorproduct(W5,S5,W5S5);
    gettensorproduct(W5,W5,W5W5);
    gettensorproduct(W5,R5,W5R5);
    gettensorproduct(R5,L5,R5L5);
    gettensorproduct(R5,E5,R5E5);
    gettensorproduct(R5,S5,R5S5);
    gettensorproduct(R5,W5,R5W5);
    gettensorproduct(R5,R5,R5R5);

    //Converting image in unsigned char to double format for ease of solving
    getdouble(Imagedata,Imagedatanew,imageDimension);
    
    ////Convert 1D input image to 2D R channel( the input image is gray scale a image)
    get2Dchannel(Imagedatanew,imageHeight,imageWidth,rpixel2D,0);
    
    //Get the Laws filtered input images
    getLawsfilteredimage(rpixel2D,L5L5,imageHeight,imageWidth,filteredoutput1);
    getLawsfilteredimage(rpixel2D,L5E5,imageHeight,imageWidth,filteredoutput2);
    getLawsfilteredimage(rpixel2D,L5S5,imageHeight,imageWidth,filteredoutput3);
    getLawsfilteredimage(rpixel2D,L5W5,imageHeight,imageWidth,filteredoutput4);
    getLawsfilteredimage(rpixel2D,L5R5,imageHeight,imageWidth,filteredoutput5);
    getLawsfilteredimage(rpixel2D,E5L5,imageHeight,imageWidth,filteredoutput6);
    getLawsfilteredimage(rpixel2D,E5E5,imageHeight,imageWidth,filteredoutput7);
    getLawsfilteredimage(rpixel2D,E5S5,imageHeight,imageWidth,filteredoutput8);
    getLawsfilteredimage(rpixel2D,E5W5,imageHeight,imageWidth,filteredoutput9);
    getLawsfilteredimage(rpixel2D,E5R5,imageHeight,imageWidth,filteredoutput10);
    getLawsfilteredimage(rpixel2D,S5L5,imageHeight,imageWidth,filteredoutput11);
    getLawsfilteredimage(rpixel2D,S5E5,imageHeight,imageWidth,filteredoutput12);
    getLawsfilteredimage(rpixel2D,S5S5,imageHeight,imageWidth,filteredoutput13);
    getLawsfilteredimage(rpixel2D,S5W5,imageHeight,imageWidth,filteredoutput14);
    getLawsfilteredimage(rpixel2D,S5R5,imageHeight,imageWidth,filteredoutput15);
    getLawsfilteredimage(rpixel2D,W5L5,imageHeight,imageWidth,filteredoutput16);
    getLawsfilteredimage(rpixel2D,W5E5,imageHeight,imageWidth,filteredoutput17);
    getLawsfilteredimage(rpixel2D,W5S5,imageHeight,imageWidth,filteredoutput18);
    getLawsfilteredimage(rpixel2D,W5W5,imageHeight,imageWidth,filteredoutput19);
    getLawsfilteredimage(rpixel2D,W5R5,imageHeight,imageWidth,filteredoutput20);
    getLawsfilteredimage(rpixel2D,R5L5,imageHeight,imageWidth,filteredoutput21);
    getLawsfilteredimage(rpixel2D,R5E5,imageHeight,imageWidth,filteredoutput22);
    getLawsfilteredimage(rpixel2D,R5S5,imageHeight,imageWidth,filteredoutput23);
    getLawsfilteredimage(rpixel2D,R5W5,imageHeight,imageWidth,filteredoutput24);
    getLawsfilteredimage(rpixel2D,R5R5,imageHeight,imageWidth,filteredoutput25);

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
    getEnergy(filteredoutput10,imageHeight,imageWidth,energyArray10);
    getEnergy(filteredoutput11,imageHeight,imageWidth,energyArray11);
    getEnergy(filteredoutput12,imageHeight,imageWidth,energyArray12);
    getEnergy(filteredoutput13,imageHeight,imageWidth,energyArray13);
    getEnergy(filteredoutput14,imageHeight,imageWidth,energyArray14);
    getEnergy(filteredoutput15,imageHeight,imageWidth,energyArray15);
    getEnergy(filteredoutput16,imageHeight,imageWidth,energyArray16);
    getEnergy(filteredoutput17,imageHeight,imageWidth,energyArray17);
    getEnergy(filteredoutput18,imageHeight,imageWidth,energyArray18);
    getEnergy(filteredoutput19,imageHeight,imageWidth,energyArray19);
    getEnergy(filteredoutput20,imageHeight,imageWidth,energyArray20);
    getEnergy(filteredoutput21,imageHeight,imageWidth,energyArray21);
    getEnergy(filteredoutput22,imageHeight,imageWidth,energyArray22);
    getEnergy(filteredoutput23,imageHeight,imageWidth,energyArray23);
    getEnergy(filteredoutput24,imageHeight,imageWidth,energyArray24);
    getEnergy(filteredoutput25,imageHeight,imageWidth,energyArray25);

    int *label=new int [imageHeight*imageWidth];
    unsigned char* outputArray=new unsigned char[imageHeight*imageWidth];
    
   // Apply the K-means clustering algorithm to segment the image into 6 categories
getKmeansclustering(energyArray1,energyArray2,energyArray3,energyArray4,energyArray5,energyArray6,energyArray7,energyArray8,energyArray9,energyArray10,energyArray11,energyArray12,energyArray13,energyArray14,energyArray15,energyArray16,energyArray17,energyArray18,energyArray19,energyArray20,energyArray21,energyArray22,energyArray23,energyArray24,energyArray25,label,imageHeight,imageWidth,outputArray,fileName,fileNameNew);
    
    for(int i=0;i<imageHeight*imageWidth*bytesPerPixel;i++)
    {
        combinedArray[i]=outputArray[i];
    }
    
    //Combining output image into a single Array
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






