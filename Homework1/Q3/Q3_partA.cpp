#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace std;

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int bytesPerPixel=3;
    int imageHeight=512;
    int imageWidth=512;
    string fileName;
    FILE *newfile;
    
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
            fileName=argv[6];
        }
    }
    
    // Allocate image data array
    unsigned char imagedata[imageHeight*imageWidth*bytesPerPixel];
    unsigned char imagedataNew[imageHeight*imageWidth*bytesPerPixel];
    // Read image (filename specified by first and seventh argument) into image data matrix where 'rb' indicate reading a binary file
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }
    if (!(newfile=fopen(argv[7],"rb"))) {
        cout << "Cannot open new file: " << argv[7] <<endl;
        exit(1);
    }
    // format of fread is fread ( void * ptr, size of each clock of memory, count to indicate memory required, FILE to read from);
    fread(imagedata, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);        //Noisy image
    fread(imagedataNew, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, newfile); //Noise-free image
    fclose(file);
    fclose(newfile);
    
    ///////////////////////// PROCESSING CODE ///////////////////////
    
    
    unsigned char rPixel[imageHeight*imageWidth];
    unsigned char gPixel[imageHeight*imageWidth];
    unsigned char bPixel[imageHeight*imageWidth];
    unsigned char rPixel2D[imageHeight][imageWidth];
    unsigned char gPixel2D[imageHeight][imageWidth];
    unsigned char bPixel2D[imageHeight][imageWidth];
    unsigned char rPixelMean[imageHeight][imageWidth];
    unsigned char gPixelMean[imageHeight][imageWidth];
    unsigned char bPixelMean[imageHeight][imageWidth];
    unsigned char rPixelMedian[imageHeight][imageWidth];
    unsigned char gPixelMedian[imageHeight][imageWidth];
    unsigned char bPixelMedian[imageHeight][imageWidth];
    unsigned char rPixelOriginal[imageHeight][imageWidth];
    unsigned char gPixelOriginal[imageHeight][imageWidth];
    unsigned char bPixelOriginal[imageHeight][imageWidth];
    unsigned int intensityR[256]={0};
    unsigned int intensityG[256]={0};
    unsigned int intensityB[256]={0};
    unsigned char rgbArray[imageHeight*imageWidth*bytesPerPixel];
    int i,j,k;
    int N=5;  //Declaration of value of window size
    
     //Conversion of 1D noisy original image into seperate 1D R,G,B channels
    for (i = 0, j = 0; i < imageHeight*imageWidth && j < imageHeight*imageWidth* 3; i++, j = j + 3)
    {
        rPixel[i] = imagedata[j];
        gPixel[i]= imagedata[j+1];
        bPixel[i]= imagedata[j+2];
    }
    
    //Conversion of 1D input original image into seperate 2D R,G,B channels
    k=0;
    for (i=0;i<imageHeight;i++)
    {
        for(j=0;j<imageWidth;j++)
        {
            rPixelOriginal[i][j]=imagedataNew[k];
            gPixelOriginal[i][j]=imagedataNew[k+1];
            bPixelOriginal[i][j]=imagedataNew[k+2];
            k=k+3;
        }
    }
    
    //Finding the count of each intensity value for each Channel seperately
    for(k=0;k<=255;k++)
    {
        for(i=0;i<imageHeight*imageWidth;i++)
        {
            if (rPixel[i]==k)
            {
                intensityR[k] = intensityR[k]+1;
            }
            if (gPixel[i]==k)
            {
                intensityG[k]=intensityG[k]+1;
            }
            if (bPixel[i]==k)
            {
                intensityB[k]=intensityB[k]+1;
            }
        }
    }
    
    //Conversion of 1D array to 2D array for the noisy image to ease the computation
    k=0;
    for (i=0;i<imageHeight;i++)
    {
        for(j=0;j<imageWidth;j++)
        {
            rPixel2D[i][j]=rPixel[k];
            gPixel2D[i][j]=gPixel[k];
            bPixel2D[i][j]=bPixel[k];
            k++;
        }
    }
    
    //Code for noise removel- Using Median Filter
    int n,x,p,q,d,v=0;
    int sorted1[N*N];int sorted2[N*N];int sorted3[N*N];
    int sortedNew1[N*N];int sortedNew2[N*N];int sortedNew3[N*N];
    for(i=0;i<imageHeight;i++)
    {
        for(j=0;j<imageWidth;j++)
        {
            for(x=0;x<N*N;x++)
            {
                sorted1[x]={};
                sorted2[x]={};
                sorted3[x]={};
                sortedNew1[x]={};
                sortedNew2[x]={};
                sortedNew3[x]={};
            }
            k=0;
            int x=0;int y=0;int z=0;
            for(p=i-(N-1)/2;p<=i+(N-1)/2;p++) //Traversing the entire image using a NxN mask
            {
                for(q=j-(N-1)/2;q<=j+(N-1)/2;q++)
                {
                    int rowIndex=p;
                    int colIndex=q;
                    if(rowIndex<0)  // To check for boundary conditions to see if the mask index is excedding the image size
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
                    sorted1[k]=(int)rPixel2D[rowIndex][colIndex];
                    sorted2[k]=(int)gPixel2D[rowIndex][colIndex];
                    sorted3[k]=(int)bPixel2D[rowIndex][colIndex];
                    k++;
                }
            }
            //To sort the elements in the NxN neighborhood to find the median
            for(n=0;n<=255;n++)
            {
                for(d=0;d<N*N;d++)
                {
                    if(sorted1[d]==n)
                    {
                        sortedNew1[x]=n;
                        x++;
                    }
                    if(sorted2[d]==n)
                    {
                        sortedNew2[y]=n;
                        y++;
                    }
                    if(sorted3[d]==n)
                    {
                        sortedNew3[z]=n;
                        z++;
                    }
                }
            }
            //To find the median from the elements of theNxN neighborhood. The median is the central element in the array
            v=N*N;
            rPixelMedian[i][j]=(unsigned char)sortedNew1[(v-1)/2];  //Assigning the median of NxN neighborhood to the central element
            gPixelMedian[i][j]=(unsigned char)sortedNew2[(v-1)/2];
            bPixelMedian[i][j]=(unsigned char)sortedNew3[(v-1)/2];
        }
    }
    
    //Code for noise removel- Using Mean Filter after the median filter
    for(i=0;i<imageHeight;i++)
    {
        for(j=0;j<imageWidth;j++)
        {
            for(x=0;x<N*N;x++)
            {
                sorted1[x]={};
                sorted2[x]={};
                sorted3[x]={};
                sortedNew1[x]={};
                sortedNew2[x]={};
                sortedNew3[x]={};
            }
            k=0;
            int sum1=0; int sum2=0;int sum3=0;
            int sum1Final=0;int sum2Final=0;int sum3Final=0;
            for(p=i-(N-1)/2;p<=i+(N-1)/2;p++)  //Traversing the entire image using a NxN mask
            {
                for(q=j-(N-1)/2;q<=j+(N-1)/2;q++)
                {
                    int rowIndex=p;
                    int colIndex=q;
                    if(rowIndex<0)  //To check for boundary conditions to see if the mask index is excedding the image size
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
                    sorted1[k]=(int)rPixelMedian[rowIndex][colIndex];
                    sorted2[k]=(int)gPixelMedian[rowIndex][colIndex];
                    sorted3[k]=(int)bPixelMedian[rowIndex][colIndex];
                    k++;
                }
            }
            //Find the sum of all elements in the NxN neighborhood to get the mean. Mean=Sum of all elements/ Total number of elements
            for(d=0;d<N*N;d++)
            {
                sum1=sum1+sorted1[d];
                sum2=sum2+sorted2[d];
                sum3=sum3+sorted3[d];
            }
            sum1Final=(sum1/(N*N));
            sum2Final=(sum2/(N*N));
            sum3Final=(sum3/(N*N));
            rPixelMean[i][j]=(unsigned char)sum1Final;  //Assigning the mean of NxN neighborhood to the central element
            gPixelMean[i][j]=(unsigned char)sum2Final;
            bPixelMean[i][j]=(unsigned char)sum3Final;
        }
    }
    
    //Combining the R,G,B channels after noise removal by median and mean filters in cascade to get a noise removed 1D image
    int count=0;
    for(i=0;i<imageHeight;i++)
    {
        for(j=0;j<imageWidth;j++)
        {
            rgbArray[count]=rPixelMean[i][j];
            rgbArray[count+1]=gPixelMean[i][j];
            rgbArray[count+2]=bPixelMean[i][j];
            count=count+3;
        }
    }
   
    //Calculation for psnr values of R,G,B channels
    //Formula for PSRN= 10log10((max intensity)^2/MSE)
    //Formula for MSE= 1/size of image((noise removed image- given noise removed image)^2)
    float max=255;
    float sumR=0,sumG=0,sumB=0,sum=0;
    float mseR=0,mseG=0,mseB=0,mse=0;
    float psnrR=0,psnrG=0,psnrB=0,psnr=0;
    for(i=0;i<imageHeight;i++)
    {
        for(j=0;j<imageWidth;j++)
        {
            sumR=pow((rPixelMedian[i][j]-rPixelOriginal[i][j]),2);
            mseR=mseR+sumR;
            sumG=pow((gPixelMedian[i][j]-gPixelOriginal[i][j]),2);
            mseG=mseG+sumG;
            sumB=pow((bPixelMean[i][j]-bPixelOriginal[i][j]),2);
            mseB=mseB+sumB;
        }
    }
    //Calculation for psnr values for the entire image
    for(i=0;i<imageHeight*imageWidth*bytesPerPixel;i++)
    {
        sum=pow((rgbArray[i]-imagedataNew[i]),2);
        mse=mse+sum;
    }
    mse=mse/(imageHeight*imageWidth*bytesPerPixel);
    mseR=mseR/(imageHeight*imageWidth);
    mseG=mseG/(imageHeight*imageWidth);
    mseB=mseB/(imageHeight*imageWidth);
    psnr=10.00*log10((max*max)/mse);
    psnrR=10.00*log10((max*max)/mseR);
    psnrG=10.00*log10((max*max)/mseG);
    psnrB=10.00*log10((max*max)/mseB);
    cout<<"psnr for entire image is: ";
    cout<<psnr<<endl;
    cout<<"psnr for R channel: ";
    cout<<endl<<psnrR<<endl;
    cout<<"psnr for G channel: ";
    cout<<endl<<psnrG<<endl;
    cout<<"psnr for B channel: ";
    cout<<endl<<psnrB<<endl;
    
     //Writing the intensity count for R,G,B channels into a text file to plot the histograms
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
    
    //Write image data (filename specified by second argument) from image data matrix
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    
    //copies contents from file pointed by imagedata to the filename given in the input as the second argument
    //fwrite(rPixel, sizeof(unsigned char), imageHeight*imageWidth, file);
    fwrite(rgbArray, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fclose(file);
    return 0;
}


 
