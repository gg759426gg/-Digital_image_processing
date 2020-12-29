#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <windows.h>
#define M_PI (3.14159265359)
#include<time.h>

using namespace std;

//高斯雜訊
double gaussian_noise(double mean,double std) //期望值 標準差 
{	
	static double z1, z2, S;
    static int phase = 0;
    double g;
    double u1,u2;
    if ( phase == 0 ) 
	{
        do 
		{
            u1 = (double)rand() / RAND_MAX; //RAND_MAX=32767
            u2 = (double)rand() / RAND_MAX;

            z1 = 2 * u1 - 1;
            z2 = 2 * u2 - 1;
            S = z1 * z1 + z2 * z2;
        } 
		while(S >= 1 || S == 0);

        g = z1 * sqrt(-2 * log(S) / S);
    } 
	else
	{
        g = z2 * sqrt(-2 * log(S) / S);
    }
    phase = 1 - phase;
    return mean+std*g;
}






unsigned char img[512][512]={0};
char girlfacematrix[512][512]={0};
double temp[8]={0};
double mp[8]={0};
double wp[8]={0};
float q[512][512]={0};
float e[512][512]={0};
char DCT_img[512][512]={0};
float mask_img[512][512]={0};


int main(){
	
    FILE *input_file;
	FILE *output_file1 ;
	FILE *output_file2 ;
	FILE *fpm1;
 	 
	input_file = fopen("girlface.bmp", "rb");
	output_file1 = fopen("girlfaceDCT.bmp", "wb");
	output_file2  = fopen("girlfaceDCT_IDCT.bmp", "wb");
    fpm1 = fopen("gaussian_noise.bmp", "wb");

	char girlface[1078];
	fread(&girlface,1,1078,input_file);
	
	int mask[8][8]={ {1,1,1,0,0,0,0,0}, 
                     {1,1,0,0,0,0,0,0}, 
                     {1,0,0,0,0,0,0,0}, 
                     {0,0,0,0,0,0,0,0}, 
                     {0,0,0,0,0,0,0,0}, 
                     {0,0,0,0,0,0,0,0}, 
                     {0,0,0,0,0,0,0,0}, 
                     {0,0,0,0,0,0,0,0} };


	
	for(int i=0;i<512;i++)
	{
		for(int j=0;j<512;j++)
		{
			fread(&img[i][j], 1, 1, input_file);
			girlfacematrix[i][j]=img[i][j]-128; 
		}
    }
 
//------------------ DCT ----------------------- 
    int y,y1,tdy;
    int x,x1,tdx;
    float cu,cv;
    int v,w,c=0;
    double cosy,y2,sum;
    double cosx,x2;
//------------------- 8 X 8  1-D DCT  行運算開始 y ------------------------
    sum=0;
	    
    for(x=0;x<512;x++) 
    {
    	for(y=0;y<512;y++)
    	{
    		// 判斷 C(v) 值 
    		if(y==0||y%8==0)
    		{
    			cv=1/sqrt(2); //  1/根號2 
			}
			else
			{
				cv=1;
			}
			// 切割 8*8 
			if(y!=0&&y%8==0)
			{
				y1=y;
				tdy=y;            //變數tdy 儲存x的值 
			}
			else
			{
				y1=tdy;
			}
			if(y<=7)                //如果y小於等於7時值都歸0 
			{
				y1=0;
			}
			// 累加 
			for(int k=0;k<8;k++)   //用來儲存作完1D DCT陣列中的值
			{
				v=y%8;
				y2=sum;
				cosy=cos((2*k+1)*v*M_PI/(double)16);       
				sum=girlfacematrix[x][y1]*cosy;           //將原先陣列中的灰階值乘上Cos
				sum=sum+y2;
				y1=y1+1;
			}
			sum=0.5*cv*sum;
			q[x][y]=sum;
			sum=0;
			
		}
   }

	//================8 X 8  1-D DCT  行運算結束==================================

//------------------- 8 X 8  1-D DCT  列運算開始 x ------------------------ 
    sum=0;
    
    for(y=0;y<512;y++)
    {
    	for(x=0;x<512;x++)
    	{
    		if(x==0||x%8==0)
    		{
    			cu=1/sqrt(2);
			}
			else
			{
				cu=1;
			}
			// 切割 8*8 
			if(x!=0&&x%8==0)
			{
				x1=x;
				tdx=x; 
			}
			else
			{
				x1=tdx;               //如果x比8大則後續的tdx會累加1 ex: 8、 9、 10、 11、 12、 13、 14        
			}
			
			if(x<=7)                    //如果x小於等於7時值都歸0 
			{
				x1=0;
			}
			// 累加 
			for(int k=0;k<8;k++)        //用來儲存作完1D DCT陣列中的值
			{   
				if((c==0)||(c%8==0))
				{
					temp[k]=q[x1][y];
				}
				w=x%8;                  //每當x>8時將x%8取餘數，這是為了做8x8 1D DCT 而準備的  因為是每8個一組  所以每一次都是一個N=8 1D DCT  
				x2=sum;                   //將前一sum的值儲存到x2
				cosx=cos((2*k+1)*w*M_PI/(double)16);     //將原先陣列中的灰階值乘上Cos
				sum=temp[k]*cosx;     
				sum=sum+x2;              //將算出來的結果作8次的累加相當於數學式中的累加符號
				x1=x1+1;                 //每做完一次x1會累加1 意思是我們取陣列中的灰階時往下一個移動 
			}
			sum=0.5*cu*sum;              //在累加完畢後  公式上還要在乘上C(u)的值 
			q[x][y]=sum;                  //將第一次運算完的1D DCT 存回陣列中
			sum=0;                        //做完一次8x8  sum的值歸0  以便做下一次8x8x累加值
			c=c+1;
  	
	    }
	}	
//================8 X 8  1-D DCT  列運算結束==================================

//-------------------WRITE-----------------------
    fwrite(&girlface,1,1078,output_file1);
//   

    fseek(output_file1,1078,SEEK_SET);
//   

	

    // -dct
	for(int i=0;i<512;i++)
	{
		for(int j=0;j<512;j++)
		{
			DCT_img[i][j]= (char)q[i][j];
			fwrite(&DCT_img[i][j], 1, 1, output_file1); 
			
			e[i][j]=DCT_img[i][j];
		}
    }
    //------------------check
    printf("idctX:\n");
    for(int w=0;w<16;w++)
    {
    	for(int z=0;z<16;z++)
    	{
    		printf("%.1f   ",e[w][z]);
		}
		printf("\n");
	}
	
	printf("\n"); 
//--------------------
 // mask
// FILE* pfm1 = fopen("girlface_DCTmask.bmp", "wb");
//    for(int i=0;i<512;i++)
//	{
//		for(int j=0;j<512;j++)
//		{
//			mask_img[i][j]= q[i][j]*mask[i%8][j%8];
//			fwrite(&mask_img[i][j], 1, 1, pfm1); 
//	    }
//    } 


//------------------ IDCT -y -------------------- 
	sum=0;
	c=0;
	double sum_m=0, m=0;
	double sum_w=0, wa=0;
	
    for(x=0;x<512;x++) 
    {
    	for(y=0;y<512;y++)
    	{
			// 切割 8*8 
			if(y!=0&&y%8==0)
			{
				y1=y;
				tdy=y;     //變數tdy 儲存y的值 
			}
			else
			{
				y1=tdy;
			}
			if(y<8)    
			{
				y1=0;
			}
			// 累加 
			for(int k=0;k<8;k++)
			{
				// 判斷 C(v) 值 
				if(y1==0||y1%8==0)  
				{
					cv=1/sqrt(2);
				}
				else
				{
					cv=1;
				}
				if((c==0)||(c%8==0))
				{
					temp[k]=q[x][y1];

				}
				v=y%8;               //每當y>8時將x%8取餘數，這是為了做8x8 1D DCT 而準備的  因為是每8個一組  所以每一次都是一個N=8 1D DCT 
				cosy=cos((2*v+1)*k*M_PI/16);
				
				y2=sum;          //將前一sum的值儲存到y2 
				m=sum_m;         //將前一sum_m的值儲存到m
				wa=sum_w;        //將前一sum_w的值儲存到wa
				
				sum=cv*temp[k]*cosy;  
				sum_m=cv*mp[k]*cosy;
				sum_w=cv*wp[k]*cosy;
				
				sum=sum+y2;
				sum_m=sum_m+m;
				sum_w=sum_w+wa;
				
				y1=y1+1;
			}
			q[x][y]=0.5*sum;      

			sum=0;
			sum_m=0;
			sum_w=0;
			
			c=c+1;
		}
   }

//------------------ IDCT -x -------------------- 
    sum=0;
    sum_m=0;
    sum_w=0;
    c=0;
    
    for(y=0;y<512;y++)
    {
    	for(x=0;x<512;x++)
    	{
			// 切割 8*8 
			if(x!=0&&x%8==0)
			{
				x1=x;
				tdx=x;
			}
			else
			{
				x1=tdx;
			}
			
			if(x<8)
			{
				x1=0;
			}
			// 累加 
			for(int k=0;k<8;k++)
			{   
			    // 判斷 C(u) 值 
				if(x1==0||x1%8==0)
				{
					cu=1/sqrt(2);
				}
				else
				{
					cu=1;
				}
				if((c==0)||(c%8==0))
				{
					temp[k]=q[x1][y];

				}
				
				w=x%8;
				cosx=cos((2*w+1)*k*M_PI/16);
				
				x2=sum;
				m=sum_m;
				wa=sum_w;
				
				sum=cu*temp[k]*cosx;
				sum_m=cu*mp[k]*cosx;
				sum_w=cu*wp[k]*cosx;
				
				sum=sum+x2;
				sum_m=sum_m+m;
				sum_w=sum_w+wa;
				
				x1=x1+1;
			}
			q[x][y]=0.5*sum;

			
			sum=0;
			sum_m=0;
			sum_w=0;
			
			c=c+1;
  	
	    }
	}
	
//-------------------WRITE-----------------------

    fwrite(&girlface,1,1078,output_file2);
    fseek(output_file2,1078,SEEK_SET);

    

    // idct
	for(int i=0;i<512;i++)
	{
		for(int j=0;j<512;j++)
		{
			DCT_img[i][j]= (char)q[i][j]+128;
			fwrite(&DCT_img[i][j], 1, 1, output_file2); 
		}
    }
    
    //-------------------noise------------------------
 	float img_new[512][512]={0};
        for(int i=0;i<512;i++) {
           for(int j=0;j<512;j++){
              img_new[i][j]=img[i][j]; //將原本img矩陣的值儲存在imgnew 
            }
         }

    for( x=0;x<512;x++) 
    {    	
	    for( y=0;y<512;y++)
    	{

    		img_new[x][y]=img_new[x][y]+(0.25*gaussian_noise(0,16));
		}
	}



    fwrite(&girlface,1,1078, fpm1);

    
    for(int i=0;i<512;i++)
	{
		for(int j=0;j<512;j++)
		{	
			if(img_new[i][j]>255)
			{
                img_new[i][j]=255;
			}
			if(img_new[i][j]<0)
			{
			    img_new[i][j]=0;
			}
			DCT_img[i][j]= (unsigned char)img_new[i][j];
			fwrite(&DCT_img[i][j], 1, 1, fpm1); 
			
		}
    }
    
    
    
    //mask
//    FILE* pfm2 = fopen("girl_IDCTmask.bmp", "wb");
//    for(int i=0;i<512;i++)
//	{
//		for(int j=0;j<512;j++)
//		{
//			DCT_img[i][j]= mask_img[i][j]+128;
//			fwrite(&DCT_img[i][j], 1, 1, pfm2); 
//		}
//    }





//----------------------MSE & PSNR
    unsigned char mse[512][512]={0};
    double MSE=0 , PSNR=0;
    
    for(int i=0;i<512;i++)
	{
		for(int j=0;j<512;j++)
		{
			img[i][j]=img[i][j]-DCT_img[i][j]; 
			mse[i][j]=img[i][j]*img[i][j];
			
			MSE=MSE+mse[i][j];
		}
    }
    MSE=MSE/(512*512);
    
    PSNR=10*(log10(255*255/MSE));
	
//-----------------
	printf("\n");
	
	printf("mse:\n");
	printf("%.1f ",MSE);	
	printf("\n");
	
	printf("\n");
	printf("PSNR:\n");
	printf("%.1f ",PSNR);	
	printf("\n");
	
	
	
//-----------------高斯雜訊MSE PSNR------------------ 
    unsigned char mse_noise[512][512]={0};
    double MSE_noise=0 , PSNR_noise=0;
    
    for(int i=0;i<512;i++)
	{
		for(int j=0;j<512;j++)
		{
			img_new[i][j]=img_new[i][j]-DCT_img[i][j]; 
			mse_noise[i][j]=img_new[i][j]*img_new[i][j];
			
			MSE_noise=MSE_noise+mse_noise[i][j];
		}
    }
    MSE_noise=MSE_noise/(512*512);
    
    PSNR_noise=10*(log10(255*255/MSE_noise));
    printf("\n");
	
	printf("mse_noise:\n");
	printf("%.1f ",MSE_noise);	
	printf("\n");
	
	printf("\n");
	printf("PSNR_noise:\n");
	printf("%.1f ",PSNR_noise);	
	printf("\n");
	
	

//------------------check
    cout<<"idctX:\n";
    for(int w=0;w<16;w++)
    {
    	for(int z=0;z<16;z++)
    	{
    		printf("%.1f   ",(float)girlfacematrix[w][z]);
		}
		printf("\n");
	}
	
	cout<<"\n";

  //加入高斯雜訊 
//    unsigned int Height = 0;
//	unsigned int Width = 0;
//	 
//	int i=0;
//	int j=0;
//	
//	BITMAPFILEHEADER bmpfileheader; //文件
//	BITMAPINFOHEADER bmpinfoheader; //信息
//	
//	typedef struct tagRGB 
//	{
//		BYTE blue;
//		BYTE green;
//		BYTE red;
//	}RGBDATA; 
//	
//	FILE *fpin; 
//	FILE *fpout; 
//	
//	fpin=fopen("girlface.bmp","rb");
//	fread(&bmpfileheader,sizeof(BITMAPFILEHEADER),1,fpin);
//	fread(&bmpinfoheader,sizeof(BITMAPINFOHEADER),1,fpin);
//	Height=bmpinfoheader.biHeight;
//	Width=bmpinfoheader.biWidth;
//	
//
//	RGBDATA** RGBin; 
//	RGBin = (RGBDATA **)malloc(sizeof(RGBDATA*) * Height);
//	for (i = 0; i < Height; i++) 
//	{
//		RGBin[i] = (RGBDATA *)malloc(sizeof(RGBDATA) * Width);
//	}
//	
//	
//	for(i=0;i<Height;i++)
//	{
//		fread(RGBin[i], sizeof(tagRGB), Width, fpin);
//	}
//	
//	cout<<"輸入高斯雜訊的均值:"<<endl;
//	double mean;
//	cin>>mean;
//	cout<<"輸入高斯雜訊的方差:"<<endl;
//	double std;
//	cin>>std;
//	cout<<"輸入高斯雜訊的系數:"<<endl;
//	double k;
//	cin>>k;
//	
//	srand((unsigned) (time(NULL)));
//	
//	
//	for(i=0;i<Height;i++)
//	{
//		for(j=0;j<Width;j++)
//		{
//			RGBin[j][i].blue+=(k*gaussian_noise(mean,std));
//			RGBin[j][i].green+=(k*gaussian_noise(mean,std));
//			RGBin[j][i].red+=(k*gaussian_noise(mean,std));
//		}
//	}
//	
//	
//	fpout=fopen("gaussian_noise.bmp","wb");
//	fwrite(&bmpfileheader,sizeof(BITMAPFILEHEADER),1,fpout);
//	fwrite(&bmpinfoheader,sizeof(BITMAPINFOHEADER),1,fpout);
//	for (i=0;i<Height;i++)
//	{
//		fwrite(RGBin[i],sizeof(tagRGB),Width,fpout);
//	}
// 	printf("生成新圖片!\n");
 	
 	
 	
 	
 	

  //-----close
	fclose(input_file);
	fclose(output_file1);
	fclose(output_file2);
    //fclose(fpin);
	//fclose(fpout);
	return 0;
}
