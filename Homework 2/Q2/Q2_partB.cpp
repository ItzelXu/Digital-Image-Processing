#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace std;

void get2Dchannel(unsigned char* Imagedata,int imageHeight, int imageWidth,unsigned char**rpixel2D)
{
    int rowIndex,colIndex,traversalIndex=0;
    for (rowIndex = 0; rowIndex < imageHeight; rowIndex++)
    {
        for(colIndex = 0;colIndex < imageWidth; colIndex++)
        {
            rpixel2D[rowIndex][colIndex] = Imagedata[traversalIndex];
            traversalIndex++;
        }
    }
}

void combinechannel(unsigned char** rPixel2Dnew,unsigned char *combinedArray,int imageHeight, int imageWidth)
{
    int rowIndex,colIndex,traversalIndex=0;
    for (rowIndex = 0;rowIndex<imageHeight; rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth; colIndex++)
        {
            combinedArray[traversalIndex] = rPixel2Dnew[rowIndex][colIndex];
            traversalIndex++;
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
                if (rPixel2D[i][j]<127)
                {
                    rPixel2Dnew[i][j]=0;
                }
                else
                {
                    rPixel2Dnew[i][j]=255;
                }
                //Error Calculation wherein error is original value- thresholded value
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
                //Error Calculation wherein error is original value- thresholded value
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

void getJJNmatrix(unsigned char **rPixel2D,int imageHeight,int imageWidth,unsigned char **rPixel2Dnew)
{
    int i,j;
    float error;
    for(i=0;i<imageHeight;i++)
    {
        if (i%2==0)  // left to right traversal for even rows
        {
            for(j=0;j<imageWidth;j++)
            {
                if (rPixel2D[i][j]>127)
                {
                    rPixel2Dnew[i][j]=255;
                }
                else
                {
                    rPixel2Dnew[i][j]=0;
                }
                //Error Calculation wherein error is original value- thresholded value
                error=rPixel2D[i][j]-rPixel2Dnew[i][j];
                //Error Diffusion to future pixels
                if(j<imageWidth-1)
                {
                    rPixel2D[i][j+1]=rPixel2D[i][j+1]+(int)((error*(7.00/48.00)));
                }
                if(j<imageWidth-2)
                {
                    rPixel2D[i][j+2]=rPixel2D[i][j+2]+(int)((error*(5.00/48.00)));
                }
                if(i<imageHeight-1 && j>1)
                {
                    rPixel2D[i+1][j-2]=rPixel2D[i+1][j-2]+(int)((error*(3.00/48.00)));
                }
                if(i<imageHeight-1 && j>0)
                {
                    rPixel2D[i+1][j-1]=rPixel2D[i+1][j-1]+(int)((error*(5.00/48.00)));
                }
                if(i<imageHeight-1)
                {
                    rPixel2D[i+1][j]=rPixel2D[i+1][j]+(int)((error*(7.00/48.00)));
                }
                if(i<imageHeight-1 && j<imageWidth-1)
                {
                    rPixel2D[i+1][j+1]=rPixel2D[i+1][j+1]+(int)((error*(5.00/48.00)));
                }
                if(i<imageHeight-1 && j<imageWidth-2)
                {
                    rPixel2D[i+1][j+2]=rPixel2D[i+1][j+2]+(int)((error*(3.00/48.00)));
                }
                if(i<imageHeight-2 && j>1)
                {
                    rPixel2D[i+2][j-2]=rPixel2D[i+2][j-2]+(int)((error*(1.00/48.00)));
                }
                if(i<imageHeight-2 && j>0)
                {
                    rPixel2D[i+2][j-1]=rPixel2D[i+2][j-1]+(int)((error*(3.00/48.00)));
                }
                if(i<imageHeight-2)
                {
                    rPixel2D[i+2][j]=rPixel2D[i+2][j]+(int)((error*(5.00/48.00)));
                }
                if(i<imageHeight-2 && j<imageWidth-1)
                {
                    rPixel2D[i+2][j+1]=rPixel2D[i+2][j+1]+(int)((error*(3.00/48.00)));
                }
                if(i<imageHeight-2 && j<imageWidth-2)
                {
                    rPixel2D[i+2][j+2]=rPixel2D[i+2][j+2]+(int)((error*(1.00/48.00)));
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
                //Error Calculation wherein error is original value- thresholded value
                error=rPixel2D[i][j]-rPixel2Dnew[i][j];
                //Error Diffusion to future pixels
                if(j>0)
                {
                    rPixel2D[i][j-1]=rPixel2D[i][j-1]+(int)((error*(7.00/48.00)));
                }
                if(j>1)
                {
                    rPixel2D[i][j-2]=rPixel2D[i][j-2]+(int)((error*(5.00/48.00)));
                }
                if(i<imageHeight-1 && j>1)
                {
                    rPixel2D[i+1][j-2]=rPixel2D[i+1][j-2]+(int)((error*(3.00/48.00)));
                }
                if(i<imageHeight-1 && j>0)
                {
                    rPixel2D[i+1][j-1]=rPixel2D[i+1][j-1]+(int)((error*(5.00/48.00)));
                }
                if(i<imageHeight-1)
                {
                    rPixel2D[i+1][j]=rPixel2D[i+1][j]+(int)((error*(7.00/48.00)));
                }
                if(i<imageHeight-1 && j<imageWidth-1)
                {
                    rPixel2D[i+1][j+1]=rPixel2D[i+1][j+1]+(int)((error*(5.00/48.00)));
                }
                if(i<imageHeight-1 && j<imageWidth-2)
                {
                    rPixel2D[i+1][j+2]=rPixel2D[i+1][j+2]+(int)((error*(3.00/48.00)));
                }
                if(i<imageHeight-2 && j>1)
                {
                    rPixel2D[i+2][j-2]=rPixel2D[i+2][j-2]+(int)((error*(1.00/48.00)));
                }
                if(i<imageHeight-2 && j>0)
                {
                    rPixel2D[i+2][j-1]=rPixel2D[i+2][j-1]+(int)((error*(3.00/48.00)));
                }
                if(i<imageHeight-2 )
                {
                    rPixel2D[i+2][j]=rPixel2D[i+2][j]+(int)((error*(5.00/48.00)));
                }
                if(i<imageHeight-2 && j<imageWidth-1)
                {
                    rPixel2D[i+2][j+1]=rPixel2D[i+2][j+1]+(int)((error*(3.00/48.00)));
                }
                if(i<imageHeight-2 && j<imageWidth-2)
                {
                    rPixel2D[i+2][j+2]=rPixel2D[i+2][j+2]+(int)((error*(1.00/48.00)));
                }
            }
        }
    }
    
}

void getStuckimatrix(unsigned char **rPixel2D,int imageHeight,int imageWidth,unsigned char **rPixel2Dnew)
{
    int i,j;
    float error;
    for(i=0;i<imageHeight;i++)
    {
        if (i%2==0)  // left to right traversal for even rows
        {
            for(j=0;j<imageWidth;j++)
            {
                if (rPixel2D[i][j]>127)
                {
                    rPixel2Dnew[i][j]=255;
                }
                else
                {
                    rPixel2Dnew[i][j]=0;
                }
                //Error Calculation wherein error is original value- thresholded value
                error=rPixel2D[i][j]-rPixel2Dnew[i][j];
                //Error Diffusion to future pixels
                if(j<imageWidth-1)
                {
                    rPixel2D[i][j+1]=rPixel2D[i][j+1]+(int)((error*(8.00/42.00)));
                }
                if(j<imageWidth-2)
                {
                    rPixel2D[i][j+2]=rPixel2D[i][j+2]+(int)((error*(4.00/42.00)));
                }
                if(i<imageHeight-1 && j>1)
                {
                    rPixel2D[i+1][j-2]=rPixel2D[i+1][j-2]+(int)((error*(2.00/42.00)));
                }
                if(i<imageHeight-1 && j>0)
                {
                    rPixel2D[i+1][j-1]=rPixel2D[i+1][j-1]+(int)((error*(4.00/42.00)));
                }
                if(i<imageHeight-1)
                {
                    rPixel2D[i+1][j]=rPixel2D[i+1][j]+(int)((error*(8.00/42.00)));
                }
                if(i<imageHeight-1 && j<imageWidth-1)
                {
                    rPixel2D[i+1][j+1]=rPixel2D[i+1][j+1]+(int)((error*(4.00/42.00)));
                }
                if(i<imageHeight-1 && j<imageWidth-2)
                {
                    rPixel2D[i+1][j+2]=rPixel2D[i+1][j+2]+(int)((error*(2.00/42.00)));
                }
                if(i<imageHeight-2 && j>1)
                {
                    rPixel2D[i+2][j-2]=rPixel2D[i+2][j-2]+(int)((error*(1.00/42.00)));
                }
                if(i<imageHeight-2 && j>0)
                {
                    rPixel2D[i+2][j-1]=rPixel2D[i+2][j-1]+(int)((error*(2.00/42.00)));
                }
                if(i<imageHeight-2)
                {
                    rPixel2D[i+2][j]=rPixel2D[i+2][j]+(int)((error*(4.00/42.00)));
                }
                if(i<imageHeight-2 && j<imageWidth-1)
                {
                    rPixel2D[i+2][j+1]=rPixel2D[i+2][j+1]+(int)((error*(2.00/42.00)));
                }
                if(i<imageHeight-2 && j<imageWidth-2)
                {
                    rPixel2D[i+2][j+2]=rPixel2D[i+2][j+2]+(int)((error*(1.00/42.00)));
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
                //Error Calculation wherein error is original value- thresholded value
                error=rPixel2D[i][j]-rPixel2Dnew[i][j];
                //Error Diffusion to future pixels
                if(j>0)
                {
                    rPixel2D[i][j-1]=rPixel2D[i][j-1]+(int)((error*(8.00/42.00)));
                }
                if(j>1)
                {
                    rPixel2D[i][j-2]=rPixel2D[i][j-2]+(int)((error*(4.00/42.00)));
                }
                if(i<imageHeight-1 && j>1)
                {
                    rPixel2D[i+1][j-2]=rPixel2D[i+1][j-2]+(int)((error*(2.00/42.00)));
                }
                if(i<imageHeight-1 && j>0)
                {
                    rPixel2D[i+1][j-1]=rPixel2D[i+1][j-1]+(int)((error*(4.00/42.00)));
                }
                if(i<imageHeight-1)
                {
                    rPixel2D[i+1][j]=rPixel2D[i+1][j]+(int)((error*(8.00/42.00)));
                }
                if(i<imageHeight-1 && j<imageWidth-1)
                {
                    rPixel2D[i+1][j+1]=rPixel2D[i+1][j+1]+(int)((error*(4.00/42.00)));
                }
                if(i<imageHeight-1 && j<imageWidth-2)
                {
                    rPixel2D[i+1][j+2]=rPixel2D[i+1][j+2]+(int)((error*(2.00/42.00)));
                }
                if(i<imageHeight-2 && j>1)
                {
                    rPixel2D[i+2][j-2]=rPixel2D[i+2][j-2]+(int)((error*(1.00/42.00)));
                }
                if(i<imageHeight-2 && j>0)
                {
                    rPixel2D[i+2][j-1]=rPixel2D[i+2][j-1]+(int)((error*(2.00/42.00)));
                }
                if(i<imageHeight-2 )
                {
                    rPixel2D[i+2][j]=rPixel2D[i+2][j]+(int)((error*(4.00/42.00)));
                }
                if(i<imageHeight-2 && j<imageWidth-1)
                {
                    rPixel2D[i+2][j+1]=rPixel2D[i+2][j+1]+(int)((error*(2.00/42.00)));
                }
                if(i<imageHeight-2 && j<imageWidth-2)
                {
                    rPixel2D[i+2][j+2]=rPixel2D[i+2][j+2]+(int)((error*(1.00/42.00)));
                }
            }
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
    unsigned char combinedJJN[imageHeight*imageWidth*bytesPerPixel];
    unsigned char combinedStucki[imageHeight*imageWidth*bytesPerPixel];
    
    unsigned char** rpixel2Ddummy=new  unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** rpixel2D1=rpixel2Ddummy;
    
    unsigned char** rpixel2Ddummy4=new  unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy4[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy4[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** rpixel2D2=rpixel2Ddummy4;
    
    unsigned char** rpixel2Ddummy5=new  unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy5[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy5[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** rpixel2D3=rpixel2Ddummy5;
    
    unsigned char** rpixel2Ddummy1=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy1[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy1[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** rpixel2Dfloyd=rpixel2Ddummy1;
    
    unsigned char** rpixel2Ddummy2=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy2[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy2[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** rpixel2DJJN=rpixel2Ddummy2;
    
    unsigned char** rpixel2Ddummy6=new unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy6[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy6[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** rpixel2DStucki=rpixel2Ddummy6;
    
    //Converting the 1D input image into R channel (the inout image consists of a single channel
    get2Dchannel(Imagedata,imageHeight,imageWidth,rpixel2D1);
    get2Dchannel(Imagedata,imageHeight,imageWidth,rpixel2D2);
    get2Dchannel(Imagedata,imageHeight,imageWidth,rpixel2D3);
    
    //Applying floyd,JJN and Stucki filters
    getfloydmatrix(rpixel2D1,imageHeight,imageWidth,rpixel2Dfloyd);
    getJJNmatrix(rpixel2D2,imageHeight,imageWidth,rpixel2DJJN);
    getStuckimatrix(rpixel2D3,imageHeight,imageWidth,rpixel2DStucki);
    
    //Combine R channel back into a single output image
    combinechannel(rpixel2Dfloyd,combinedFloyd,imageHeight,imageWidth);
    combinechannel(rpixel2DJJN,combinedJJN,imageHeight,imageWidth);
    combinechannel(rpixel2DStucki,combinedStucki,imageHeight,imageWidth);
    
    // Write image data (filename specified by second argument) from image data matrix
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    //fwrite(combinedFloyd, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    //fwrite(combinedJJN, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fwrite(combinedStucki, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fclose(file);
    return 0;
}




