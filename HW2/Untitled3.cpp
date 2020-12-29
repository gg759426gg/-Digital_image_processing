#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <windows.h>
#define M_PI (3.14159265359)
#include<time.h>

using namespace std;

//�������T
double gaussian_noise(double mean,double std) //����� �зǮt 
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
//------------------- 8 X 8  1-D DCT  ��B��}�l y ------------------------
    sum=0;
	    
    for(x=0;x<512;x++) 
    {
    	for(y=0;y<512;y++)
    	{
    		// �P�_ C(v) �� 
    		if(y==0||y%8==0)
    		{
    			cv=1/sqrt(2); //  1/�ڸ�2 
			}
			else
			{
				cv=1;
			}
			// ���� 8*8 
			if(y!=0&&y%8==0)
			{
				y1=y;
				tdy=y;            //�ܼ�tdy �x�sx���� 
			}
			else
			{
				y1=tdy;
			}
			if(y<=7)                //�p�Gy�p�󵥩�7�ɭȳ��k0 
			{
				y1=0;
			}
			// �֥[ 
			for(int k=0;k<8;k++)   //�Ψ��x�s�@��1D DCT�}�C������
			{
				v=y%8;
				y2=sum;
				cosy=cos((2*k+1)*v*M_PI/(double)16);       
				sum=girlfacematrix[x][y1]*cosy;           //�N����}�C�����Ƕ��ȭ��WCos
				sum=sum+y2;
				y1=y1+1;
			}
			sum=0.5*cv*sum;
			q[x][y]=sum;
			sum=0;
			
		}
   }

	//================8 X 8  1-D DCT  ��B�⵲��==================================

//------------------- 8 X 8  1-D DCT  �C�B��}�l x ------------------------ 
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
			// ���� 8*8 
			if(x!=0&&x%8==0)
			{
				x1=x;
				tdx=x; 
			}
			else
			{
				x1=tdx;               //�p�Gx��8�j�h����tdx�|�֥[1 ex: 8�B 9�B 10�B 11�B 12�B 13�B 14        
			}
			
			if(x<=7)                    //�p�Gx�p�󵥩�7�ɭȳ��k0 
			{
				x1=0;
			}
			// �֥[ 
			for(int k=0;k<8;k++)        //�Ψ��x�s�@��1D DCT�}�C������
			{   
				if((c==0)||(c%8==0))
				{
					temp[k]=q[x1][y];
				}
				w=x%8;                  //�C��x>8�ɱNx%8���l�ơA�o�O���F��8x8 1D DCT �ӷǳƪ�  �]���O�C8�Ӥ@��  �ҥH�C�@�����O�@��N=8 1D DCT  
				x2=sum;                   //�N�e�@sum�����x�s��x2
				cosx=cos((2*k+1)*w*M_PI/(double)16);     //�N����}�C�����Ƕ��ȭ��WCos
				sum=temp[k]*cosx;     
				sum=sum+x2;              //�N��X�Ӫ����G�@8�����֥[�۷��ƾǦ������֥[�Ÿ�
				x1=x1+1;                 //�C�����@��x1�|�֥[1 �N��O�ڭ̨��}�C�����Ƕ��ɩ��U�@�Ӳ��� 
			}
			sum=0.5*cu*sum;              //�b�֥[������  �����W�٭n�b���WC(u)���� 
			q[x][y]=sum;                  //�N�Ĥ@���B�⧹��1D DCT �s�^�}�C��
			sum=0;                        //�����@��8x8  sum�����k0  �H�K���U�@��8x8x�֥[��
			c=c+1;
  	
	    }
	}	
//================8 X 8  1-D DCT  �C�B�⵲��==================================

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
			// ���� 8*8 
			if(y!=0&&y%8==0)
			{
				y1=y;
				tdy=y;     //�ܼ�tdy �x�sy���� 
			}
			else
			{
				y1=tdy;
			}
			if(y<8)    
			{
				y1=0;
			}
			// �֥[ 
			for(int k=0;k<8;k++)
			{
				// �P�_ C(v) �� 
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
				v=y%8;               //�C��y>8�ɱNx%8���l�ơA�o�O���F��8x8 1D DCT �ӷǳƪ�  �]���O�C8�Ӥ@��  �ҥH�C�@�����O�@��N=8 1D DCT 
				cosy=cos((2*v+1)*k*M_PI/16);
				
				y2=sum;          //�N�e�@sum�����x�s��y2 
				m=sum_m;         //�N�e�@sum_m�����x�s��m
				wa=sum_w;        //�N�e�@sum_w�����x�s��wa
				
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
			// ���� 8*8 
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
			// �֥[ 
			for(int k=0;k<8;k++)
			{   
			    // �P�_ C(u) �� 
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
              img_new[i][j]=img[i][j]; //�N�쥻img�x�}�����x�s�bimgnew 
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
	
	
	
//-----------------�������TMSE PSNR------------------ 
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

  //�[�J�������T 
//    unsigned int Height = 0;
//	unsigned int Width = 0;
//	 
//	int i=0;
//	int j=0;
//	
//	BITMAPFILEHEADER bmpfileheader; //���
//	BITMAPINFOHEADER bmpinfoheader; //�H��
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
//	cout<<"��J�������T������:"<<endl;
//	double mean;
//	cin>>mean;
//	cout<<"��J�������T����t:"<<endl;
//	double std;
//	cin>>std;
//	cout<<"��J�������T���t��:"<<endl;
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
// 	printf("�ͦ��s�Ϥ�!\n");
 	
 	
 	
 	
 	

  //-----close
	fclose(input_file);
	fclose(output_file1);
	fclose(output_file2);
    //fclose(fpin);
	//fclose(fpout);
	return 0;
}
