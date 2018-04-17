#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <cmath>

using namespace std;

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    FILE *newfile1;
    FILE *newfile2;
    int bytesPerPixel=3;
    int imageHeight=750;
    int imageWidth=1000;
    int girlImageHeight=256;
    int girlImageWidth=256;
    
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
            girlImageHeight= atoi(argv[8]);
            girlImageWidth= atoi(argv[9]);
        }
    }
    
    // Allocate image data array
    unsigned char Imagedata[imageHeight*imageWidth*bytesPerPixel];
    unsigned char ImagedataFilm[imageHeight*imageWidth*bytesPerPixel];
    unsigned char ImagedataGirl[girlImageHeight*girlImageWidth*bytesPerPixel];
    
    // Read image (filename specified by first argument) into image data matrix where 'rb' indicate reading a binary file
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    if (!(newfile1=fopen(argv[6],"rb"))) {
        cout << "Cannot open file: " << argv[6] <<endl;
        exit(1);
    }
    if (!(newfile2=fopen(argv[7],"rb"))) {
        cout << "Cannot open file: " << argv[7] <<endl;
        exit(1);
    }
    
    // format of fread is fread ( void * ptr, size of each clock of memory, count to indicate memory required, FILE to read from);
    fread(Imagedata, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fread(ImagedataFilm, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, newfile1);
    fread(ImagedataGirl, sizeof(unsigned char), girlImageHeight*girlImageWidth*bytesPerPixel, newfile2);
    fclose(file);
    fclose(newfile1);
    fclose(newfile2);
    
    ///////////////////////// PROCESSING CODE ///////////////////////
    
    vector<vector<unsigned char> > rPixel = vector<vector<unsigned char>>(imageHeight);
    vector<vector<unsigned char> > gPixel = vector<vector<unsigned char>>(imageHeight);
    vector<vector<unsigned char> > bPixel = vector<vector<unsigned char>>(imageHeight);
    vector<vector<unsigned char> > rPixelFilm = vector<vector<unsigned char>>(imageHeight);
    vector<vector<unsigned char> > gPixelFilm = vector<vector<unsigned char>>(imageHeight);
    vector<vector<unsigned char> > bPixelFilm = vector<vector<unsigned char>>(imageHeight);
    vector<vector<unsigned char> > rNewArray= vector<vector<unsigned char>>(imageHeight);
    vector<vector<unsigned char> > gNewArray= vector<vector<unsigned char>>(imageHeight);
    vector<vector<unsigned char> > bNewArray= vector<vector<unsigned char>>(imageHeight);
    vector<vector<unsigned char> > rPixelgirl = vector<vector<unsigned char>>(girlImageHeight);
    vector<vector<unsigned char> > gPixelgirl = vector<vector<unsigned char>>(girlImageHeight);
    vector<vector<unsigned char> > bPixelgirl = vector<vector<unsigned char>>(girlImageHeight);
    vector<vector<unsigned char> > rNewArraygirl=vector<vector<unsigned char>>(girlImageHeight);
    vector<vector<unsigned char> > gNewArraygirl=vector<vector<unsigned char>>(girlImageHeight);
    vector<vector<unsigned char> > bNewArraygirl=vector<vector<unsigned char>>(girlImageHeight);
    vector<unsigned int>intensityR=vector<unsigned int>(256);
    vector<unsigned int>intensityG=vector<unsigned int>(256);
    vector<unsigned int>intensityB=vector<unsigned int>(256);
    vector<unsigned int>intensityRfilm=vector<unsigned int>(256);
    vector<unsigned int>intensityGfilm=vector<unsigned int>(256);
    vector<unsigned int>intensityBfilm=vector<unsigned int>(256);
    vector<float>probabilityR=vector<float>(256);
    vector<float>probabilityG=vector<float>(256);
    vector<float>probabilityB=vector<float>(256);
    vector<float>probabilityRfilm=vector<float>(256);
    vector<float>probabilityGfilm=vector<float>(256);
    vector<float>probabilityBfilm=vector<float>(256);
    vector<float>cdfR=vector<float>(256);
    vector<float>cdfG=vector<float>(256);
    vector<float>cdfB=vector<float>(256);
    vector<float>cdfRfilm=vector<float>(256);
    vector<float>cdfGfilm=vector<float>(256);
    vector<float>cdfBfilm=vector<float>(256);
    vector<unsigned int>cdfRnew=vector<unsigned int>(256);
    vector<unsigned int>cdfGnew=vector<unsigned int>(256);
    vector<unsigned int>cdfBnew=vector<unsigned int>(256);
    vector<float>distR=vector<float>(256);
    vector<float>distG=vector<float>(256);
    vector<float>distB=vector<float>(256);
    unsigned char rgbArray[imageHeight*imageWidth*bytesPerPixel];
    unsigned char rgbArraygirl[girlImageHeight*girlImageWidth*bytesPerPixel];
    
    for (int i = 0; i < imageHeight; i++)
    {
        rPixel.at(i) = vector<unsigned char>(imageWidth);
        gPixel.at(i) = vector<unsigned char>(imageWidth);
        bPixel.at(i) = vector<unsigned char>(imageWidth);
        rPixelFilm.at(i)=vector<unsigned char>(imageWidth);
        gPixelFilm.at(i)=vector<unsigned char>(imageWidth);
        bPixelFilm.at(i)=vector<unsigned char>(imageWidth);
        rNewArray.at(i) = vector<unsigned char>(imageWidth);
        gNewArray.at(i) = vector<unsigned char>(imageWidth);
        bNewArray.at(i) = vector<unsigned char>(imageWidth);
    }
    for (int i = 0; i < 256; i++)
    {
        rPixelgirl.at(i)=vector<unsigned char>(girlImageHeight);
        gPixelgirl.at(i)=vector<unsigned char>(girlImageHeight);
        bPixelgirl.at(i)=vector<unsigned char>(girlImageHeight);
        rNewArraygirl.at(i) = vector<unsigned char>(girlImageHeight);
        gNewArraygirl.at(i) = vector<unsigned char>(girlImageHeight);
        bNewArraygirl.at(i) = vector<unsigned char>(girlImageHeight);
    }
    
    //Seperating the R,G,B channels for the film effect image and the original image
    unsigned int i,j,k;
    i=0;
    for(j=0;j<imageHeight;j++)
    {
        for(k=0;k<imageWidth;k++)
        {
            rPixel[j][k]=Imagedata[i];
            rPixelFilm[j][k]=ImagedataFilm[i];
            i=i+3;
        }
    }
    
    i=1;
    for(j=0;j<imageHeight;j++)
    {
        for(k=0;k<imageWidth;k++)
        {
            gPixel[j][k]=Imagedata[i];
            gPixelFilm[j][k]=ImagedataFilm[i];
            i=i+3;
        }
    }
    
    i=2;
    for(j=0;j<imageHeight;j++)
    {
        for(k=0;k<imageWidth;k++)
        {
            bPixel[j][k]=Imagedata[i];
            bPixelFilm[j][k]=ImagedataFilm[i];
            i=i+3;
        }
    }
    
    //Seperating the R,G,B channels for the girl image
    i=0;
    for(j=0;j<girlImageHeight;j++)
    {
        for(k=0;k<girlImageWidth;k++)
        {
            rPixelgirl[j][k]=ImagedataGirl[i];
            i=i+3;
        }
    }
    
    i=1;
    for(j=0;j<girlImageHeight;j++)
    {
        for(k=0;k<girlImageWidth;k++)
        {
            gPixelgirl[j][k]=ImagedataGirl[i];
            i=i+3;
        }
    }
    
    i=2;
    for(j=0;j<girlImageHeight;j++)
    {
        for(k=0;k<girlImageWidth;k++)
        {
            bPixelgirl[j][k]=ImagedataGirl[i];
            i=i+3;
        }
    }
    
    //Mirroring of film image
    for(i=0;i<imageHeight;i++)
    {
        for(j=0;j<imageWidth;j++)
        {
            rNewArray[i][j]=rPixel[i][imageWidth-1-j];
            gNewArray[i][j]=gPixel[i][imageWidth-1-j];
            bNewArray[i][j]=bPixel[i][imageWidth-1-j];
        }
    }
    // Colour inversion of film image i.e. converting the R,G,B image to C,M,Y image
    for(i=0;i<imageHeight;i++)
    {
        for (j = 0; j < imageWidth; j++)
        {
            rNewArray[i][j]=255-rNewArray[i][j];
            gNewArray[i][j]=255-gNewArray[i][j];
            bNewArray[i][j]=255-bNewArray[i][j];
        }
    }
    // Mirroring of girl image
    for(i=0;i<girlImageHeight;i++)
    {
        for(j=0;j<girlImageWidth;j++)
        {
            rNewArraygirl[i][j]=rPixelgirl[i][girlImageWidth-1-j];
            gNewArraygirl[i][j]=gPixelgirl[i][girlImageWidth-1-j];
            bNewArraygirl[i][j]=bPixelgirl[i][girlImageWidth-1-j];
        }
    }
    // Colour inversion of girl image i.e. converting the R,G,B image to C,M,Y image
    for(i=0;i<girlImageHeight;i++)
    {
        for (j = 0; j < girlImageWidth; j++)
        {
            rNewArraygirl[i][j]=255-rNewArraygirl[i][j];
            gNewArraygirl[i][j]=255-gNewArraygirl[i][j];
            bNewArraygirl[i][j]=255-bNewArraygirl[i][j];
        }
    }
    
    //Calculating count of intensities for each intensity value for the film effect image and the original image
    for(k=0;k<=255;k++)
    {
        for(i=0;i<imageHeight;i++)
        {
            for(j=0;j<imageWidth;j++)
            {
                if (rNewArray[i][j] == k)
                {
                    intensityR[k] = intensityR[k]+1;
                }
                if (rPixelFilm[i][j] == k)
                {
                    intensityRfilm[k] = intensityRfilm[k]+1;
                }
                if (gNewArray[i][j]==k)
                {
                    intensityG[k]=intensityG[k]+1;
                }
                if (gPixelFilm[i][j] == k)
                {
                    intensityGfilm[k] = intensityGfilm[k]+1;
                }
                if (bNewArray[i][j]==k)
                {
                    intensityB[k]=intensityB[k]+1;
                }
                if (bPixelFilm[i][j] == k)
                {
                    intensityBfilm[k] = intensityBfilm[k]+1;
                }
            }
        }
    }
    
    //Finding the probabilties of intensities for R,G,B channel for the filme effect image and oriignal image- Probability = Number of intensities in one bin/ Total number of intensities in all the bins
    for(i=0;i<=255;i++)
    {
        probabilityR[i]=(float)(intensityR[i])/(float)(imageHeight*imageWidth);
        probabilityRfilm[i]=(float)(intensityRfilm[i])/(float)(imageHeight*imageWidth);
        probabilityG[i]=(float)(intensityG[i])/(float)(imageHeight*imageWidth);
        probabilityGfilm[i]=(float)(intensityGfilm[i])/(float)(imageHeight*imageWidth);
        probabilityB[i]=(float)(intensityB[i])/(float)(imageHeight*imageWidth);
        probabilityBfilm[i]=(float)(intensityBfilm[i])/(float)(imageHeight*imageWidth);
    }
    
    //Calculating cdf values for each intensity value of the film effect image and original image
    // cdf = probability at (i)th instance+probabilty at (i-1)th instance
    float flag1=0,flag2=0,flag3=0,flag4=0,flag5=0,flag6=0;
    for(i=0;i<=255;i++)
    {
        flag1=flag1+probabilityR[i];
        cdfR[i]=flag1;
        flag2=flag2+probabilityG[i];
        cdfG[i]=flag2;
        flag3=flag3+probabilityB[i];
        cdfB[i]=flag3;
        flag4=flag4+probabilityRfilm[i];
        cdfRfilm[i]=flag4;
        flag5=flag5+probabilityGfilm[i];
        cdfGfilm[i]=flag5;
        flag6=flag6+probabilityBfilm[i];
        cdfBfilm[i]=flag6;
    }
    
    //Calculating mimimum distances for film effect histogram values from original histogram values for R channel
    float minR=0,minIntensityR=0,minG=0,minIntensityG=0,minB=0,minIntensityB=0;
    for(i=0;i<=255;i++)
    {
        for(j=0;j<=255;j++)
        {
            distR[j]=abs(cdfR[i]-cdfRfilm[j]);
        }
        minR=1;
        for(k=0;k<=255;k++)
        {
            if(distR[k]<minR) //Finding the minimum distance and the corresponding index with the minimum distance so as to transform the histogram of the original image with respect to the film effect image
            {
                minR=distR[k];
                minIntensityR=k;
            }
        }
        cdfRnew[i]=minIntensityR;
    }
    //Calculating mimimum distances for film effect histogram values from original histogram values for G channel
    for(i=0;i<=255;i++)
    {
        for(j=0;j<=255;j++)
        {
            distG[j]=abs(cdfG[i]-cdfGfilm[j]);
        }
        minG=1;
        for(k=0;k<=255;k++)
        {
            if(distG[k]<minG)
            {
                minG=distG[k];
                minIntensityG=k;
            }
        }
        cdfGnew[i]=minIntensityG;
    }
    //Calculating mimimum distances for film effect histogram values from original histogram values for B channel
    for(i=0;i<=255;i++)
    {
        for(j=0;j<=255;j++)
        {
            distB[j]=abs(cdfB[i]-cdfBfilm[j]);
        }
        minB=1;
        for(k=0;k<=255;k++)
        {
            if(distB[k]<minB)
            {
                minB=distB[k];
                minIntensityB=k;
            }
        }
        cdfBnew[i]=minIntensityB;
    }
    
    //Assigning the changed intensity values to the mirrored and inverted R,G,B channels of the image to se  if we get the same film effect image as given in the question
    //This step is done to test the algorithm on the already given image
    unsigned char tempR=0, tempG=0, tempB=0;
    for(i=0; i < imageHeight; i++)
    {
        for(j=0; j < imageWidth; j++)
        {
            tempR = rNewArray[i][j];
            tempG = gNewArray[i][j];
            tempB = bNewArray[i][j];
            rNewArray[i][j] = cdfRnew[tempR];
            gNewArray[i][j] = cdfGnew[tempG];
            bNewArray[i][j] = cdfBnew[tempB];
        }
    }
   
    //Running the designed algorithm for the girl image to egt the film effect on the girl image
    tempR=0;tempG=0;tempB=0;
    for(i=0; i < girlImageHeight; i++)
    {
        for(j=0; j < girlImageWidth; j++)
        {
            tempR = rNewArraygirl[i][j];
            tempG = gNewArraygirl[i][j];
            tempB = bNewArraygirl[i][j];
            rNewArraygirl[i][j] = cdfRnew[tempR];
            gNewArraygirl[i][j] = cdfGnew[tempG];
            bNewArraygirl[i][j] = cdfBnew[tempB];
        }
    }

    //Combining the changes R,G,B channels to get the 1D array to display for the film image
    unsigned long int count=0;
    for(i=0;i<imageHeight;i++)
    {
        for(j=0;j<imageWidth;j++)
        {
            rgbArray[count]=rNewArray[i][j];
            rgbArray[count+1]=gNewArray[i][j];
            rgbArray[count+2]=bNewArray[i][j];
            count=count+3;
        }
    }
    //Combining the changes R,G,B channels to get the 1D array to display the film effect on the girl image
    count=0;
    for(i=0;i<girlImageHeight;i++)
    {
        for(j=0;j<girlImageWidth;j++)
        {
            rgbArraygirl[count]=rNewArraygirl[i][j];
            rgbArraygirl[count+1]=gNewArraygirl[i][j];
            rgbArraygirl[count+2]=bNewArraygirl[i][j];
            count=count+3;
        }
    }
    
    
    // Write image data (filename specified by second argument and tenth argument) from image data matrix
    
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    if (!(newfile1=fopen(argv[10],"wb"))) {
        cout << "Cannot open file: " << argv[10] << endl;
        exit(1);
    }
    fwrite(rgbArray, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fwrite(rgbArraygirl, sizeof(unsigned char), girlImageHeight*girlImageWidth*bytesPerPixel, newfile1);
    fclose(file);
    fclose(newfile1);
    return 0;
}


