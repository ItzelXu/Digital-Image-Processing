//
//  main.cpp
//  Asssignment3_Q1a
//
//  Created by Deepika Kanade on 3/16/18.
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

double getEnergy(double **rpixel2D,double** filter,int imageHeight,int imageWidth)
{
    double mean=0,sum=0;
    int rowIndex,colIndex;
    
    double** rpixel2Ddummy41=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy41[i]=new double[imageWidth];
        memset(rpixel2Ddummy41[i],0,imageWidth*sizeof(double));
    }
    double** rpixelDCremoved=rpixel2Ddummy41;
    
    //Finding the mean for all pixels in the array---Every image will have 1 mean
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            sum=sum+rpixel2D[rowIndex][colIndex];
        }
    }
   
    mean=sum/(double)(imageHeight*imageWidth);
    //Subtracting the mean from the pixel values to eliminate illumination effects
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            rpixelDCremoved[rowIndex][colIndex]=rpixel2D[rowIndex][colIndex]-mean;
        }
    }

    int i,j;
    int n,x,p,q,m=0;
    int N=5;
    double filter1D[N*N];
    
    double** rpixel2Ddummy60=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy60[i]=new double[imageWidth];
        memset(rpixel2Ddummy60[i],0,imageWidth*sizeof(double));
    }
    double** energyArray=rpixel2Ddummy60;
    
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            filter1D[m]=filter[i][j];
            m++;
        }
    }
    double sorted[N*N],energyArr[N*N];
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
            energyArray[i][j]=energyVal;
        }
    }
    sum=0;
    for(i=0;i<imageHeight;i++)
    {
        for(j=0;j<imageWidth;j++)
        {
            sum=sum+pow(energyArray[i][j],2);
        }
    }
    sum=sum/(imageHeight*imageWidth);
    return(sum);
}

void getKmeansclustering(double** energyArray,int* label,string* labelName,string fileName)
{
    //For displaying histogram for discriminant analysis
    FILE *File;
    File=fopen(fileName.c_str(),"w");
    
    if(File!=nullptr)
    {
        for(int i=0;i<12;i++)
        {
            for(int j=0;j<9;j++)
            {
                fprintf(File,"%lf\t",energyArray[i][j]);
            }
            fprintf(File,"\n");
        }
    }
    fclose(File);
    
    //Initializing centroids
    double meanArray[4][9]=
    {
        {348.429,186.763,61.3237,185.493,118.002,43.5267,53.9798,38.377,15.3386},{47.5558,31.1735,9.43407,23.504,16.9658,5.81954,5.97848,4.7019,1.85115},{25.6384,10.6018,2.89253,13.0281,5.70674,1.65945,4.12969,1.85916,0.554682},{2.65409,2.32582,1.20472,2.38193,2.27593,1.22917,1.22167,1.24336,0.697996}
    };
    for(int c=0;c<100;c++)
    {
    memset(label,0,12*sizeof(int));
    double dist[4];
    double r=0,s=0,t=0,u=0;
    for(int i=0;i<12;i++)
    {
        memset(dist,0,4*sizeof(double));
        //Finding the distance of each image will 4 means as there are 4 clusters to be formed from the images
        dist[0]=sqrt(pow((energyArray[i][0]-meanArray[0][0]),2)+pow((energyArray[i][1]-meanArray[0][1]),2)+pow((energyArray[i][2]-meanArray[0][2]),2)+pow((energyArray[i][3]-meanArray[0][3]),2)+pow((energyArray[i][4]-meanArray[0][4]),2)+pow((energyArray[i][5]-meanArray[0][5]),2)+pow((energyArray[i][6]-meanArray[0][6]),2)+pow((energyArray[i][7]-meanArray[0][7]),2)+pow((energyArray[i][8]-meanArray[0][8]),2));
        
        dist[1]=sqrt(pow((energyArray[i][0]-meanArray[1][0]),2)+pow((energyArray[i][1]-meanArray[1][1]),2)+pow((energyArray[i][2]-meanArray[1][2]),2)+pow((energyArray[i][3]-meanArray[1][3]),2)+pow((energyArray[i][4]-meanArray[1][4]),2)+pow((energyArray[i][5]-meanArray[1][5]),2)+pow((energyArray[i][6]-meanArray[1][6]),2)+pow((energyArray[i][7]-meanArray[1][7]),2)+pow((energyArray[i][8]-meanArray[1][8]),2));
        
        dist[2]=sqrt(pow((energyArray[i][0]-meanArray[2][0]),2)+pow((energyArray[i][1]-meanArray[2][1]),2)+pow((energyArray[i][2]-meanArray[2][2]),2)+pow((energyArray[i][3]-meanArray[2][3]),2)+pow((energyArray[i][4]-meanArray[2][4]),2)+pow((energyArray[i][5]-meanArray[2][5]),2)+pow((energyArray[i][6]-meanArray[2][6]),2)+pow((energyArray[i][7]-meanArray[2][7]),2)+pow((energyArray[i][8]-meanArray[2][8]),2));
        
        dist[3]=sqrt(pow((energyArray[i][0]-meanArray[3][0]),2)+pow((energyArray[i][1]-meanArray[3][1]),2)+pow((energyArray[i][2]-meanArray[3][2]),2)+pow((energyArray[i][3]-meanArray[3][3]),2)+pow((energyArray[i][4]-meanArray[3][4]),2)+pow((energyArray[i][5]-meanArray[3][5]),2)+pow((energyArray[i][6]-meanArray[3][6]),2)+pow((energyArray[i][7]-meanArray[3][7]),2)+pow((energyArray[i][8]-meanArray[3][8]),2));
        
        double min=dist[0];
        //Assign a label to image based on the minimum distance
        for(int v=0;v<4;v++)
        {
            if (dist[v]<min)
                min=dist[v];
        }
        if(min==dist[0])
        {
            label[i]=1;
            labelName[i]="Rock";
            r++;
        }
        if(min==dist[1])
        {
            label[i]=2;
            labelName[i]="Grass";
            s++;
        }
        if(min==dist[2])
        {
            label[i]=3;
            labelName[i]="Weave";
            t++;
        }
        if(min==dist[3])
        {
            label[i]=4;
            labelName[i]="Sand";
            u++;
        }
    }
    //Updating centroids after each iteration
    double sum1[9]={0},sum2[9]={0},sum3[9]={0},sum4[9]={0};
    for(int i=0;i<12;i++)
    {
        for(int j=0;j<9;j++)
        {
            //Calculating the sum of values of each datapoint in each cluster
            if(label[i]==1)
            {
                sum1[j]=sum1[j]+energyArray[i][j];
            }
            else if(label[i]==2)
            {
                sum2[j]=sum2[j]+energyArray[i][j];
            }
            else if(label[i]==3)
            {
                sum3[j]=sum3[j]+energyArray[i][j];
            }
            else if(label[i]==4)
            {
                sum4[j]=sum4[j]+energyArray[i][j];
            }
        }
   }
        for(int j=0;j<9;j++)
        {
            meanArray[0][j]=(sum1[j]/(double)r);
            meanArray[1][j]=(sum2[j]/(double)s);
            meanArray[2][j]=(sum3[j]/(double)t);
            meanArray[3][j]=(sum4[j]/(double)u);
        }
    }
}

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    FILE *file1;
    FILE *file2;
    FILE *file3;
    FILE *file4;
    FILE *file5;
    FILE *file6;
    FILE *file7;
    FILE *file8;
    FILE *file9;
    FILE *file10;
    FILE *file11;
    FILE *file12;
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
    if (argc < 13){
        bytesPerPixel = 1; // default is grey image
    }
    else {
        bytesPerPixel = atoi(argv[13]);  //atoi converts string characters into integer
        // Check if size is specified
        if (argc >= 5){
            imageWidth= atoi(argv[14]);
            imageHeight= atoi(argv[15]);
            fileName=argv[17];
            
        }
    }
    
    // Allocate image data array
    int imageDimension=imageHeight*imageWidth*bytesPerPixel;
    unsigned char Imagedata1[imageDimension];
    unsigned char Imagedata2[imageDimension];
    unsigned char Imagedata3[imageDimension];
    unsigned char Imagedata4[imageDimension];
    unsigned char Imagedata5[imageDimension];
    unsigned char Imagedata6[imageDimension];
    unsigned char Imagedata7[imageDimension];
    unsigned char Imagedata8[imageDimension];
    unsigned char Imagedata9[imageDimension];
    unsigned char Imagedata10[imageDimension];
    unsigned char Imagedata11[imageDimension];
    unsigned char Imagedata12[imageDimension];
    
    double  Imagedata1new[imageDimension];
    double  Imagedata2new[imageDimension];
    double  Imagedata3new[imageDimension];
    double  Imagedata4new[imageDimension];
    double  Imagedata5new[imageDimension];
    double  Imagedata6new[imageDimension];
    double  Imagedata7new[imageDimension];
    double  Imagedata8new[imageDimension];
    double  Imagedata9new[imageDimension];
    double  Imagedata10new[imageDimension];
    double  Imagedata11new[imageDimension];
    double  Imagedata12new[imageDimension];
    
    double  combinedArray[imageDimension];
    
    // Read images into image data matrices where 'rb' indicate reading a binary file
    if (!(file1=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    if (!(file2=fopen(argv[2],"rb"))) {
        cout << "Cannot open file: " << argv[2] <<endl;
        exit(1);
    }
    if (!(file3=fopen(argv[3],"rb"))) {
        cout << "Cannot open file: " << argv[3] <<endl;
        exit(1);
    }
    if (!(file4=fopen(argv[4],"rb"))) {
        cout << "Cannot open file: " << argv[4] <<endl;
        exit(1);
    }
    if (!(file5=fopen(argv[5],"rb"))) {
        cout << "Cannot open file: " << argv[5] <<endl;
        exit(1);
    }
    if (!(file6=fopen(argv[6],"rb"))) {
        cout << "Cannot open file: " << argv[6] <<endl;
        exit(1);
    }
    if (!(file7=fopen(argv[7],"rb"))) {
        cout << "Cannot open file: " << argv[7] <<endl;
        exit(1);
    }
    if (!(file8=fopen(argv[8],"rb"))) {
        cout << "Cannot open file: " << argv[8] <<endl;
        exit(1);
    }
    if (!(file9=fopen(argv[9],"rb"))) {
        cout << "Cannot open file: " << argv[9] <<endl;
        exit(1);
    }
    if (!(file10=fopen(argv[10],"rb"))) {
        cout << "Cannot open file: " << argv[10] <<endl;
        exit(1);
    }
    if (!(file11=fopen(argv[11],"rb"))) {
        cout << "Cannot open file: " << argv[11] <<endl;
        exit(1);
    }
    if (!(file12=fopen(argv[12],"rb"))) {
        cout << "Cannot open file: " << argv[12] <<endl;
        exit(1);
    }
    // format of fread is fread ( void * ptr, size of each clock of memory, count to indicate memory required, FILE to read from);
    fread(Imagedata1, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file1);
    fread(Imagedata2, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file2);
    fread(Imagedata3, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file3);
    fread(Imagedata4, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file4);
    fread(Imagedata5, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file5);
    fread(Imagedata6, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file6);
    fread(Imagedata7, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file7);
    fread(Imagedata8, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file8);
    fread(Imagedata9, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file9);
    fread(Imagedata10, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file10);
    fread(Imagedata11, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file11);
    fread(Imagedata12, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file12);
    fclose(file1);
    fclose(file2);
    fclose(file3);
    fclose(file4);
    fclose(file5);
    fclose(file6);
    fclose(file7);
    fclose(file8);
    fclose(file9);
    fclose(file10);
    fclose(file11);
    fclose(file12);
    
    ///////////////////////// PROCESSING CODE ///////////////////////
    //Declarations for all the arrays
    double E5[5]={-1.0/6.0,-2.0/6.0,0,2.0/6.0,1.0/6.0};
    double S5[5]={-1.0/4.0,0,2.0/4.0,0,-1.0/4.0};
    double W5[5]={-1.0/6.0,2.0/6.0,0,-2.0/6.0,1.0/6.0};
    
    double** rpixel2Ddummy1=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy1[i]=new double[5];
        memset(rpixel2Ddummy1[i],0,5*sizeof(double));
    }
    double** E5E5=rpixel2Ddummy1;
    
    double** rpixel2Ddummy11=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy11[i]=new double[5];
        memset(rpixel2Ddummy11[i],0,5*sizeof(double));
    }
    double** E5S5=rpixel2Ddummy11;
    
    double** rpixel2Ddummy12=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy12[i]=new double[5];
        memset(rpixel2Ddummy12[i],0,5*sizeof(double));
    }
    double** E5W5=rpixel2Ddummy12;
    
    double** rpixel2Ddummy13=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy13[i]=new double[5];
        memset(rpixel2Ddummy13[i],0,5*sizeof(double));
    }
    double** S5E5=rpixel2Ddummy13;
    
    double** rpixel2Ddummy14=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy14[i]=new double[5];
        memset(rpixel2Ddummy14[i],0,5*sizeof(double));
    }
    double** S5S5=rpixel2Ddummy14;
    
    double** rpixel2Ddummy15=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy15[i]=new double[5];
        memset(rpixel2Ddummy15[i],0,5*sizeof(double));
    }
    double** S5W5=rpixel2Ddummy15;
    
    double** rpixel2Ddummy16=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy16[i]=new double[5];
        memset(rpixel2Ddummy16[i],0,5*sizeof(double));
    }
    double** W5E5=rpixel2Ddummy16;
    
    double** rpixel2Ddummy17=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy17[i]=new double[5];
        memset(rpixel2Ddummy17[i],0,5*sizeof(double));
    }
    double** W5S5=rpixel2Ddummy17;
    
    double** rpixel2Ddummy18=new double* [5];
    for(int i=0;i<5;i++)
    {
        rpixel2Ddummy18[i]=new double[5];
        memset(rpixel2Ddummy18[i],0,5*sizeof(double));
    }
    double** W5W5=rpixel2Ddummy18;
    //
    double** rpixel2Ddummy21=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy21[i]=new double[imageWidth];
        memset(rpixel2Ddummy21[i],0,imageWidth*sizeof(double));
    }
    double** rpixel2D1=rpixel2Ddummy21;
    
    double** rpixel2Ddummy22=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy22[i]=new double[imageWidth];
        memset(rpixel2Ddummy22[i],0,imageWidth*sizeof(double));
    }
    double** rpixel2D2=rpixel2Ddummy22;
    
    double** rpixel2Ddummy23=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy23[i]=new double[imageWidth];
        memset(rpixel2Ddummy23[i],0,imageWidth*sizeof(double));
    }
    double** rpixel2D3=rpixel2Ddummy23;
    
    double** rpixel2Ddummy24=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy24[i]=new double[imageWidth];
        memset(rpixel2Ddummy24[i],0,imageWidth*sizeof(double));
    }
    double** rpixel2D4=rpixel2Ddummy24;
    
    double** rpixel2Ddummy25=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy25[i]=new double[imageWidth];
        memset(rpixel2Ddummy25[i],0,imageWidth*sizeof(double));
    }
    double** rpixel2D5=rpixel2Ddummy25;
    
    double** rpixel2Ddummy26=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy26[i]=new double[imageWidth];
        memset(rpixel2Ddummy26[i],0,imageWidth*sizeof(double));
    }
    double** rpixel2D6=rpixel2Ddummy26;
    
    double** rpixel2Ddummy27=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy27[i]=new double[imageWidth];
        memset(rpixel2Ddummy27[i],0,imageWidth*sizeof(double));
    }
    double** rpixel2D7=rpixel2Ddummy27;
    
    double** rpixel2Ddummy28=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy28[i]=new double[imageWidth];
        memset(rpixel2Ddummy28[i],0,imageWidth*sizeof(double));
    }
    double** rpixel2D8=rpixel2Ddummy28;
    
    double** rpixel2Ddummy29=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy29[i]=new double[imageWidth];
        memset(rpixel2Ddummy29[i],0,imageWidth*sizeof(double));
    }
    double** rpixel2D9=rpixel2Ddummy29;
    
    double** rpixel2Ddummy30=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy30[i]=new double[imageWidth];
        memset(rpixel2Ddummy30[i],0,imageWidth*sizeof(double));
    }
    double** rpixel2D10=rpixel2Ddummy30;
    
    double** rpixel2Ddummy31=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy31[i]=new double[imageWidth];
        memset(rpixel2Ddummy31[i],0,imageWidth*sizeof(double));
    }
    double** rpixel2D11=rpixel2Ddummy31;
    
    double** rpixel2Ddummy32=new double* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy32[i]=new double[imageWidth];
        memset(rpixel2Ddummy32[i],0,imageWidth*sizeof(double));
    }
    double** rpixel2D12=rpixel2Ddummy32;
    
    double energyValue;
    int m=0;
    double** rpixel2Ddummy323=new double* [12];
    for(int i=0;i<12;i++)
    {
        rpixel2Ddummy323[i]=new double[9];
        memset(rpixel2Ddummy323[i],0,9*sizeof(double));
    }
    double** energyArray=rpixel2Ddummy323;
    
    //Get 9 filters using tensor products
    gettensorproduct(E5,E5,E5E5);   //E5E5 Laws filter
    gettensorproduct(E5,S5,E5S5);   //E5S5 Laws filter
    gettensorproduct(E5,W5,E5W5);   //E5W5 Laws filter
    gettensorproduct(S5,E5,S5E5);   //S5E5 Laws filter
    gettensorproduct(S5,S5,S5S5);   //S5S5 Laws filter
    gettensorproduct(S5,W5,S5W5);   //S5W5 Laws filter
    gettensorproduct(W5,E5,W5E5);   //W5E5 Laws filter
    gettensorproduct(W5,S5,W5S5);   //W5S5 Laws filter
    gettensorproduct(W5,W5,W5W5);   //W5W5 Laws filter
    
    //Converting image in unsigned char to double format for ease of solving
    getdouble(Imagedata1,Imagedata1new,imageDimension);
    getdouble(Imagedata2,Imagedata2new,imageDimension);
    getdouble(Imagedata3,Imagedata3new,imageDimension);
    getdouble(Imagedata4,Imagedata4new,imageDimension);
    getdouble(Imagedata5,Imagedata5new,imageDimension);
    getdouble(Imagedata6,Imagedata6new,imageDimension);
    getdouble(Imagedata7,Imagedata7new,imageDimension);
    getdouble(Imagedata8,Imagedata8new,imageDimension);
    getdouble(Imagedata9,Imagedata9new,imageDimension);
    getdouble(Imagedata10,Imagedata10new,imageDimension);
    getdouble(Imagedata11,Imagedata11new,imageDimension);
    getdouble(Imagedata12,Imagedata12new,imageDimension);
    
    //Convert 1D input image to 2D R channel( the input images are gray scale images)
    get2Dchannel(Imagedata1new,imageHeight,imageWidth,rpixel2D1,0);
    get2Dchannel(Imagedata2new,imageHeight,imageWidth,rpixel2D2,0);
    get2Dchannel(Imagedata3new,imageHeight,imageWidth,rpixel2D3,0);
    get2Dchannel(Imagedata4new,imageHeight,imageWidth,rpixel2D4,0);
    get2Dchannel(Imagedata5new,imageHeight,imageWidth,rpixel2D5,0);
    get2Dchannel(Imagedata6new,imageHeight,imageWidth,rpixel2D6,0);
    get2Dchannel(Imagedata7new,imageHeight,imageWidth,rpixel2D7,0);
    get2Dchannel(Imagedata8new,imageHeight,imageWidth,rpixel2D8,0);
    get2Dchannel(Imagedata9new,imageHeight,imageWidth,rpixel2D9,0);
    get2Dchannel(Imagedata10new,imageHeight,imageWidth,rpixel2D10,0);
    get2Dchannel(Imagedata11new,imageHeight,imageWidth,rpixel2D11,0);
    get2Dchannel(Imagedata12new,imageHeight,imageWidth,rpixel2D12,0);
 
    //Extract the energy features for each of the input image using the Laws filters
    energyValue=getEnergy(rpixel2D1,E5E5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,E5S5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,E5W5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,S5E5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,S5S5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,S5W5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,W5E5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,W5S5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D1,W5W5,imageHeight,imageWidth);
    energyArray[0][m]=energyValue;
    m++;

    m=0;
    energyValue=getEnergy(rpixel2D2,E5E5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,E5S5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,E5W5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,S5E5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,S5S5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,S5W5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,W5E5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,W5S5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D2,W5W5,imageHeight,imageWidth);
    energyArray[1][m]=energyValue;
    m++;
    
    m=0;
    energyValue=getEnergy(rpixel2D3,E5E5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,E5S5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,E5W5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,S5E5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,S5S5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,S5W5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,W5E5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,W5S5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D3,W5W5,imageHeight,imageWidth);
    energyArray[2][m]=energyValue;
    m++;

    m=0;
    energyValue=getEnergy(rpixel2D4,E5E5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,E5S5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,E5W5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,S5E5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,S5S5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,S5W5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,W5E5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,W5S5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D4,W5W5,imageHeight,imageWidth);
    energyArray[3][m]=energyValue;
    m++;

    m=0;
    energyValue=getEnergy(rpixel2D5,E5E5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,E5S5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,E5W5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,S5E5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,S5S5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,S5W5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,W5E5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,W5S5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D5,W5W5,imageHeight,imageWidth);
    energyArray[4][m]=energyValue;
    m++;
    
    m=0;
    energyValue=getEnergy(rpixel2D6,E5E5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,E5S5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,E5W5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,S5E5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,S5S5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,S5W5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,W5E5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,W5S5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D6,W5W5,imageHeight,imageWidth);
    energyArray[5][m]=energyValue;
    m++;
    
    m=0;
    energyValue=getEnergy(rpixel2D7,E5E5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,E5S5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,E5W5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,S5E5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,S5S5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,S5W5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,W5E5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,W5S5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D7,W5W5,imageHeight,imageWidth);
    energyArray[6][m]=energyValue;
    m++;
    
    m=0;
    energyValue=getEnergy(rpixel2D8,E5E5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,E5S5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,E5W5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,S5E5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,S5S5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,S5W5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,W5E5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,W5S5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D8,W5W5,imageHeight,imageWidth);
    energyArray[7][m]=energyValue;
    m++;

    m=0;
    energyValue=getEnergy(rpixel2D9,E5E5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,E5S5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,E5W5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,S5E5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,S5S5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,S5W5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,W5E5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,W5S5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D9,W5W5,imageHeight,imageWidth);
    energyArray[8][m]=energyValue;
    m++;
    
    m=0;
    energyValue=getEnergy(rpixel2D10,E5E5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,E5S5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,E5W5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,S5E5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,S5S5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,S5W5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,W5E5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,W5S5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D10,W5W5,imageHeight,imageWidth);
    energyArray[9][m]=energyValue;
    m++;
    
    m=0;
    energyValue=getEnergy(rpixel2D11,E5E5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,E5S5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,E5W5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,S5E5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,S5S5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,S5W5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,W5E5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,W5S5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D11,W5W5,imageHeight,imageWidth);
    energyArray[10][m]=energyValue;
    m++;
    
    m=0;
    energyValue=getEnergy(rpixel2D12,E5E5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,E5S5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,E5W5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,S5E5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,S5S5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,S5W5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,W5E5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,W5S5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;
    energyValue=getEnergy(rpixel2D12,W5W5,imageHeight,imageWidth);
    energyArray[11][m]=energyValue;
    m++;

    int label[12];
    string labelName[12];
    // Apply the K-means clustering algorithm to cluster the images into 4 categories
    getKmeansclustering(energyArray,label,labelName,fileName);
    
    cout<<endl;
    cout<<"Image"<<"\t"<<"CLass Label"<<"\t"<<"Class Name"<<endl;
    for(int i=0;i<12;i++)
    {
        cout<<i<<"\t\t\t"<<label[i]<<"\t\t\t"<<labelName[i]<<endl;
    }
    cout<<endl;
    
    int labelCount[4]={0};
    for(int i=0;i<12;i++)
    {
        if (label[i]==1)
            labelCount[0]++;
        if (label[i]==2)
            labelCount[1]++;
        if (label[i]==3)
            labelCount[2]++;
        if (label[i]==4)
            labelCount[3]++;
    }
    
    cout<<endl;
    cout<<"Count for each label is: "<<endl;
    for(int i=1;i<=4;i++)
    {
        cout<<i<<"\t"<<labelCount[i-1]<<" "<<endl;
    }
    cout<<endl;
    
    // Write image data (filename specified by second argument) from image data matrix
    if (!(file=fopen(argv[16],"wb"))) {
        cout << "Cannot open file: " << argv[16] << endl;
        exit(1);
    }
    fwrite(combinedArray, sizeof(unsigned char), imageHeight*imageWidth, file);
    //fwrite(luminosityArray, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fclose(file);
    return 0;
}




