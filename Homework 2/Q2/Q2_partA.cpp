#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

bool checkfile(const string &filename,unsigned char *Imagedata, int imageDimension)
{
    bool filestatus = false;
    FILE *file;
    file=fopen(filename.c_str(),"rb");
    if(file!= nullptr && Imagedata!= nullptr)
    {
        filestatus=true;
        fread(Imagedata, sizeof(unsigned char),imageDimension, file);
        fclose(file);
    }
    else {
        filestatus = false;
    }
    
    return(filestatus);
}

void get2Dchannel(unsigned char* Imagedata,int imageHeight, int imageWidth,unsigned char **rpixel2D)
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

void combinechannel(unsigned char** rPixel2D,unsigned char *combinedArray,int imageHeight, int imageWidth)
{
    int rowIndex,colIndex,traversalIndex=0;
    for (rowIndex = 0;rowIndex<imageHeight; rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth; colIndex++)
        {
            combinedArray[traversalIndex] = rPixel2D[rowIndex][colIndex];
            traversalIndex++;
        }
    }
}

void getfixedthreshold(unsigned char** rPixel2D,unsigned char** rPixelfixedthreshold,int imageHeight ,int imageWidth)
{
    int rowIndex,colIndex;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            if ((int)rPixel2D[rowIndex][colIndex]<127) //Compare the input image values with threshold-- if input image value < threshold, output=0 else output=255
            {
                rPixelfixedthreshold[rowIndex][colIndex]=0;
            }
            else
            {
                rPixelfixedthreshold[rowIndex][colIndex]=255;
            }
        }
    }
}

void getrandomthreshold(unsigned char** rPixel2D,unsigned char **rPixelrandomthreshold,int imageHeight, int imageWidth)
{
    int v,rowIndex,colIndex;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            v=rand() % 256;  // Generate random numbers between 0 to 256
            if (v<=(int)rPixel2D[rowIndex][colIndex]) //Compare the randomly generated values with threshold-- if random value value < threshold, output=0 else output=255
            {
                rPixelrandomthreshold[rowIndex][colIndex]=255;
            }
            else
            {
                rPixelrandomthreshold[rowIndex][colIndex]=0;
            }
        }
    }
}
void getnormalized(unsigned char** rPixel2D,float **rPixel2Dfloat, int imageHeight, int imageWidth)
{
    int rowIndex,colIndex;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            rPixel2Dfloat[rowIndex][colIndex]=((float)rPixel2D[rowIndex][colIndex]/255.00); //Normalizing is dividing each intensity value by the maximum intensity value
        }
    }
}

void getfourlevels(unsigned char** rPixel2D,unsigned char** rPixelfourlevels,int imageHeight ,int imageWidth)
{
    //This is useing the bin method wherein each intensity value is divided in one bin to and the bin size is four
    int rowIndex,colIndex;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            if ((int)rPixel2D[rowIndex][colIndex]>0 && rPixel2D[rowIndex][colIndex]<=85)
            {
                if((int)rPixel2D[rowIndex][colIndex]<(int)(85/2))
                {
                    rPixelfourlevels[rowIndex][colIndex]=0;
                }
                else
                {
                    rPixelfourlevels[rowIndex][colIndex]=85;
                }
            }
            else if((int)rPixel2D[rowIndex][colIndex]>85 && rPixel2D[rowIndex][colIndex]<=170)
            {
                if((int)rPixel2D[rowIndex][colIndex]<(int)((170+85)/2))
                {
                    rPixelfourlevels[rowIndex][colIndex]=85;
                }
                else
                {
                    rPixelfourlevels[rowIndex][colIndex]=170;
                }
            }
            else if((int)rPixel2D[rowIndex][colIndex]>170 && rPixel2D[rowIndex][colIndex]<=255)
            {
                if((int)rPixel2D[rowIndex][colIndex]<(int)((170+255)/2))
                {
                    rPixelfourlevels[rowIndex][colIndex]=170;
                }
                else
                {
                    rPixelfourlevels[rowIndex][colIndex]=255;
                }
            }
        }
    }
}

void getIndexmatrix(float **I2,float **I4,int N)
{
    int i,j,p,q;
    for(i=0;i<N;i=i+(N/2))
    {
        for(j=0;j<N;j=j+(N/2))
        {
            for(p=i;p<i+(N/2);p++)
            {
                for(q=j;q<j+(N/2);q++)
                {
                        if(i==0 && i<N/2 && j==0 && j<N/2 )
                        {
                            I4[p][q]=4 *I2[p%(N/2)][q%(N/2)] + 1;
                        }
                        else if(i==0 && i<N/2 && j==N/2 && j<N )
                        {
                            I4[p][q]=4 *I2[p%(N/2)][q%(N/2)] + 2;
                        }
                        else if(i==N/2 && i<N && j==0 && j<N/2 )
                        {
                            I4[p][q]=4 *I2[p%(N/2)][q%(N/2)] + 3;
                        }
                        else if(i==N/2 && i<N && j==N/2 && j<N)
                        {
                            I4[p][q]=4 *I2[p%(N/2)][q%(N/2)];
                        }
                }
            }
        }
    }
}

void  getThreshold(float **I2,float **threshold2, int N)
{
    int rowIndex,colIndex;
    for(rowIndex=0;rowIndex<N;rowIndex++)
    {
        for(colIndex=0;colIndex<N;colIndex++)
        {
            {
                threshold2[rowIndex][colIndex] = ((I2[rowIndex][colIndex] + 0.50) / (N * N));
            }
        }
    }
}
void getDither(float** rPixel2Dfloat,float **threshold2,unsigned char **rPixeldither,int imageHeight, int imageWidth,int N)
{
    int i,j,p,q;
    for(i=0;i<imageHeight;i=i+N)
    {
        for(j=0;j<imageWidth;j=j+N)
        {
            for(p=i;p<i+N;p++)
            {
                for(q=j;q<j+N;q++)
                {
                        if (rPixel2Dfloat[p][q]>threshold2[p%N][q%N])  //Comparing the dithered matrices with threshold to get a binary image
                        {
                            rPixeldither[p][q]=255;
                        }
                        else
                        {
                            rPixeldither[p][q]=0;
                        }
                }
            }
        }
    }
}


int main(int argc, char *argv[]) {
    // Define file pointer and variables
    string filename;
    FILE *file;
    
    // Check for proper syntax
    if (argc !=6)
    {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [bytesPerPixel = 1] [Height=255] [Width=255]" << endl;
        return 0;
    }
    // Check if image is grayscale or color
    int bytesPerPixel = atoi(argv[3]);  //atoi converts string characters into integer
    int imageWidth = atoi(argv[4]);
    int imageHeight = atoi(argv[5]);
    int imageDimension = imageHeight * imageWidth * bytesPerPixel;
    
    unsigned char Imagedata[imageDimension];
    memset(Imagedata, 0, imageDimension * sizeof(unsigned char));
    // function to check file- if it exists or not
    bool filestatus;
    filestatus = checkfile(argv[1], Imagedata,imageDimension);
    if (filestatus)
    {
        unsigned char rgbfixedthreshold[imageDimension];
        unsigned char rgbrandomthreshold[imageDimension];
        unsigned char rgbfourlevels[imageDimension];
        unsigned char rgbdither2[imageDimension];
        unsigned char rgbdither4[imageDimension];
        unsigned char rgbdither8[imageDimension];
        memset(rgbfixedthreshold, 0, imageDimension * sizeof(unsigned char));
        memset(rgbrandomthreshold, 0, imageDimension * sizeof(unsigned char));
        memset(rgbdither2, 0, imageDimension * sizeof(unsigned char));
        memset(rgbdither4, 0, imageDimension * sizeof(unsigned char));
        memset(rgbdither8, 0, imageDimension * sizeof(unsigned char));
        int rowIndex,colIndex;
        //Array Declarations
        unsigned char** rpixel2Ddummy=new unsigned char* [imageHeight];
        for(int i=0;i<imageHeight;i++)
        {
            rpixel2Ddummy[i]=new unsigned char[imageWidth];
            memset(rpixel2Ddummy[i],0,imageWidth*sizeof(unsigned char));
        }
        unsigned char** rpixel2D=rpixel2Ddummy;
        
        unsigned char** rpixel2Ddummy1=new unsigned char* [imageHeight];
        for(int i=0;i<imageHeight;i++)
        {
            rpixel2Ddummy1[i]=new unsigned char[imageWidth];
            memset(rpixel2Ddummy1[i],0,imageWidth*sizeof(unsigned char));
        }
        unsigned char** rpixelfixedthreshold=rpixel2Ddummy1;
        
        unsigned char** rpixel2Ddummy2=new unsigned char* [imageHeight];
        for(int i=0;i<imageHeight;i++)
        {
            rpixel2Ddummy2[i]=new unsigned char[imageWidth];
            memset(rpixel2Ddummy2[i],0,imageWidth*sizeof(unsigned char));
        }
        unsigned char** rpixelrandomthreshold=rpixel2Ddummy2;
        
        unsigned char** rpixel2Ddummy15=new unsigned char* [imageHeight];
        for(int i=0;i<imageHeight;i++)
        {
            rpixel2Ddummy15[i]=new unsigned char[imageWidth];
            memset(rpixel2Ddummy15[i],0,imageWidth*sizeof(unsigned char));
        }
        unsigned char** rpixelfourlevels=rpixel2Ddummy15;
        
        unsigned char** rpixel2Ddummy3=new unsigned char* [imageHeight];
        for(int i=0;i<imageHeight;i++)
        {
            rpixel2Ddummy3[i]=new unsigned char[imageWidth];
            memset(rpixel2Ddummy3[i],0,imageWidth*sizeof(unsigned char));
        }
        unsigned char** rpixeldither2=rpixel2Ddummy3;
        
        unsigned char** rpixel2Ddummy7=new unsigned char* [imageHeight];
        for(int i=0;i<imageHeight;i++)
        {
            rpixel2Ddummy7[i]=new unsigned char[imageWidth];
            memset(rpixel2Ddummy7[i],0,imageWidth*sizeof(unsigned char));
        }
        unsigned char** rpixeldither4=rpixel2Ddummy7;
        
        unsigned char** rpixel2Ddummy9=new unsigned char* [imageHeight];
        for(int i=0;i<imageHeight;i++)
        {
            rpixel2Ddummy9[i]=new unsigned char[imageWidth];
            memset(rpixel2Ddummy9[i],0,imageWidth*sizeof(unsigned char));
        }
        unsigned char** rpixeldither8=rpixel2Ddummy9;
        
        float** rpixel2Ddummy4=new float* [imageHeight];
        for(int i=0;i<imageHeight;i++)
        {
            rpixel2Ddummy4[i]=new float[imageWidth];
            memset(rpixel2Ddummy4[i],0,imageWidth*sizeof(float));
        }
        float** rpixelfloat=rpixel2Ddummy4;
        
        float** indexdummy=new float* [2];
        for(int i=0;i<2;i++)
        {
            indexdummy[i]=new float[2];
            memset(indexdummy[i],0,2*sizeof(float));
        }
        float** I2=indexdummy;
        
        float** indexdummy1=new float* [4];
        for(int i=0;i<4;i++)
        {
            indexdummy1[i]=new float[4];
            memset(indexdummy1[i],0,4*sizeof(float));
        }
        float** I4=indexdummy1;
        
        float** indexdummy2=new float* [8];
        for(int i=0;i<8;i++)
        {
            indexdummy2[i]=new float[8];
            memset(indexdummy2[i],0,8*sizeof(float));
        }
        float** I8=indexdummy2;
        
        float** indexthreshold=new float* [2];
        for(int i=0;i<2;i++)
        {
            indexthreshold[i]=new float[2];
            memset(indexthreshold[i],0,2*sizeof(float));
        }
        float** threshold2=indexthreshold;
        
        float** indexthreshold1=new float* [4];
        for(int i=0;i<4;i++)
        {
            indexthreshold1[i]=new float[4];
            memset(indexthreshold1[i],0,4*sizeof(float));
        }
        float** threshold4=indexthreshold1;
        
        float** indexthreshold2=new float* [8];
        for(int i=0;i<8;i++)
        {
            indexthreshold2[i]=new float[8];
            memset(indexthreshold2[i],0,8*sizeof(float));
        }
        float** threshold8=indexthreshold2;
        
        int dummy[4]={1,2,3,0};
        int traversalIndex=0;
        for(rowIndex=0;rowIndex<2;rowIndex++)
        {
            for(colIndex=0;colIndex<2;colIndex++)
            {
                I2[rowIndex][colIndex]=dummy[traversalIndex];
                traversalIndex++;
            }
        }
        //Seperate R channel from the input image(The image consists of a single channel)
        get2Dchannel(Imagedata,imageHeight,imageWidth,rpixel2D);
        
        //Apply fixed thresholding algorithm
        getfixedthreshold(rpixel2D,rpixelfixedthreshold,imageHeight,imageWidth);
        
        //Apply random thresholding algorithm
        getrandomthreshold(rpixel2D,rpixelrandomthreshold,imageHeight,imageWidth);
        
        //Get normalized values between 0 and 1
        getnormalized(rpixel2D,rpixelfloat,imageHeight,imageWidth);
        
        //Get four output intensity levels
        getfourlevels(rpixel2D,rpixelfourlevels,imageHeight,imageWidth);
   
        //Obtain index matrix
        getIndexmatrix(I2,I4,4);
        getIndexmatrix(I4,I8,8);
        
        //Obtain threshold matrix from index matrix by formula- threshold matrix=(index value + 0.5)/(number of pixels in index matrix)^2
        getThreshold(I2,threshold2,2);
        getThreshold(I4,threshold4,4);
        getThreshold(I8,threshold8,8);
        
        //Dithering algorithm used on I2,I4,I8 matrices
        getDither(rpixelfloat,threshold2,rpixeldither2,imageHeight,imageWidth,2);
        getDither(rpixelfloat,threshold4,rpixeldither4,imageHeight,imageWidth,4);
        getDither(rpixelfloat,threshold8,rpixeldither8,imageHeight,imageWidth,8);
       
        //Combine R channel into a single output image
        combinechannel(rpixelfixedthreshold,rgbfixedthreshold,imageHeight,imageWidth);
        combinechannel(rpixelrandomthreshold,rgbrandomthreshold,imageHeight,imageWidth);
        combinechannel(rpixelfourlevels,rgbfourlevels,imageHeight,imageWidth);
        combinechannel(rpixeldither2,rgbdither2,imageHeight,imageWidth);
        combinechannel(rpixeldither4,rgbdither4,imageHeight,imageWidth);
        combinechannel(rpixeldither8,rgbdither8,imageHeight,imageWidth);
        
        //Write image data (filename specified by second argument) from image data matrix
        if (!(file=fopen(argv[2],"wb"))) {
            cout << "Cannot open file: " << argv[2] << endl;
            exit(1);
        }
        //copies contents from file pointed by imagedata to the filename given in the input
        //fwrite(rgbfixedthreshold, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
        //fwrite(rgbrandomthreshold, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
        //fwrite(rgbdither2, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
        //fwrite(rgbdither4, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
        //fwrite(rgbdither8, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
        fwrite(rgbfourlevels, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
        fclose(file);
    }
    else
    {
        cout << "Path is not correct. Check it" << endl;
    }
    
    return(0);
}

