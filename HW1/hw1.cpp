#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <fstream>

int gray(const char *fname_s, const char *fname_t){
 	typedef unsigned char   Byte;
	Byte pixel[512][512][3];
	Byte eipixel[512][512];
	Byte header[54];

 	FILE *fp1 , *fp2;
 	fp1 =fopen(fname_s,"rb");
 	fp2 =fopen(fname_t,"wb");
 	fread(header, 1, 54, fp1 );
 	fseek(fp1,54,SEEK_SET);
 	fread(pixel, 1,512*512*3, fp1);
    int width=512;
    int height=512;
	int i,j; 

     Byte palette[256][4] ;
      long file_size = height*width + 1078;
      long data_size = height*width ;
      long data_offset=1078;
      long used_color=256;
      unsigned int  x,y;

      for(x=0;x<256;x++){            //調色盤資訊 
        for(y=0;y<4;y++){
            palette[x][y]=x;
            palette[x][3]=0;
        }
      }

      for(i=0; i<height; i++){
            for(j=0; j<width; j++){
                    eipixel[j][i]=pixel[j][i][0]*0.114+pixel[j][i][1]*0.587+pixel[j][i][2]*0.299;
            }
      }

	header[2] =  file_size& 0x000000ff ;              //file size紀錄 
    header[3] = (file_size >> 8)& 0x000000ff ;
    header[4] = (file_size >> 16)& 0x000000ff ;
	header[10] =  data_offset& 0x000000ff;            //data offset紀錄 
    header[11] = (data_offset >> 8)& 0x000000ff ;
    header[28]=8;									  //bit
    header[46]=  used_color& 0x000000ff;			  //used_color
    header[47]=  (used_color  >> 8)& 0x000000ff;

    fwrite(header, 1, 54, fp2);
 	fwrite(palette, 1, 1024, fp2);
 	fwrite(eipixel, 1, height*width, fp2);

 	fclose(fp1);
    fclose(fp2);

    return 0;
}

int negative (const char *fname_s, const char *fname_t){
	
	FILE *fp1 = fopen(fname_s,"rb");
    FILE *fp2 = fopen(fname_t,"wb");
    unsigned char header[54];
    unsigned char palette[256][4];
	unsigned char pixel[512][512];
	int width=512;
    int height=512;
	fseek(fp1,0,SEEK_SET);
	fread(&header, sizeof(unsigned char),54,fp1);
	fseek(fp1,54,SEEK_SET);
	fread(&palette,1,1024,fp1);    
 	fseek(fp1,1078,SEEK_SET);
 	fread(pixel, sizeof(unsigned char),height*width, fp1);
    unsigned char a[512][512];
	unsigned int i,j;
    
		for( i = 0 ; i < height ; i++){
			for( j = 0 ; j < width ; j++){
					a[j][i] = 255-pixel[j][i] ; 
			}
		}
	
    
    fwrite(&header, sizeof(unsigned char), 54, fp2);
    fwrite(&palette,1,1024, fp2);
 	fwrite(a, sizeof(unsigned char), height*width, fp2);
	
	fclose(fp1);
	fclose(fp2);
	
	return 0;
	}

int main() {
	FILE *fp;
    unsigned char  Identifier,Identifier1; //Announce ID short 2 byte 
    unsigned int   File_Size;              //Announce File_Size int 4 byte           
    unsigned int   Reserved;               //Announce Reserved  int 4 byte
    unsigned int   Bitmap_Data_Offset;     // 點陣圖資料開始前的偏移量 
    unsigned int   Bitmap_Header_Size;     //bitmap info header的長度 
    unsigned int   width, height;          //寬.高 
    unsigned short Planes;				   //點陣圖的位元圖層數 
    unsigned short Bits_Per_Pixel;		  
    unsigned int   Compression;			   //壓縮方式(沒壓縮) 
    unsigned int   Bitmap_Data_Size;       //512*512
    unsigned int   H_Resolution;
    unsigned int   V_Resolution;
    unsigned int   Used_Colors;			   //調色盤數量 
    unsigned int   Important_Colors;	   // 
    //=======Header variable // 54byte===========
	fp        = fopen("lena.bmp","rb");     //open the bmp file 
	fseek(fp, 0, SEEK_SET);                                       
	fread(&Identifier,sizeof(unsigned char),1,fp);				//1為元素的數目 
	fread(&Identifier1,sizeof(unsigned char),1,fp);
    fread(&File_Size,sizeof(unsigned int), 1, fp);
    fread(&Reserved,sizeof(unsigned int), 1, fp);
    fread(&Bitmap_Data_Offset,sizeof(unsigned int), 1, fp);
   	fseek(fp, 14, SEEK_SET);                                       
    fread(&Bitmap_Header_Size,sizeof(unsigned int), 1, fp);
    fread(&width,  sizeof(unsigned int), 1, fp);
	fread(&height, sizeof(unsigned int), 1, fp);
    fread(&Planes, sizeof(unsigned short), 1, fp);
    fread(&Bits_Per_Pixel,sizeof(unsigned short), 1, fp);
    fread(&Compression,sizeof(unsigned int), 1, fp);
    fread(&Bitmap_Data_Size,sizeof(unsigned int), 1, fp);
    fread(&H_Resolution,sizeof(unsigned int), 1, fp);
    fread(&V_Resolution,sizeof(unsigned int), 1, fp);
    fread(&Used_Colors,sizeof(unsigned int), 1, fp);
    fread(&Important_Colors,sizeof(unsigned int), 1, fp);
	 
   	printf("==========Bitmap File Header==========\n\n",Identifier,Identifier1);
	printf("Identifie  %c%c\n",Identifier,Identifier1);
	printf("File_Size : %d bytes\n",File_Size);
	printf("Reserved  : %d \n",Reserved);
    printf("Bitmap_Data_Offset: %d \n\n",Bitmap_Data_Offset);
    printf("==========Bitmap Info Header==========\n\n",Identifier,Identifier1);
    printf("Bitmap_Header_Size: %d \n",Bitmap_Header_Size);
    printf("width: %d pixel\n",width);
    printf("height: %d pixel\n",height);
    printf("Planes: %d \n",Planes);
    printf("Bits_Per_Pixel: %d \n",Bits_Per_Pixel);
    printf("Compression: %d \n",Compression);
    printf("Bitmap_Data_Size: %d \n",Bitmap_Data_Size);
    printf("H_Resolution: %d \n",H_Resolution);
    printf("V_Resolution: %d \n",V_Resolution);
    printf("Used_Colors: %d \n",Used_Colors);
    printf("Important_Colors: %d \n",Important_Colors); 
 	fclose(fp);
 		
	gray("lena.bmp","lena_8bit.bmp");
 	negative( "lena_8bit.bmp" ,"lenachange.bmp"  );
 	
 	FILE *fp1;
	FILE *histogram;
	unsigned int  graylevel[256];
	unsigned char pixelarray[262144];
	unsigned int i,j;
	fp1       = fopen("lenachange.bmp","rb");
	histogram = fopen("histogram.csv","w");
	fseek( fp , 1078 , SEEK_SET );
	
	for(i=0;i<=262143;i++){
    fread(&pixelarray[i],sizeof(unsigned char), 1, fp);
	}
	
	for(i=0;i<=262143;i++){
   		for(j=0;j<=255;j++){
   			if(pixelarray[i]==j){          //陣列的內容作整理將裡面的gray-level整理、累加起來，得到我們256個gray-level的各個值 
   				graylevel[j] = graylevel[j] +1;
		   	}
		}
	}
	for(int i = 0; i <=255; i++){           //直方圖 
	fprintf(histogram, "%d \n", graylevel[i]);
 	}
   fclose(fp1);
   fclose(histogram);
 return 0;
 }
