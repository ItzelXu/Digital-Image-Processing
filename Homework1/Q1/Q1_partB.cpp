#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int bytesPerPixel;
    int imageHeight=512;
    int imageWidth=512;
    
    // Check if all arguments are passed
    if (argc < 3)
    {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [bytesPerPixel = 1] [Height=255] [Width=255]" << endl;
        return 0;
    }
    
    // Check if image is grayscale or color which is given as fourth argument on command line
    if (argc < 4)
    {
        bytesPerPixel = 1;     // default is grey image
    }
    else
    {
        bytesPerPixel = atoi(argv[3]);  //atoi converts string characters into integer
        // Check if size is specified
        if (argc >= 5){
            imageWidth= atoi(argv[4]);
            imageHeight= atoi(argv[5]);
        }
    }
    
    // Allocate 1D image data array
    unsigned char Imagedata[imageHeight*imageWidth*bytesPerPixel];
    
    // Read image (filename specified by first argument) into image data matrix where 'rb' indicate reading a binary file
    if (!(file=fopen(argv[1],"rb")))
    {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    
    // format of fread is fread ( void * ptr, size required for each block of memory, count to indicate amount memory required, FILE to read from);
    fread(Imagedata, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fclose(file);
    
    
    ///////////////////////// PROCESSING CODE ///////////////////////
    int newHeight=650;
    int newWidth=650;
    unsigned char rPixel[imageHeight][imageWidth];
    unsigned char gPixel[imageHeight][imageWidth];
    unsigned char bPixel[imageHeight][imageWidth];
    unsigned char resizedImageR[newHeight][newWidth];
    unsigned char resizedImageG[newHeight][newWidth];
    unsigned char resizedImageB[newHeight][newWidth];
    unsigned char finalArray[newHeight*newWidth*bytesPerPixel];
    int roundedMappedPixelHeight,roundedMappedPixelWidth;
    int i,j,k;
    
    // Code to seperate out R,G,B pixels from the 1D image array
    i=0;
    for(j=0;j<imageHeight;j++)
    {
        for(k=0;k<imageWidth;k++)
        {
            rPixel[j][k]=Imagedata[i];
            i=i+3;
        }
    }
    
    i=1;
    for(j=0;j<imageHeight;j++)
    {
        for(k=0;k<imageWidth;k++)
        {
            gPixel[j][k]=Imagedata[i];
            i=i+3;
        }
    }
    
    i=2;
    for(j=0;j<imageHeight;j++)
    {
        for(k=0;k<imageWidth;k++)
        {
            bPixel[j][k]=Imagedata[i];
            i=i+3;
        }
    }
    
    
    for(i=0;i<newHeight;i++)
    {
        float mappedPixelHeight = (512.0/650.0) * i;  // (520/650) is resizing ratio
        roundedMappedPixelHeight= (int) floor(mappedPixelHeight);   //to floor the float value to the nearest smaller integer value
        
        if (roundedMappedPixelHeight>=imageHeight-1)  // to check if the image does not exceed the size 512
        {
            roundedMappedPixelHeight=roundedMappedPixelHeight-1;
        }
        
        //Find the difference between the float value and floored integer value
        float delx=mappedPixelHeight-roundedMappedPixelHeight;
        
        for(j=0;j<newWidth;j++)
        {
            float mappedPixelWidth = ( 512.0/ 650.0) * j;
            roundedMappedPixelWidth=(int) floor(mappedPixelWidth);
            
            if (roundedMappedPixelHeight>=imageWidth-1)
            {
                roundedMappedPixelHeight=roundedMappedPixelHeight-1;
            }
            
            float dely=mappedPixelWidth-roundedMappedPixelWidth;
            
            //Calculating resized image intensity values for R,G,B channel seperately using bilinear interpolation
            resizedImageR[i][j]= (unsigned char)((1-delx)*(1-dely)*rPixel[roundedMappedPixelHeight][roundedMappedPixelWidth]+
                                                   (delx)*(1-dely)*rPixel[roundedMappedPixelHeight+1][roundedMappedPixelWidth]+
                                                   (1-delx)*(dely)*rPixel[roundedMappedPixelHeight][roundedMappedPixelWidth+1]+
                                                   (delx)*(dely)*rPixel[roundedMappedPixelHeight+1][roundedMappedPixelWidth+1]);
            
            resizedImageG[i][j]=(unsigned char)((1-delx)*(1-dely)*gPixel[roundedMappedPixelHeight][roundedMappedPixelWidth]+
                                                  (delx)*(1-dely)*gPixel[roundedMappedPixelHeight+1][roundedMappedPixelWidth]+
                                                  (1-delx)*(dely)*gPixel[roundedMappedPixelHeight][roundedMappedPixelWidth+1]+
                                                  (delx)*(dely)*gPixel[roundedMappedPixelHeight+1][roundedMappedPixelWidth+1]);
            
            resizedImageB[i][j]=(unsigned char)((1-delx)*(1-dely)*bPixel[roundedMappedPixelHeight][roundedMappedPixelWidth]+
                                                  (delx)*(1-dely)*bPixel[roundedMappedPixelHeight+1][roundedMappedPixelWidth]+
                                                  (1-delx)*(dely)*bPixel[roundedMappedPixelHeight][roundedMappedPixelWidth+1]+
                                                  (delx)*(dely)*bPixel[roundedMappedPixelHeight+1][roundedMappedPixelWidth+1]);
        }
    }
    
    //combining the R,G,B seperate channels into a single 1D array to be displayed as output image
    int count=0;
    for(i=0;i<newHeight;i++)
    {
        for(j=0;j<newWidth;j++)
        {
            finalArray[count]=resizedImageR[i][j];
            finalArray[count+1]=resizedImageG[i][j];
            finalArray[count+2]=resizedImageB[i][j];
            count=count+3;
        }
    }
    
    // Write image data (filename specified by second argument) from resultant data matrix obtained after processing
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    
    //copies contents from file pointed by final array to the filename given in the input with size and amount of memory specified
    fwrite(finalArray, sizeof(unsigned char), newHeight*newWidth*bytesPerPixel, file);
    fclose(file);
    return 0;
}

