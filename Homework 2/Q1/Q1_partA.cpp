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

//Conversion of square image to disc image
void getsquaretodisc(unsigned char** rpixel2D,unsigned char**rpixelsqauretodisc,int imageHeight,int imageWidth)
{
    unsigned int p,q;
    float x,y,u,v;
    int rowIndex,colIndex;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            x=(rowIndex-((imageHeight/2)-1))/(float)(((imageHeight/2)-1)); //Normalizing values between -1 to 1
            y=(colIndex-((imageWidth/2)-1))/(float)(((imageWidth/2)-1));
            u=x*sqrt(1-((pow(y,2))/2));
            v=y*sqrt(1-((pow(x,2))/2));
            p=(u*((imageHeight/2)-1))+((imageHeight/2)-1); //Renomrlaizing values between 0 to imageHeight
            q=(v*((imageWidth/2)-1))+((imageWidth/2)-1);
            rpixelsqauretodisc[p][q]=rpixel2D[rowIndex][colIndex];
        }
    }
}

//Conversion of disc image to square image
void getdisctosquare(unsigned char** rpixel2D, unsigned char** rpixeldisctosquare,int imageHeight,int imageWidth)
{
    int rowIndex,colIndex;
    unsigned int l,m;
    float x,y,u,v,xnew,ynew;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            x=(rowIndex-((imageHeight/2)-1))/(float)(((imageHeight/2)-1)); //Normalizing values between -1 to 1
            y=(colIndex-((imageWidth/2)-1))/(float)(((imageWidth/2)-1));
            u=x*sqrt(1-((pow(y,2))/2));
            v=y*sqrt(1-((pow(x,2))/2));
            xnew=1.00/2.00*((sqrt((2+(pow(u,2))-(pow(v,2))+(2*(sqrt(2)*u)))))-(sqrt((2+(pow(u,2))-(pow(v,2))-(2*(sqrt(2)*u))))));
            ynew=1.00/2.00*((sqrt((2-(pow(u,2))+(pow(v,2))+(2*(sqrt(2)*v)))))-(sqrt((2-(pow(u,2))+(pow(v,2))-(2*(sqrt(2)*v))))));
            l=round(xnew*((imageHeight/2)-1))+((imageHeight/2)-1); //Renomrlaizing values between 0 to imageHeight
            m=round(ynew*((imageWidth/2)-1))+((imageWidth/2)-1);
            rpixeldisctosquare[l][m]=rpixel2D[rowIndex][colIndex];
        }
    }
}

//Combine 2D arrays to get a 1D array to be displayed as output
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
    int imageDimension=imageHeight*imageWidth*bytesPerPixel;
    unsigned char Imagedata[imageDimension];
    unsigned char combinedArraySquaretoDisc[imageDimension];
    unsigned char combinedArrayDisctoSquare[imageDimension];
    
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
    
    unsigned char** rpixel2Ddummy2=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy2[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy2[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** rpixelsquaretodisc=rpixel2Ddummy2;
    
    unsigned char** gpixel2Ddummy2=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        gpixel2Ddummy2[i]=new unsigned char[imageWidth];
        memset(gpixel2Ddummy2[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** gpixelsquaretodisc=gpixel2Ddummy2;
    
    unsigned char** bpixel2Ddummy2=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        bpixel2Ddummy2[i]=new unsigned char[imageWidth];
        memset(bpixel2Ddummy2[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** bpixelsquaretodisc=bpixel2Ddummy2;
    
    unsigned char** rpixel2Ddummy3=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy3[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy3[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** rpixeldisctosquare=rpixel2Ddummy3;
    
    unsigned char** gpixel2Ddummy3=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        gpixel2Ddummy3[i]=new unsigned char[imageWidth];
        memset(gpixel2Ddummy3[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** gpixeldisctosquare=gpixel2Ddummy3;
    
    unsigned char** bpixel2Ddummy3=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        bpixel2Ddummy3[i]=new unsigned char[imageWidth];
        memset(bpixel2Ddummy3[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** bpixeldisctosquare=bpixel2Ddummy3;
    
    //Seperating R,G,B channels from the inout image
    get2Dchannel(Imagedata,imageHeight,imageWidth,rpixel2D,0);
    get2Dchannel(Imagedata,imageHeight,imageWidth,gpixel2D,1);
    get2Dchannel(Imagedata,imageHeight,imageWidth,bpixel2D,2);
    
    //Converting square image to disc image
    getsquaretodisc(rpixel2D,rpixelsquaretodisc,imageHeight,imageWidth);
    getsquaretodisc(gpixel2D,gpixelsquaretodisc,imageHeight,imageWidth);
    getsquaretodisc(bpixel2D,bpixelsquaretodisc,imageHeight,imageWidth);
    
    //Converting disc image to square image
    getdisctosquare(rpixel2D,rpixeldisctosquare,imageHeight,imageWidth);
    getdisctosquare(gpixel2D,gpixeldisctosquare,imageHeight,imageWidth);
    getdisctosquare(bpixel2D,bpixeldisctosquare,imageHeight,imageWidth);
    
    //Combining the R,G,B channels of output square and disc images into a single image
    getcombinechannels(rpixelsquaretodisc,combinedArraySquaretoDisc,imageHeight,imageWidth,0);
    getcombinechannels(gpixelsquaretodisc,combinedArraySquaretoDisc,imageHeight,imageWidth,1);
    getcombinechannels(bpixelsquaretodisc,combinedArraySquaretoDisc,imageHeight,imageWidth,2);
    getcombinechannels(rpixeldisctosquare,combinedArrayDisctoSquare,imageHeight,imageWidth,0);
    getcombinechannels(gpixeldisctosquare,combinedArrayDisctoSquare,imageHeight,imageWidth,1);
    getcombinechannels(bpixeldisctosquare,combinedArrayDisctoSquare,imageHeight,imageWidth,2);
    
    // Write image data (filename specified by second argument) from image data matrix
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    fwrite(combinedArraySquaretoDisc, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    //fwrite(combinedArrayDisctoSquare, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fclose(file);
    return 0;
}


