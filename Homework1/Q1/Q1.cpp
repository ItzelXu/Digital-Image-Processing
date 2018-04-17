#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

//Function to find the maximum of three numbers
unsigned char max(unsigned char arr1, unsigned char arr2, unsigned char arr3)
{
    unsigned char max1;
    max1=arr1;
    if (arr2>max1)
    {
        max1=arr2;
    }
    if (arr3>max1)
    {
        max1=arr3;
    }
    return(max1);
}

//Function to find the minimum of three numbers
unsigned char min(unsigned char arr1, unsigned char arr2, unsigned char arr3)
{
    int min1;
    min1=arr1;
    if (arr2<min1)
    {
        min1=arr2;
    }
    if (arr3<min1)
    {
        min1=arr3;
    }
    return(min1);
}

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int bytesPerPixel;
    int imageHeight=512;
    int imageWidth=512;
    
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
    unsigned char averageArray[imageHeight*imageWidth];
    unsigned char luminosityArray[imageHeight*imageWidth];
    unsigned char lightnessArray[imageHeight*imageWidth];
    unsigned char cyanArray[imageHeight*imageWidth];
    unsigned char magentaArray[imageHeight*imageWidth];
    unsigned char yellowArray[imageHeight*imageWidth];
    unsigned char rPixel[imageHeight*imageWidth];
    unsigned char gPixel[imageHeight*imageWidth];
    unsigned char bPixel[imageHeight*imageWidth];
    unsigned char minValue[imageHeight*imageWidth];
    unsigned char maxValue[imageHeight*imageWidth];
    unsigned char cmyArray[imageHeight*imageWidth];
    
    int i,j;
    
    //Conversion of the 1D input image into R,G,B channels
    for (i = 0, j = 0; i < imageHeight*imageWidth && j < imageHeight*imageWidth* 3; i++, j = j + 3)
    {
        rPixel[i] = Imagedata[j];
        gPixel[i]= Imagedata[j+1];
        bPixel[i]= Imagedata[j+2];
    }
    
    //Using the lightness method for conversion of color image to grayscale image- Lightness method formula= (max(R,G,B) + min(R,G,B))/2
    for(i=0;i<imageHeight*imageWidth;i++)
    {
        maxValue[i]=max(rPixel[i],gPixel[i],bPixel[i]);
        minValue[i]=min(rPixel[i],gPixel[i],bPixel[i]);
        lightnessArray[i]= ((maxValue[i]+minValue[i])/2);
    }
    
    //Using the average method for conversion of color image to grayscale image - Average method formula= (R+G+B)/3
    for(i=0;i<imageHeight*imageWidth;i++)
    {
        averageArray[i]= ((rPixel[i]+gPixel[i]+bPixel[i])/3);
    }
    
    
    //Using the luminosity method for conversion of color image to grayscale image- Luminosity method formula= 0.21R+0.72G+0.07B
    for(i=0;i<imageHeight*imageWidth;i++)
    {
        luminosityArray[i]= (unsigned char) (0.21*rPixel[i]+0.72*gPixel[i]+0.07*bPixel[i]);
    }
    
    //Conversion of RGB to CMY model- Cyan=255 - R, Magenta=255 - G, Yellow=255 - B
    for(i=0;i<imageHeight*imageWidth;i++)
    {
        cyanArray[i]=255-(rPixel[i]);
    }
    
    for(i=0;i<imageHeight*imageWidth;i++)
    {
        magentaArray[i]=255-(gPixel[i]);
    }
    
    for(i=0;i<imageHeight*imageWidth;i++)
    {
        yellowArray[i]=255-(bPixel[i]);
    }
    
    //Conversion of the C,M,Y channels to 1D output image
    for (i = 0, j = 0; i < imageHeight*imageWidth && j < imageHeight*imageWidth* 3; i++, j = j + 3)
    {
        cmyArray[j] = cyanArray[i];
        cmyArray[j+1]= magentaArray[i];
        cmyArray[j+2]= yellowArray[i];
    }
    
    
    // Write image data (filename specified by second argument) from image data matrix
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    
    //copies contents from file pointed by imagedata to the filename given in the input
    //fwrite(lightnessArray, sizeof(unsigned char), imageHeight*imageWidth, file);
    //fwrite(averageArray, sizeof(unsigned char), imageHeight*imageWidth, file);
    //fwrite(luminosityArray, sizeof(unsigned char), imageHeight*imageWidth, file);
    //fwrite(cyanArray, sizeof(unsigned char), imageHeight*imageWidth, file);
    //fwrite(magentaArray, sizeof(unsigned char), imageHeight*imageWidth, file);
    //fwrite(yellowArray, sizeof(unsigned char), imageHeight*imageWidth, file);
    fwrite(cmyArray, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fclose(file);
    
    return 0;
}
