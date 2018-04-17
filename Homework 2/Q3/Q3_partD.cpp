#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include<string.h>
#include<vector>

using namespace std;
int object[372][372];
vector<vector<int>> vect;

vector<int> flipVerticle(vector<int> inp)
{
    vector<int> a;
    a.push_back(inp[2]);
    a.push_back(inp[1]);
    a.push_back(inp[0]);
    a.push_back(inp[3]);
    return a;
}

vector<int> rotateleft(vector<int> inp)
{
    vector<int> a;
    a.push_back(inp[1]);
    a.push_back(inp[2]);
    a.push_back(inp[3]);
    a.push_back(inp[0]);
    return a;
}

bool checkEqual(vector<int> inp,vector<int> inp2 ){
    
    if(inp[0]==inp2[0] &&inp[1]==inp2[1] &&inp[2]==inp2[2] &&inp[3]==inp2[3]){
        
        return true;
    }
    return false;
}

int getDiff(vector<vector<int>> a)
{
    int c =16;
    int c1=0;
    cout<<"Number of unique and matched pieces in every iteration: "<<endl;
    for(int i=0;i<a.size();i++){
        
        c1=0;
        vector<int> temp = a[i];
        vector<int> ver = flipVerticle(temp);
        vector<int> temp1 = rotateleft(temp);
        vector<int> temp2 = rotateleft(temp1);
        vector<int> temp3 = rotateleft(temp2);
        vector<int> ver1 = rotateleft(ver);
        vector<int> ver2 = rotateleft(ver1);
        vector<int> ver3 = rotateleft(ver2);
        cout<<i<<" ";
        for(int j=i+1;j<a.size();j++)
        {
            vector<int> b=a[j];
            if( checkEqual(b, temp)  || checkEqual(b,  temp1)  ||checkEqual(b,temp2) || checkEqual(b,  temp3) ||checkEqual(b, ver) || checkEqual(b,  ver1) ||checkEqual(b,  ver2) || checkEqual(b,  ver3) )  //To check for all possible combination of one object with respecto to other objects
            {
                c1++;
                cout<<j<<" ";
            }
        }
        cout<<endl;
        if(c1>=1)
            c1=1;
        c-=c1;
    }
    cout<<endl;
    return c;
}



int generateVector(float** objectholes)
{
    for(int i=0;i<16;i++)
    {
        vector<int> temp;
        for(int j=0;j<4;j++){
            temp.push_back(objectholes[i][j]);
        }
        vect.push_back(temp);
    }
  int c=getDiff(vect);
    return(c);
}

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

void getnormalized(unsigned char** rPixel2D,float **rPixel2Dfloat, int imageHeight, int imageWidth)
{
    int rowIndex,colIndex;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            if(rPixel2D[rowIndex][colIndex]<=127)
            {
                rPixel2Dfloat[rowIndex][colIndex]=0;
            }
            else
            {
                rPixel2Dfloat[rowIndex][colIndex]=1;
            }
        }
    }
}

void getoriginalrangevalue(float** rPixelfloat,unsigned char **rPixel2Dnew, int imageHeight, int imageWidth)
{
    int rowIndex,colIndex;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            rPixel2Dnew[rowIndex][colIndex]=rPixelfloat[rowIndex][colIndex]*255;
        }
    }
}

void getinvertedimage(float** rPixel2Dfloat, int imageHeight, int imageWidth,float** rpixelinvertedfloat)
{
    
    int rowIndex,colIndex;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            if(rPixel2Dfloat[rowIndex][colIndex]==0)
            {
                rpixelinvertedfloat[rowIndex][colIndex]=1;
            }
            else
            {
                rpixelinvertedfloat[rowIndex][colIndex]=0;
            }
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

void geterode(float** rpixelfloat,int imageHeight,int imageWidth,float** routputarray)
{
    int rowIndex,colIndex,p,q,traversalIndex=0,temp,count;
    int N=11;
    int maskarray[N*N];
    count=0;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            for(int x=0;x<N*N;x++)
            {
                maskarray[x]=0;
            }
            traversalIndex=0;
            temp=0;
            for(p=rowIndex-(N-1)/2;p<=rowIndex+(N-1)/2;p++)
            {
                for(q=colIndex-(N-1)/2;q<=colIndex+(N-1)/2;q++)
                {
                    int row=p;
                    int col=q;
                    if(row<0)
                    {
                        row=0;
                    }
                    if(row>imageHeight-1)
                    {
                        row=imageHeight-1;
                    }
                    if(col<0)
                    {
                        col=0;
                    }
                    if(col>imageWidth-1)
                    {
                        col=imageWidth-1;
                    }
                    maskarray[traversalIndex]=rpixelfloat[row][col];
                    traversalIndex++;
                }
            }
            //If all the elements in the NxN neighborhood array ahve value 1, that pixel can retain the value of foreground
            for(int i=0;i<N*N;i++)
            {
                if(maskarray[i]==1)
                {
                    temp++;
                }
            }
            if(temp==121)
            {
                routputarray[rowIndex][colIndex]=1;
            }
        }
    }
}


void getdilate(float** rpixelfloat,int imageHeight,int imageWidth,float** routputarray)
{
    int rowIndex,colIndex,p,q,traversalIndex=0,temp,count;
    int N=27;
    int maskarray[N*N];
    
    count=0;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            for(int x=0;x<N*N;x++)
            {
                maskarray[x]=0;
            }
            traversalIndex=0;
            temp=0;
            for(p=rowIndex-(N-1)/2;p<=rowIndex+(N-1)/2;p++)
            {
                for(q=colIndex-(N-1)/2;q<=colIndex+(N-1)/2;q++)
                {
                    int row=p;
                    int col=q;
                    if(row<0)
                    {
                        row=0;
                    }
                    if(row>imageHeight-1)
                    {
                        row=imageHeight-1;
                    }
                    if(col<0)
                    {
                        col=0;
                    }
                    if(col>imageWidth-1)
                    {
                        col=imageWidth-1;
                    }
                    maskarray[traversalIndex]=rpixelfloat[row][col];
                    traversalIndex++;
                }
            }
            //If any of the element in the NxN neighborhood array ahve value 1, that pixel can retain the value of foreground
            for(int i=0;i<N*N;i++)
            {
                if(maskarray[i]==1)
                {
                    temp++;
                }
            }
            if(temp!=0)
            {
                routputarray[rowIndex][colIndex]=1;
            }
        }
    }
}

void getshrinking(float** rPixelfloat, int imageHeight, int imageWidth,int** M, float** routputArray)
{
    int iterationIndex,rowIndex,colIndex,p,q,traversalIndex=0,temp,count;
    int maskarray[9];
    //Defining the unconditional masks in the form of strings for ease of comparison later
    string bond1[4]={"001010000","100010000","000010100","000010001"};
    string bond2[4]={"000011000","010010000","000110000","000010010"};
    string bond3[8]={"001011000","011010000","110010000","100110000","000110100","000010110","000010011","000011001"};
    string bond4[4]={"001011001","111010000","100110100","000010111"};
    string bond5[8]={"110011000","010011001","011110000","001011010","011011000","110110000","000110110","000011011"};
    string bond6[10]={"110011001","011110100","111011000","011011001","111110000","110110100","100110110","000110111","000011111","001011011"};
    string bond7[4]={"111011001","111110100","100110111","001011111"};
    string bond8[4]={"000111111","011011011", "111111000", "110110110"};
    string bond9[8]={"111011011","011011111","111111100","111111001","111110110","110110111","100111111","001111111"};
    string bond10[4]={"111011111","111111101","111110111","101111111"};
    
    int bond=0;
    string str;
    //------Step 1----- detecting boundary to be peeled off in step 2
    for(iterationIndex=0;iterationIndex<50;iterationIndex++)
    {
        count=0;
        for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
        {
            for(colIndex=0;colIndex<imageWidth;colIndex++)
            {
                if (rPixelfloat[rowIndex][colIndex]==0)  //To check in center element is 0
                {
                    M[rowIndex][colIndex]=0;
                }
                else  //to start processing if the center element is 1
                {
                    for(int x=0;x<9;x++)
                    {
                        maskarray[x]=0;
                    }
                    traversalIndex=0;
                    str="\0";
                    for(p=rowIndex-1;p<=rowIndex+1;p++)
                    {
                        for(q=colIndex-1;q<=colIndex+1;q++)
                        {
                            int row=p;
                            int col=q;
                            if(row<0)
                            {
                                row=0;
                            }
                            if(row>imageHeight-1)
                            {
                                row=imageHeight-1;
                            }
                            if(col<0)
                            {
                                col=0;
                            }
                            if(col>imageWidth-1)
                            {
                                col=imageWidth-1;
                            }
                            maskarray[traversalIndex]=rPixelfloat[row][col];  //Reading contents of input image in array of 9 elements
                            traversalIndex++;
                        }
                    }
                    //Converting 9 array elements obtained from input image into a string
                    for(int i=0;i<9;i++)
                    {
                        str=str+to_string(maskarray[i]);
                    }
                    //Calculating the bond of the array -- Diagonal elements are given weight=1 and off diagonal elements are given weight=2
                    bond = 2 * (maskarray[1] + maskarray[3] + maskarray[5] + maskarray[7]) + 1 * (maskarray[0] + maskarray[2] + maskarray[6] + maskarray[8]);
                    traversalIndex=0;
                    temp=0;
                     //Check for the bond value for the mask obtianed from input image and compare with the unconditional masks
                    if(bond==1)
                    {
                        for(int i=0;i<4;i++)
                        {
                            if(str==bond1[i])
                            {
                                temp++;
                            }
                        }
                    }
                    if(bond==2)
                    {
                        for(int i=0;i<4;i++)
                        {
                            if(str==bond2[i])
                            {
                                temp++;
                            }
                        }
                    }
                    if(bond==3)
                    {
                        for(int i=0;i<8;i++)
                        {
                            if(str==bond3[i])
                            {
                                temp++;
                            }
                        }
                    }
                    if(bond==4)
                    {
                        for(int i=0;i<4;i++)
                        {
                            if(str==bond4[i])
                            {
                                temp++;
                            }
                        }
                    }
                    if (bond==5)
                    {
                        for(int i=0;i<8;i++)
                        {
                            if(str==bond5[i])
                            {
                                temp++;
                            }
                        }
                    }
                    if (bond==6)
                    {
                        for(int i=0;i<10;i++)
                        {
                            if(str==bond6[i])
                            {
                                temp++;
                            }
                        }
                    }
                    if (bond==7)
                    {
                        for(int i=0;i<4;i++)
                        {
                            if(str==bond7[i])
                            {
                                temp++;
                            }
                        }
                    }
                    if (bond==8)
                    {
                        for(int i=0;i<4;i++)
                        {
                            if(str==bond8[i])
                            {
                                temp++;
                            }
                        }
                    }
                    if (bond==9)
                    {
                        for(int i=0;i<8;i++)
                        {
                            if(str==bond9[i])
                            {
                                temp++;
                            }
                        }
                    }
                    if (bond==10)
                    {
                        for(int i=0;i<4;i++)
                        {
                            if(str==bond10[i])
                            {
                                temp++;
                            }
                        }
                    }
                    //If any mask matches the input image mask of NxN neighborhood, generate M=1
                    if(temp==1)
                    {
                        M[rowIndex][colIndex]=1;
                    }
                    else
                    {
                        M[rowIndex][colIndex]=0;
                    }
                }
            }
        }
        //------Step 2---- Peeling of the boundary generated in Step 1
        for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
        {
            for(colIndex=0;colIndex<imageWidth;colIndex++)
            {
                if (M[rowIndex][colIndex]==0)
                {
                    routputArray[rowIndex][colIndex]=rPixelfloat[rowIndex][colIndex];  //If M array generated in Step 1 has value 0, copy input pixel to output pixel
                }
                else
                {
                    for(int x=0;x<9;x++)
                    {
                        maskarray[x]={0};
                    }
                    traversalIndex=0;
                    for(p=rowIndex-1;p<=rowIndex+1;p++)
                    {
                        for(q=colIndex-1;q<=colIndex+1;q++)
                        {
                            int row=p;
                            int col=q;
                            if(row<0)
                            {
                                row=0;
                            }
                            if(row>imageHeight-1)
                            {
                                row=imageHeight-1;
                            }
                            if(col<0)
                            {
                                col=0;
                            }
                            if(col>imageWidth-1)
                            {
                                col=imageWidth-1;
                            }
                            maskarray[traversalIndex]=M[row][col];
                            traversalIndex++;
                        }
                    }
                    //Comparing the 9 element array genarated to conditional masks
                    temp=0;
                    //Spur
                    if(maskarray[0]==0 && maskarray[1]==0 && maskarray[2]==1 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==0 && maskarray[6]==0 && maskarray[7]==0 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    if(maskarray[0]==1 && maskarray[1]==0 && maskarray[2]==0 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==0 && maskarray[6]==0 && maskarray[7]==0 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    //Single 4 connection
                    if(maskarray[0]==0 && maskarray[1]==0 && maskarray[2]==0 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==0 && maskarray[6]==0 && maskarray[7]==1 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    if(maskarray[0]==0 && maskarray[1]==0 && maskarray[2]==0 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==1 && maskarray[6]==0 && maskarray[7]==0 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    //L-cluster
                    if(maskarray[0]==0 && maskarray[1]==0 && maskarray[2]==1 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==1 && maskarray[6]==0 && maskarray[7]==0 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    if(maskarray[0]==0 && maskarray[1]==1 && maskarray[2]==1 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==0 && maskarray[6]==0 && maskarray[7]==0 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    if(maskarray[0]==1 && maskarray[1]==1 && maskarray[2]==0 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==0 && maskarray[6]==0 && maskarray[7]==0 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    if(maskarray[0]==1 && maskarray[1]==0 && maskarray[2]==0 && maskarray[3]==1 && maskarray[4]==1 && maskarray[5]==0 && maskarray[6]==0 && maskarray[7]==0 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    if(maskarray[0]==0 && maskarray[1]==0 && maskarray[2]==0 && maskarray[3]==1 && maskarray[4]==1 && maskarray[5]==0 && maskarray[6]==1 && maskarray[7]==0 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    if(maskarray[0]==0 && maskarray[1]==0 && maskarray[2]==0 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==0 && maskarray[6]==1 && maskarray[7]==1 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    if(maskarray[0]==0 && maskarray[1]==0 && maskarray[2]==0 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==0 && maskarray[6]==0 && maskarray[7]==1 && maskarray[8]==1)
                    {
                        temp++;
                    }
                    if(maskarray[0]==0 && maskarray[1]==0 && maskarray[2]==0 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==1 && maskarray[6]==0 && maskarray[7]==0 && maskarray[8]==1)
                    {
                        temp++;
                    }
                    //4-connected offset
                    if(maskarray[0]==0 && maskarray[1]==1 && maskarray[2]==1 && maskarray[3]==1 && maskarray[4]==1 && maskarray[5]==0 && maskarray[6]==0 && maskarray[7]==0 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    if(maskarray[0]==1 && maskarray[1]==1 && maskarray[2]==0 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==1 && maskarray[6]==0 && maskarray[7]==0 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    if(maskarray[0]==0 && maskarray[1]==1 && maskarray[2]==0 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==1 && maskarray[6]==0 && maskarray[7]==0 && maskarray[8]==1)
                    {
                        temp++;
                    }
                    if(maskarray[0]==0 && maskarray[1]==0 && maskarray[2]==1 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==1 && maskarray[6]==0 && maskarray[7]==1 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    //Spur corner cluster
                    if(maskarray[0]==0 && (maskarray[1] || maskarray[5]==1) && maskarray[2]==1 && maskarray[3]==0 && maskarray[4]==1 && maskarray[6]==1 && maskarray[7]==0 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    if(maskarray[0]==1 && (maskarray[1] || maskarray[3]==1) && maskarray[2]==0 && maskarray[4]==1 && maskarray[5]==0 && maskarray[6]==0 && maskarray[7]==0 && maskarray[8]==1)
                    {
                        temp++;
                    }
                    if(maskarray[0]==0 && maskarray[1]==0 && maskarray[2]==1 && (maskarray[3]||maskarray[7]==1) && maskarray[4]==1 && maskarray[5]==0 && maskarray[6]==1 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    if(maskarray[0]==1 && maskarray[1]==0 && maskarray[2]==0 && maskarray[3]==0 && maskarray[4]==1 &&  (maskarray[5]||maskarray[7]==1) && maskarray[6]==0 && maskarray[8]==1)
                    {
                        temp++;
                    }
                    //Corner cluster
                    if(maskarray[0]==1 && maskarray[1]==1 &&  maskarray[3]==1 && maskarray[4]==1)
                    {
                        temp++;
                    }
                    //Tee Branch
                    if(maskarray[1]==1 && maskarray[2]==0 && maskarray[3]==1 && maskarray[4]==1 && maskarray[5]==1 && maskarray[7]==0 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    if(maskarray[0]==0 && maskarray[1]==1 && maskarray[3]==1 && maskarray[4]==1 && maskarray[5]==1 && maskarray[6]==0 && maskarray[7]==0)
                    {
                        temp++;
                    }
                    if(maskarray[0]==0 && maskarray[1]==0 && maskarray[3]==1 && maskarray[4]==1 && maskarray[5]==1 && maskarray[6]==0 && maskarray[7]==1)
                    {
                        temp++;
                    }
                    if(maskarray[1]==0 && maskarray[2]==0 && maskarray[3]==1 && maskarray[4]==1 && maskarray[5]==1 && maskarray[7]==1 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    if(maskarray[1]==1 && maskarray[3]==1 && maskarray[4]==1 && maskarray[5]==0 && maskarray[6]==0 && maskarray[7]==1 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    if(maskarray[0]==0 && maskarray[1]==1 && maskarray[2]==0 && maskarray[3]==1 && maskarray[4]==1 && maskarray[5]==0 && maskarray[7]==1)
                    {
                        temp++;
                    }
                    if(maskarray[0]==0 && maskarray[1]==1 && maskarray[2]==0 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==1 && maskarray[7]==1)
                    {
                        temp++;
                    }
                    if(maskarray[1]==1 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==1 && maskarray[6]==0 && maskarray[7]==1 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    //Vee Branch
                    if(maskarray[0]==1 && (maskarray[6] || maskarray[7] || maskarray[8]==1) && maskarray[2]==1 && maskarray[4]==1)
                    {
                        temp++;
                    }
                    if(maskarray[0]==1 && (maskarray[2] || maskarray[5] || maskarray[8]==1) && maskarray[4]==1 && maskarray[6]==1)
                    {
                        temp++;
                    }
                    if((maskarray[0] || maskarray[1] || maskarray[2]==1) && maskarray[4]==1 && maskarray[6]==1 && maskarray[8]==1)
                    {
                        temp++;
                    }
                    if((maskarray[0] || maskarray[3] || maskarray[6]==1) && maskarray[2]==1 && maskarray[4]==1 && maskarray[8]==1)
                    {
                        temp++;
                    }
                    //Diagonal Branch
                    if(maskarray[1]==1 && maskarray[2]==0 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==1 && maskarray[6]==1 && maskarray[7]==0)
                    {
                        temp++;
                    }
                    if(maskarray[0]==0 && maskarray[1]==1 && maskarray[3]==1 && maskarray[4]==1 && maskarray[5]==0 && maskarray[7]==0 && maskarray[8]==1)
                    {
                        temp++;
                    }
                    if(maskarray[1]==0 && maskarray[2]==1 && maskarray[3]==1 && maskarray[4]==1 && maskarray[5]==0 && maskarray[6]==0 && maskarray[7]==1)
                    {
                        temp++;
                    }
                    if(maskarray[0]==1 && maskarray[1]==0 && maskarray[3]==0 && maskarray[4]==1 && maskarray[5]==1 && maskarray[7]==1 && maskarray[8]==0)
                    {
                        temp++;
                    }
                    //If no match is found with any conditional mask, output=0 else output=input
                    if(temp==0)
                    {
                        routputArray[rowIndex][colIndex]=0;
                    }
                    else
                    {
                        routputArray[rowIndex][colIndex]=rPixelfloat[rowIndex][colIndex];
                    }
                    rPixelfloat[rowIndex][colIndex]=routputArray[rowIndex][colIndex];
                }
            }
        }
    }
}

int getobjectsforcounting(unsigned char** rpixelshrinked,int imageHeight, int imageWidth)
{
    int rowIndex,colIndex,count=0;
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            if(rpixelshrinked[rowIndex][colIndex]==255)  // To count number of white dots in the image indicating the shrinked objects of the original image
            {
                count++;
            }
        }
    }
    return(count);
}

void mergee(int i, int j,int countobject,float** origimage,int imageHeight,int imageWidth)
{
    if(i<0||j<0||i>=imageHeight||j>=imageWidth||origimage[i][j]==0)
        return;
    
    object[i][j]=countobject;
    
    if(i<imageHeight-1 && object[i+1][j]!=countobject)
    mergee(i+1, j,countobject,origimage,imageHeight,imageWidth);
    
    if(j<imageWidth-1 && object[i][j+1]!=countobject)
    mergee(i, j+1,countobject,origimage,imageHeight,imageWidth);
    
    if(i>0 && j>0 && object[i-1][j-1]!=countobject)
    mergee(i-1, j-1,countobject,origimage,imageHeight,imageWidth);
    
    if(i>0 && object[i-1][j]!=countobject)
    mergee(i-1, j,countobject,origimage,imageHeight,imageWidth);
    
    if(i>0 && j<imageWidth-1 && object[i-1][j+1]!=countobject)
    mergee(i-1, j+1,countobject,origimage,imageHeight,imageWidth);
    
    if(j>0 && object[i][j-1]!=countobject)
    mergee(i, j-1,countobject,origimage,imageHeight,imageWidth);
    
    if(i<imageHeight-1 && j>0 && object[i+1][j-1]!=countobject)
    mergee(i+1, j-1,countobject,origimage,imageHeight,imageWidth);
}

void numIslands(float** origimage,int imageHeight,int imageWidth)
{
    int countobject=0;
    for(int i=0; i<imageHeight; i++)
    {
        for(int j=0; j<imageWidth; j++)
        {
            object[i][j]=0;
        }
    }
    for(int i=0; i<imageHeight; i++)
    {
        for(int j=0; j<imageWidth; j++)
        {
            if(origimage[i][j]!=0 && object[i][j]==0)
            {
                countobject++;
                mergee(i, j,countobject,origimage,imageHeight,imageWidth);
            }
        }
    }
}

void getobjects(float** inputarray,int imageHeight,int imageWidth,float** outputarray)
{
    int maskarray[4];
    int p[4];
    int rowIndex,colIndex;
    int label1=2,label2=0,temp;
    //First pass for Connected Component Analysis
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            if(inputarray[rowIndex][colIndex]==1)
            {
                if(rowIndex>0 && colIndex>0 && colIndex<imageWidth-1 && rowIndex<imageHeight-1)
                {
                    maskarray[0]=inputarray[rowIndex-1][colIndex+1];
                    maskarray[1]=inputarray[rowIndex-1][colIndex];
                    maskarray[2]=inputarray[rowIndex-1][colIndex-1];
                    maskarray[3]=inputarray[rowIndex][colIndex-1];
                    if(maskarray[0]==0 && maskarray[1]==0 && maskarray[2]==0 && maskarray[3]==0)
                    {
                        inputarray[rowIndex][colIndex]=label1;
                        label1++;
                    }
                    else
                    {
                        int k=0;
                        for(int x=0;x<4;x++)
                        {
                            if (maskarray[x]!=0)
                            {
                                p[k]=maskarray[x];
                                k++;
                            }
                        }
                        for(int y=0;y<k-1;y++)
                        {
                            for(int z=0;z<k-y-1;z++)
                            {
                                if(p[z]>p[z+1])
                                {
                                    temp=p[z];
                                    p[z]=p[z+1];
                                    p[z+1]=temp;
                                }
                            }
                        }
                        label2=p[0];
                        inputarray[rowIndex][colIndex]=label2;
                    }
                }
            }
        }
    }
    //Second pass for Connected Component Analysis
   numIslands(inputarray,imageHeight,imageWidth);
    
    for(rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(colIndex=0;colIndex<imageWidth;colIndex++)
        {
            outputarray[rowIndex][colIndex]=object[rowIndex][colIndex];
        }
    }
}

void getobjectsandprotrusions(int xindex,int yindex,int label,float** XORarray,float** objectholes)
{
    int xstart,ystart,count=0;
    xstart=xindex+23;
    ystart=yindex+23;
    //holes=1,protrusion=-1,nothing=0
     //For counting holes nad protrusion in upward direction
    for(int i=xstart;i>=xindex-2;i--)
    {
        if(XORarray[i][ystart]==0)
        {
            count++;
        }
    }
        if(count<23)
        {
            objectholes[label][0]=1;
        }
        if(count==24||count==23)
        {
            objectholes[label][0]=-1;
        }
        if(count>24)
        {
            objectholes[label][0]=0;
        }
    count=0;
    //For counting holes nad protrusion in right direction
    for(int i=ystart;i<=ystart+25;i++)
    {
        if(XORarray[xstart][i]==0)
        {
            count++;
        }
    }
    if(count<23)
    {
        objectholes[label][1]=1;
    }
    if(count==24||count==23)
    {
        objectholes[label][1]=-1;
    }
    if(count>24)
    {
        objectholes[label][1]=0;
    }
    count=0;
    ///For counting holes nad protrusion in downward direction
    for(int i=xstart;i<=xstart+25;i++)
    {
        if(XORarray[i][ystart]==0)
        {
            count++;
        }
    }
    if(count<23)
    {
        objectholes[label][2]=1;
    }
    if(count==24||count==23)
    {
        objectholes[label][2]=-1;
    }
    if(count>24)
    {
        objectholes[label][2]=0;
    }
    count=0;
    //For counting holes nad protrusion in left direction
    for(int i=ystart;i>=yindex-2;i--)
    {
        if(XORarray[xstart][i]==0)
        {
            count++;
        }
    }
    if(count<23)
    {
        objectholes[label][3]=1;
    }
    if(count==24||count==23)
    {
        objectholes[label][3]=-1;
    }
    if(count>24)
    {
        objectholes[label][3]=0;
    }
}

int main(int argc, char *argv[])

{
    // Define file pointer and variables
    FILE *file;
    int bytesPerPixel;
    int imageHeight=100;
    int imageWidth=100;
    
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
    // format of fread is fread ( void * ptr, size of each clock of memory, temp to indicate memory required, FILE to read from);
    fread(Imagedata, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fclose(file);
    
    ///////////////////////// PROCESSING CODE ///////////////////////
    unsigned char combinedArray[imageHeight*imageWidth*bytesPerPixel];
    unsigned char combinedArrayshrinking[imageHeight*imageWidth*bytesPerPixel];
    
    unsigned char** rpixel2Ddummy=new  unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** rpixel2D=rpixel2Ddummy;
    
    float** rpixel2Ddummy10=new  float* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy10[i]=new float[imageWidth];
        memset(rpixel2Ddummy10[i],0,imageWidth*sizeof(float));
    }
    float** routputArray=rpixel2Ddummy10;
    
    float** rpixel2Ddummy4=new  float* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy4[i]=new float[imageWidth];
        memset(rpixel2Ddummy4[i],0,imageWidth*sizeof(float));
    }
    float** rpixelfloat=rpixel2Ddummy4;
    
    float** rpixel2Ddummy13=new  float* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy13[i]=new float[imageWidth];
        memset(rpixel2Ddummy13[i],0,imageWidth*sizeof(float));
    }
    float** rpixelinvertedfloat=rpixel2Ddummy13;
    
    float** rpixel2Ddummy151=new  float* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy151[i]=new float[imageWidth];
        memset(rpixel2Ddummy151[i],0,imageWidth*sizeof(float));
    }
    float** originalimage=rpixel2Ddummy151;
    
    float squareArray[imageHeight][imageWidth];
    memset(squareArray,0,imageHeight*imageWidth*sizeof(float));
    
    float** rpixel2Ddummy154=new  float* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy154[i]=new float[imageWidth];
        memset(rpixel2Ddummy154[i],0,imageWidth*sizeof(float));
    }
    float** XORarray=rpixel2Ddummy154;
    
    float** rpixel2Ddummy150=new  float* [16];
    for(int i=0;i<16;i++)
    {
        rpixel2Ddummy150[i]=new float[4];
        memset(rpixel2Ddummy150[i],0,4*sizeof(float));
    }
    float** objectholes=rpixel2Ddummy150;
    
    float** rpixel2Ddummy16=new  float* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy16[i]=new float[imageWidth];
        memset(rpixel2Ddummy16[i],0,imageWidth*sizeof(float));
    }
    float** rpixeldilate=rpixel2Ddummy16;
    
    float** rpixel2Ddummy17=new  float* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy17[i]=new float[imageWidth];
        memset(rpixel2Ddummy17[i],0,imageWidth*sizeof(float));
    }
    float** rpixelerode=rpixel2Ddummy17;
    
    int** rpixel2Ddummy9=new  int* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy9[i]=new int[imageWidth];
        memset(rpixel2Ddummy9[i],0,imageWidth*sizeof(int));
    }
    int** M=rpixel2Ddummy9;
    
    float** rpixel2Ddummy101=new  float* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy101[i]=new float[imageWidth];
        memset(rpixel2Ddummy101[i],0,imageWidth*sizeof(float));
    }
    float** routputArrayshrinking=rpixel2Ddummy101;
    
    unsigned char** rpixel2Ddummy455=new  unsigned char* [imageHeight];
    for(int i=0;i<imageHeight;i++)
    {
        rpixel2Ddummy455[i]=new unsigned char[imageWidth];
        memset(rpixel2Ddummy455[i],0,imageWidth*sizeof(unsigned char));
    }
    unsigned char** rpixel2Dshrinked=rpixel2Ddummy455;
    
    //Converting the 1D input image into R channel(the input image is a gray image)
    get2Dchannel(Imagedata,imageHeight,imageWidth,rpixel2D);
    //Normalizing the values of pixels from 0 to 1
    getnormalized(rpixel2D,rpixelfloat,imageHeight,imageWidth);
    //Inverting the original image to get white foreground and black background
    getinvertedimage(rpixelfloat,imageHeight,imageWidth,rpixelinvertedfloat);
    
    //PART 1--COUNTING NUMBER OF OBJECTS
    getinvertedimage(rpixelfloat,imageHeight,imageWidth,originalimage);
    //Apply erosion algorithm on the image
    geterode(originalimage,imageHeight,imageWidth,rpixelerode);
    //Apply dilation algorithm on the eroded image
    getdilate(rpixelerode,imageHeight,imageWidth,rpixeldilate);
    //Apply shrinking algorithm in the eroded and dilated image
    getshrinking(rpixeldilate,imageHeight,imageWidth, M, routputArrayshrinking);
    //Converting the values back to 0 to 255 range
    getoriginalrangevalue(routputArrayshrinking,rpixel2Dshrinked,imageHeight,imageWidth);
    
    //To cound=t number of objects in the picture
    int count=getobjectsforcounting(rpixel2Dshrinked,imageHeight,imageWidth);
    cout<<"Total number of objects in the image are: "<<endl<<count<<endl<<endl;
   
    //Combining the R channel to a single image
    combinechannel(rpixel2Dshrinked,combinedArrayshrinking,imageHeight,imageWidth);
  
    //PART2---COUNTING UNIQUE OBJECTS
    getobjects(rpixelinvertedfloat,imageHeight,imageWidth, routputArray);
    
    //Gerating a dummy square array
    int yindex[5000]={0};
    int xindexvalue[16];
    int yindexvalue[16];
    int minx,temp;
    int k=0,i,miny=0;
    int positive[5000]={0};
    int a=0;
    int n=0;
    //Calculating starting x and y index for each object in the image
    for(i=1;i<=16;i++)
    {
        int m=0;
        memset(yindex,0,5000*sizeof(int));
        for(int rowIndex=0;rowIndex<imageHeight;rowIndex++)
        {
            for(int colIndex=0;colIndex<imageWidth;colIndex++)
            {
                if(routputArray[rowIndex][colIndex]==i)
                {
                    yindex[m]=colIndex;
                    m++;
                }
            }
        }
        k=0;
        memset(positive,0,5000*sizeof(int));
        for(int x=0;x<5000;x++)
        {
            if (yindex[x]!=0)
            {
                positive[k]=yindex[x];
                k++;
            }
        }
        for(int y=0;y<k-1;y++)
        {
            for(int z=0;z<k-y-1;z++)
            {
                if(positive[z]>positive[z+1])   //Sorting in ascending order using bubble sort
                {
                    temp=positive[z];
                    positive[z]=positive[z+1];
                    positive[z+1]=temp;
                }
            }
        }
        miny=positive[0];
        yindexvalue[n]=miny;
        n++;
        minx=0;
        for(int rowIndex=0;rowIndex<imageHeight;rowIndex++)
        {
            for(int colIndex=0;colIndex<imageWidth;colIndex++)
            {
                if(routputArray[rowIndex][miny]==i)
                {
                    minx=rowIndex;
                    break;
                }
            }
            if(routputArray[rowIndex][miny]==i)
            {
                break;
            }
        }
        xindexvalue[a]=minx;
        a++;
        //Generate a square array of size 45 * 45
       for(int r=minx;r<=minx+45;r++)
        {
            for(int s=miny;s<=miny+45;s++)
            {
                squareArray[r][s]=1;
            }
        }
    }
    //XOR the square output and original output to get image with holes and protrusions
    for(int rowIndex=0;rowIndex<imageHeight;rowIndex++)
    {
        for(int colIndex=0;colIndex<imageWidth;colIndex++)
        {
            XORarray[rowIndex][colIndex]=((int)squareArray[rowIndex][colIndex])^((int)originalimage[rowIndex][colIndex]);
        }
    }
 
    //To get holes and protrusions for each object
    for(i=0;i<16;i++)
    {
        getobjectsandprotrusions(xindexvalue[i],yindexvalue[i],i,XORarray,objectholes);
    }
    
    //To count total number of unique objects--this takes into account rotations, flipping of the objects with respect to each other
    int c=generateVector(objectholes);
    cout<<"Total Number of unique objects in the image is: "<<endl;
    cout<<c<<endl;
    
    
    // Write image data (filename specified by second argument) from image data matrix
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }
    //fwrite(combinedArray, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fwrite(combinedArrayshrinking, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    //fwrite(combinedsqaureArray, sizeof(unsigned char), imageHeight*imageWidth*bytesPerPixel, file);
    fclose(file);
    return 0;
}







