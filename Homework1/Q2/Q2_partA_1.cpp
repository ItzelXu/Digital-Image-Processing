#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int bytesPerpixel = 3;
    int imageHeight = 300;
    int imageWidth = 400;
    string fileName;
    string fileName1;
    
    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [bytesPerpixel = 1] [Height=255] [Width=255]" << endl;
        return 0;
    }
    
    // Check if image is grayscale or color
    if (argc < 4){
        bytesPerpixel = 1; // default is grey image
    }
    else {
        bytesPerpixel = atoi(argv[3]);  //atoi converts string characters into integer
        // Check if size is specified
        if (argc >= 5){
            imageWidth= atoi(argv[4]);
            imageHeight= atoi(argv[5]);
            //Take input filenames for displaying histograms
            fileName=argv[6];
            fileName1=argv[7];
        }
    }
    
    // Allocate image data array
    unsigned char Imagedata[imageHeight*imageWidth*bytesPerpixel];
    
    // Read image (filename specified by first argument) into image data matrix where 'rb' indicate reading a binary file
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    // format of fread is fread ( void * ptr, size of each clock of memory, count to indicate memory required, FILE to read from);
    fread(Imagedata, sizeof(unsigned char), imageHeight*imageWidth*bytesPerpixel, file);
    fclose(file);
    
    ///////////////////////// PROCESSING CODE ///////////////////////
    unsigned char rpixel[imageHeight*imageWidth];
    unsigned char gpixel[imageHeight*imageWidth];
    unsigned char bpixel[imageHeight*imageWidth];
    unsigned char rpixelnew[imageHeight*imageWidth];
    unsigned char gpixelnew[imageHeight*imageWidth];
    unsigned char bpixelnew[imageHeight*imageWidth];
    unsigned char histogramArray[imageHeight*imageWidth*bytesPerpixel];
    unsigned int intensityR[256]={0};
    unsigned int intensityG[256]={0};
    unsigned int intensityB[256]={0};
    float probabilityR[256]={0};
    float probabilityB[256]={0};
    float probabilityG[256]={0};
    float cdfR[256]={0};
    float cdfG[256]={0};
    float cdfB[256]={0};
    int cdfRfloor[256];
    int cdfGfloor[256];
    int cdfBfloor[256];
    int i,j,k;
    
    //Conversion of 1D input image into seperate R,G,B channels
    for (i = 0, j = 0; i < imageHeight*imageWidth && j < imageHeight*imageWidth* 3; i++, j = j + 3)
    {
        rpixel[i] = Imagedata[j];
        gpixel[i]= Imagedata[j+1];
        bpixel[i]= Imagedata[j+2];
    }
    
    //Finding the count of each intensity value for R Channel
    for(k=0;k<=255;k++)
    {
        for(i=0;i<imageHeight*imageWidth;i++)
        {
            if (rpixel[i] == k)
            {
                intensityR[k] = intensityR[k]+1;
            }
        }
    }
    
    //Finding the count of each intensity value for G Channel
    for(k=0;k<=255;k++)
    {
        for(i=0;i<imageHeight*imageWidth;i++)
        {
            if (gpixel[i]==k)
            {
                intensityG[k]=intensityG[k]+1;
            }
        }
    }
    
    //Finding the count of each intensity value for B Channel
    for(k=0;k<=255;k++)
    {
        for(i=0;i<imageHeight*imageWidth;i++)
        {
            if (bpixel[i]==k)
            {
                intensityB[k]=intensityB[k]+1;
            }
        }
    }

    //Finding the probabilties of intensities for R,G,B channel- Probability = Number of intensities in one bin/ Total number of intensities in all the bins
    for(i=0;i<=255;i++)
    {
        probabilityR[i]=(float)(intensityR[i])/(float)(imageHeight*imageWidth);
        probabilityG[i]=(float)(intensityG[i])/(float)(imageHeight*imageWidth);
        probabilityB[i]=(float)(intensityB[i])/(float)(imageHeight*imageWidth);
    }
    
    //Finding the CDF of R channel(values between 0 and 1) and mapping it back to values between 0 to 255
    // cdf = probability at (i)th instance+probabilty at (i-1)th instance
    float flag1=0;
    for(i=0;i<=255;i++)
    {
        flag1=flag1+probabilityR[i];
        cdfR[i]=flag1;
        cdfRfloor[i]=floor(cdfR[i]*(float)255);
    }
    
    //Finding the CDF of G channel(values between 0 and 1) and the mapping it back to values between 0 to 255
    float flag2=0;
    for(i=0;i<=255;i++)
    {
        flag2=flag2+probabilityG[i];
        cdfG[i]=flag2;
        cdfGfloor[i]=floor(cdfG[i]*(float)255);
    }
    
    //Finding the CDF of B channel(values between 0 and 1) and the mapping it back to values between 0 to 255
    float flag3=0;
    for(i=0;i<=255;i++)
    {
        flag3=flag3+probabilityB[i];
        cdfB[i]=flag3;
        cdfBfloor[i]=floor(cdfB[i]*(float)255);
    }
    
    //Remmaping the cdf values to the original R,G,B pixels locations
    for(k=0;k<=255;k++)
    {
        for(i=0;i<imageHeight*imageWidth;i++)
        {
            if (rpixel[i]==(unsigned char)k)
            {
                rpixelnew[i]=(unsigned char)cdfRfloor[k];
            }
            if (gpixel[i]==k)
            {
                gpixelnew[i]=(unsigned char)cdfGfloor[k];
            }
            if (bpixel[i]==k)
            {
                bpixelnew[i]=(unsigned char)cdfBfloor[k];
            }
        }
    }
    
    //Combining the R,G,B channels to get a equalized 1D image using transfer function based method
     for (i = 0, j = 0; i < imageHeight*imageWidth && j < imageHeight*imageWidth* 3; i++, j = j + 3)
     {
     histogramArray[j] = rpixelnew[i];
     histogramArray[j+1]= gpixelnew[i];
     histogramArray[j+2]= bpixelnew[i];
     }
    
    //Writing contents into a file to find the histogram of the file
    FILE *histFile;
    histFile=fopen(fileName.c_str(),"w");
    
    if(histFile!=nullptr)
    {
        for(int k=0;k<256;k++)
        {
            fprintf(histFile,"%d\t%d\t%d\t%d\n",k,intensityR[k],intensityG[k],intensityB[k]);
        }
    }
    fclose(histFile);
    
    //Writing contents into a file to find the transfer function of the file
    FILE *transferfunction;
    transferfunction=fopen(fileName1.c_str(),"w");
    
    if(transferfunction!=nullptr)
    {
        for(int k=0;k<256;k++)
        {
            fprintf(transferfunction,"%d\t%d\t%d\t%d\n",k,cdfRfloor[k],cdfGfloor[k],cdfBfloor[k]);
        }
    }
    fclose(transferfunction);
    
    //Write image data (filename specified by second argument) from image data matrix
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }

    //copies contents from file pointed by imagedata to the filename given in the input given as the second argument
    fwrite(histogramArray, sizeof(unsigned char), imageHeight*imageWidth*bytesPerpixel, file);
    //fwrite(gpixel, sizeof(unsigned int), imageHeight*imageWidth, file);
    fclose(file);

    return 0;
}
