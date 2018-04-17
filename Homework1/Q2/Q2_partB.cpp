#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

using namespace std;

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
    int colquant=4;  // To select colquant=4 for 64 bit represntation and colquant=8 for 512 bit representation
    vector <unsigned char> rPixel = vector <unsigned char>(imageHeight*imageWidth);
    vector <unsigned char> gPixel = vector <unsigned char>(imageHeight*imageWidth);
    vector <unsigned char> bPixel = vector <unsigned char>(imageHeight*imageWidth);
    vector <unsigned char> rPixelNew = vector <unsigned char>(imageHeight*imageWidth);
    vector <unsigned char> gPixelNew = vector <unsigned char>(imageHeight*imageWidth);
    vector <unsigned char> bPixelNew = vector <unsigned char>(imageHeight*imageWidth);
    vector<vector<unsigned char> > rPixel2D = vector<vector<unsigned char>>(imageHeight);
    vector<vector<unsigned char> > gPixel2D = vector<vector<unsigned char>>(imageHeight);
    vector<vector<unsigned char> > bPixel2D = vector<vector<unsigned char>>(imageHeight);
    vector<vector<unsigned char> > rPixel2DNew = vector<vector<unsigned char>>(imageHeight);
    vector<vector<unsigned char> > gPixel2DNew = vector<vector<unsigned char>>(imageHeight);
    vector<vector<unsigned char> > bPixel2DNew = vector<vector<unsigned char>>(imageHeight);
    vector <unsigned int> intensityR = vector <unsigned int>(256);
    vector <unsigned int> intensityG  = vector <unsigned int>(256);
    vector <unsigned int> intensityB  = vector <unsigned int>(256);
    vector <unsigned char> intensityRnew = vector <unsigned char>(imageHeight*imageWidth);
    vector <unsigned char> intensityGnew = vector <unsigned char>(imageHeight*imageWidth);
    vector <unsigned char> intensityBnew = vector <unsigned char>(imageHeight*imageWidth);
    vector <unsigned int> pR= vector <unsigned int>(imageHeight*imageWidth);
    vector <unsigned int> pG= vector <unsigned int>(imageHeight*imageWidth);
    vector <unsigned int> pB= vector <unsigned int>(imageHeight*imageWidth);
    vector<float> intensityRfinal=vector<float>(colquant);
    vector<float> intensityGfinal=vector<float>(colquant);
    vector<float> intensityBfinal=vector<float>(colquant);
    unsigned char histogramArray[imageHeight*imageWidth*bytesPerPixel];
    unsigned char R64bit[imageHeight][imageWidth];
    unsigned char G64bit[imageHeight][imageWidth];
    unsigned char B64bit[imageHeight][imageWidth];
    unsigned char combinedArray[imageHeight*imageWidth*bytesPerPixel];
    int i,j,k;
    int N=5;  //Declaration of value of window size
    
    for (int i = 0; i < imageHeight; i++)
    {
        rPixel2D.at(i) = vector<unsigned char>(imageWidth);
        gPixel2D.at(i) = vector<unsigned char>(imageWidth);
        bPixel2D.at(i) = vector<unsigned char>(imageWidth);
        rPixel2DNew.at(i) = vector<unsigned char>(imageWidth);
        gPixel2DNew.at(i) = vector<unsigned char>(imageWidth);
        bPixel2DNew.at(i) = vector<unsigned char>(imageWidth);
    }
    
    //Converting the 1D input image into R,G,B channels
    for (i = 0, j = 0; i < imageHeight*imageWidth && j < imageHeight*imageWidth* 3; i++, j = j + 3)
    {
        rPixel[i] = Imagedata[j];
        gPixel[i]= Imagedata[j+1];
        bPixel[i]= Imagedata[j+2];
    }
    
    //Finding the count of intensites for the R,G,B channel and storing the locations of the intensities for R,G,B channels respectively
    int m=0,p=0,q=0,r=0,s=0;
    j=0;
    for(k=0;k<=255;k++)
    {
        for(i=0;i<imageHeight*imageWidth;i++)
        {
            if (rPixel[i] == k)
            {
                pR[m]=i;
                intensityR[k] = intensityR[k]+1;
                intensityRnew[j]=k;
                j++;
                m++;
            }
            if (gPixel[i]==k)
            {
                pG[q]=i;
                intensityG[k]=intensityG[k]+1;
                intensityGnew[p]=k;
                p++;
                q++;
            }
            if (bPixel[i]==k)
            {
                pB[s]=i;
                intensityB[k]=intensityB[k]+1;
                intensityBnew[r]=k;
                r++;
                s++;
            }
        }
    }
    
    // Define the bin size and find the intensity to be assigned for every pixel in a bin by finding the mean intensity in that bin
    //Bin size is defined to be the total number of pixels/ the number of bins for each channel e.g for a 64 bit represntation each channel is divided into 4 bins
    float binSize=(float)(imageHeight*imageWidth)/(float)colquant;
    int l=0,count=0;
    float sum1=0,sum2=0,sum3=0;
    j=0;
    
    //Putting pixels into each bin by traversing till the bin size
    for(i=0;i<=imageHeight*imageWidth;i++)
    {
        if (i/binSize==0)
        {
            sum1=sum1+intensityRnew[i];
            sum2=sum2+intensityGnew[i];
            sum3=sum3+intensityBnew[i];
            l++;
        }
        else if(i/binSize!=l)
        {
            sum1=sum1+intensityRnew[i];
            sum2=sum2+intensityGnew[i];
            sum3=sum3+intensityBnew[i];
        }
        else
        {
            intensityRfinal[j]=(float)sum1/binSize;
            intensityGfinal[j]=(float)sum2/binSize;
            intensityBfinal[j]=(float)sum3/binSize;
            count++;
            j++;
            sum1=0;
            sum2=0;
            sum3=0;
            sum1=sum1+intensityRnew[i];
            sum2=sum2+intensityGnew[i];
            sum3=sum3+intensityBnew[i];
            l++;
        }
    }
    
    //Remapping the changed intensities at original positions in the R,G,B arrays
    j=0;l=0;
    for(i=0;i<imageWidth*imageHeight;i++)
    {
        if (i/binSize==0)
        {
            rPixelNew[pR[i]]=(unsigned char)intensityRfinal[j];
            gPixelNew[pG[i]]=(unsigned char)intensityGfinal[j];
            bPixelNew[pB[i]]=(unsigned char)intensityBfinal[j];
            l++;
        }
        else if(i/binSize!=l)
        {
            rPixelNew[pR[i]]=(unsigned char)intensityRfinal[j];
            gPixelNew[pG[i]]=(unsigned char)intensityGfinal[j];
            bPixelNew[pB[i]]=(unsigned char)intensityBfinal[j];
        }
        else
        {
            j++;
            rPixelNew[pR[i]]=(unsigned char)intensityRfinal[j];
            gPixelNew[pG[i]]=(unsigned char)intensityGfinal[j];
            bPixelNew[pB[i]]=(unsigned char)intensityBfinal[j];
            l++;
        }
    }
    
    //Converting 1D arrays for R,G,B channels into 2D arrays
    k=0;
    for (i=0;i<imageHeight;i++)
    {
        for(j=0;j<imageWidth;j++)
        {
            R64bit[i][j]=rPixelNew[k];
            G64bit[i][j]=gPixelNew[k];
            B64bit[i][j]=bPixelNew[k];
            k++;
        }
    }
    k=0;
    for (i=0;i<imageHeight;i++)
    {
        for(j=0;j<imageWidth;j++)
        {
            rPixel2D[i][j]=rPixelNew[k];
            gPixel2D[i][j]=gPixelNew[k];
            bPixel2D[i][j]=bPixelNew[k];
            k++;
        }
    }
    
    //-------------------------------Step 2---------------------------------------------------
    //Finding the most frequent color in the NxN neighborhood to showcase the oil painting effect
    int n,x;
    int max1=0,max2=0,max3=0;
    int sorted1[N*N];int sorted2[N*N];int sorted3[N*N];
    for(i=0;i<imageHeight;i++)
    {
        for(j=0;j<imageWidth;j++)
        {
            for(x=0;x<N*N;x++)
            {
                sorted1[x]={};
                sorted2[x]={};
                sorted3[x]={};
            }
            unsigned int intensityR[256]={};
            unsigned int intensityG[256]={};
            unsigned int intensityB[256]={};
            k=0;
            int max_value1=0,max_value2=0,max_value3=0;
            max1=0;max2=0;max3=0;
            for(p=i-(N-1)/2;p<=i+(N-1)/2;p++) //Traversing through the image by a NxN mask
            {
                for(q=j-(N-1)/2;q<=j+(N-1)/2;q++)
                {
                    int rowIndex=p;
                    int colIndex=q;
                    if(rowIndex<0)    //Checking for the boundary conditions if the mask index exceeds the image size
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
                    sorted1[k]=rPixel2D[rowIndex][colIndex];
                    sorted2[k]=gPixel2D[rowIndex][colIndex];
                    sorted3[k]=bPixel2D[rowIndex][colIndex];
                    k++;
                }
            }
            // Finding the count of each intensity in NxN neighborhood generated from the arrays from Step 1
            for(k=0;k<N*N;k++)
            {
                for(n=0;n<=255;n++)
                {
                    if (sorted1[k]==n)
                    {
                        intensityR[n]=intensityR[n]+1;
                    }
                    if (sorted2[k]==n)
                    {
                        intensityG[n]=intensityG[n]+1;
                    }
                    if (sorted3[k]==n)
                    {
                        intensityB[n]=intensityB[n]+1;
                    }
                    
                }
            }
            //Finding the intensity having the maximum count to be regarded as the most frequent intensity in the NxN neighborhood
            for(n=0;n<=255;n++)
            {
                if(intensityR[n]>max1)
                {
                    max1=intensityR[n];
                    max_value1=n;
                }
                if(intensityG[n]>max2)
                {
                    max2=intensityG[n];
                    max_value2=n;
                }
                if(intensityB[n]>max3)
                {
                    max3=intensityB[n];
                    max_value3=n;
                }
            }
            rPixel2DNew[i][j]=(unsigned char)max_value1;
            gPixel2DNew[i][j]=(unsigned char)max_value2;
            bPixel2DNew[i][j]=(unsigned char)max_value3;
        }
        
    }
    //Recombining the changed R,G,B channel arrays into a 1D array for displaying
     count=0;
     for(i=0;i<imageHeight;i++)
     {
        for(j=0;j<imageWidth;j++)
        {
            histogramArray[count]=rPixel2DNew[i][j];
            histogramArray[count+1]=gPixel2DNew[i][j];
            histogramArray[count+2]=bPixel2DNew[i][j];
            count=count+3;
        }
     }
    count=0;
    //To display 64 bit representation of the images
    for(i=0;i<imageHeight;i++)
    {
        for(j=0;j<imageWidth;j++)
        {
            combinedArray[count]=R64bit[i][j];
            combinedArray[count+1]=G64bit[i][j];
            combinedArray[count+2]=B64bit[i][j];
            count=count+3;
        }
    }
    
    // Write image data (filename specified by second argument) from image data matrix
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    //fwrite(combinedArray, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fwrite(histogramArray, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fclose(file);
    return 0;
}

