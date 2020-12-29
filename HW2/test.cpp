#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cmath>
using namespace std;

#define M_PI (3.14159265359)

int main() {

	FILE *input_file;
	FILE *output_file20;
	FILE *output_file21;
	FILE *output_file22;
	FILE *output_file23;
	FILE *output_file24;
	FILE *output_file25;
	FILE *output_file26;
	FILE *detection;

	input_file = fopen("girlface.bmp", "rb");
	output_file20 = fopen("signature.bmp", "rb");
	output_file21 = fopen("girlfaceDCT.bmp", "wb");
	output_file22 = fopen("girlfaceDCT_IDCT.bmp", "wb");
	output_file23 = fopen("girlfaceDCT_Mask.bmp", "wb");
	output_file24 = fopen("girlfaceDCT_Mask_IDCT.bmp", "wb");
	output_file25 = fopen("couplewater.bmp", "wb");
	output_file26 = fopen("coupleoutput_file26mark.bmp", "wb");





	int mask[8][8] = { { 1, 1 ,1 ,0 ,0 ,0, 0 ,0 },      //HW2-4 
						{ 1, 1, 0 ,0 ,0, 0, 0, 0 },
						{ 1 ,0, 0, 0, 0, 0, 0, 0 },
						{ 0 ,0, 0, 0, 0, 0, 0, 0 },
						{ 0, 0, 0, 0, 0, 0, 0, 0 },
						{ 0 ,0, 0, 0, 0, 0, 0, 0 },
						{ 0, 0, 0, 0, 0, 0, 0, 0 },
						{ 0, 0, 0, 0, 0, 0, 0, 0 } }; 


	//start DCT
	int y, x;
	int count_ = 0;
	double abc[8] = { 0 };
	const int sizex = 512;
	const int sizey = 512;
	unsigned char temp[512 * 512] = { 0 };
	unsigned char temp1[512 * 512] = { 0 };
	unsigned char temp2[32 * 32] = { 0 };


	//分配二維空間 

	float **td_girlfacematrix = (float **)malloc(sizex * sizeof(void *));
	float **fd_girlfacematrix = (float **)malloc(sizex * sizeof(void *));
	float **q = (float **)malloc(sizex * sizeof(void *));


	float *iat = (float *)malloc(sizex * sizey * sizeof(float *));
	float *iaf = (float *)malloc(sizex * sizey * sizeof(float *));
	float *iaq = (float *)malloc(sizex * sizey * sizeof(float *));
	

	for (y = 0; y != sizey; ++y, iat += sizey, iaf += sizey, iaq += sizey) {
		td_girlfacematrix[y] = iat;
		fd_girlfacematrix[y] = iaf;

		q[y] = iaq;
	}
	for (y = 0; y != sizey; ++y) {
		for (x = 0; x != sizex; ++x) {
			td_girlfacematrix[x][y] = y + x;
			fd_girlfacematrix[x][y] = y + x;
	
			q[x][y] = y + x;
		}
	}
	for (y = 0; y != 32; ++y) {
	
	}
	for (y = 0; y != 32; ++y) {
		for (x = 0; x != 32; ++x) {
	
		}
	}

	int length = 8;
	int BmpWidth = 512, BmpHeight = 512;

	fseek(input_file, 1078L, SEEK_SET);   //跳至第1078個byte 開始讀取灰階值 
	fread(&temp, 1, BmpWidth*BmpHeight, input_file);


	for (y = 0; y<512; y++) {
		for (x = 0; x<512; x++) {
			td_girlfacematrix[x][y] = temp[y * 512 + x] - 128;
			q[x][y] = 0;
			//cout<<"td_lenamatrix= "<<td_lenamatrix[x][y]<<" x= "<<x<<" y= "<<y<<endl;
		}
	}
	printf("原始灰階");
	for (x = 0; x<16; x++) {
		for (y = 0; y<16; y++) {
			printf("%.1f  ", td_girlfacematrix[x][y]);
		}
		printf("\n");
	}

	//================8 X 8  1-D DCT  列運算開始,對於圖片而言列是y軸 行是x軸==================================
	float alfa, fre_tempmatrix[512][512] = { 0 };
	int i;
	double sum = 0;
	for (y = 0; y<BmpHeight; y++) {
		int tdx = 0, tmp = 0;
		for (x = 0; x<BmpWidth; x++) {
			//C(u)
			if (x == 0 || x % 8 == 0) {
				alfa = 0.70710678118654752440084436210485; //  1  / 根號2  
			}
			else {
				alfa = 1;
			}
			if (x % 8 == 0 && x != 0) {
				tdx = x;	                     //變數tdx(time domain的x)  儲存x的值 
				tmp = x;                        //變數tmp 儲存x的值     
			}
			else {
				tdx = tmp;                    //概念:如果x比8大則後續的tdx會累加1 ex: 8、 9、 10、 11、 12、 13、 14、 15 共8個一組  
			}
			if (x <= 7) {                       //如果x小於等於7時值都歸0 
				tdx = 0;
			}
			for (i = 0; i<length; i++) {
				int cc = 0;                   //宣告int儲存區域變數rr,cc
				double rr = 0;
				double pp = 0;               //宣告double儲存區域變數pp,oo  
				rr = td_girlfacematrix[tdx][y];     //將陣列中的灰階值儲存到rr中 ,tdx為控制變數是原先的x值 但是要8個一組(也就是說有8次運算),故要對x的值做控制,y為控制變數 如果使用x值 會做不滿512 
				pp = sum;                       //將前一sum的值儲存到pp                
				cc = x % 8;                       //每當x>8時將x%8取餘數，這是為了做8x8 1D DCT 而準備的  因為是每8個一組  所以每一次都是一個N=8 1D DCT 
												  //cos內容值運算式,原先範例給的參數做出來的結果不盡理想  
				sum = rr*cos((2 * i + 1)* cc* M_PI / (double)16);                //將原先陣列中的灰階值乘上Cos   注意:原先Cos係數因該是要乘上PI/180 才會是我們計算中Cos的值 但是此處似乎不用這樣做 
																				 //printf("sum:%.2f\n",sum);
				sum = sum + pp;                 //將算出來的結果作8次的累加相當於數學式中的累加符號 
				tdx = tdx + 1;                       //每做完一次tdx會累加1 意思是我們取陣列中的灰階時往下一個移動 
			}
			sum = sum*0.5*alfa;                  		 //在累加完畢後  公式上還要在乘上C(u)的值 
			fre_tempmatrix[y][x] = sum;                   //將第一次運算完的1D DCT 存回陣列中 
														  //printf("fre_tempmatrix:%.2f\n",fre_tempmatrix[x][y]);
			sum = 0;                        			//做完一次8x8  sum的值歸0  以便做下一次8x8x累加值 
		}
	}
	//================8 X 8  1-D DCT  列運算結束==================================
	//================8 X 8  1-D DCT  行運算開始==================================
	for (x = 0; x<BmpWidth; x++) {
		int tdy = 0, tmp = 0;
		for (y = 0; y<BmpHeight; y++) {
			if (y == 0 || y % 8 == 0) {
				alfa = 0.70710678118654752440084436210485; //  1  / 根號2  
			}
			else {
				alfa = 1;
			}
			if (y % 8 == 0 && y != 0) {
				tdy = y;	                     //變數tdx(time domain的x)  儲存x的值 
				tmp = y;                        //變數tmp 儲存x的值     
			}
			else {
				tdy = tmp;                    //概念:如果x比8大則後續的tdx會累加1 ex: 8、 9、 10、 11、 12、 13、 14、 15 共8個一組  
			}
			if (y <= 7) {                       //如果x小於等於7時值都歸0 
				tdy = 0;
			}
			for (i = 0; i<length; i++) {
				//用來儲存作完1D DCT陣列中的值   因為有小數用dobule 
				double pp = 0;
				int cc = 0;
				pp = sum;
				cc = y % 8;
				//printf("oo:%.2f\n",oo);
				//printf("rr:%.2f \n ", abc[k]);
				//sum =abc[i]*cos((2*i+1)*cc* M_PI/(double)16);
				sum = fre_tempmatrix[tdy][x] * cos((2 * i + 1)*cc* M_PI / (double)16);
				sum = sum + pp;
				tdy = tdy + 1;
				//printf("sum:%.2f\n",sum);
			}
			sum = sum *0.5*alfa;    //此處因為做完的圖片結果不盡理想 故控制一個額外的參數  將原先的DCT反相 
			fd_girlfacematrix[y][x] = sum;
			sum = 0;
			count_ = count_ + 1;
		}
	}

	printf("再度開始檢測運算值是否正確");
	for (y = 0; y<16; y++) {
		for (x = 0; x<16; x++) {
			printf("%.1f  ", fd_girlfacematrix[y][x]);
		}
		printf("\n");
	}
	//================8 X 8  1-D DCT  行運算結束==================================
	//DCT之頻域圖  HW2-1
	float t;
	for (y = 0; y<512; y++) {
		for (x = 0; x<512; x++) {
			t = fd_girlfacematrix[y][x];
			if (t<0.0) {
				q[x][y] = 0;
			}
			else if (t>255.0) {
				q[x][y] = 255;
			}
			else {
				q[x][y] = t;
			}
			temp1[y * 512 + x] = round(fd_girlfacematrix[y][x]);
			//printf("%d  ", temp1[y*512+x]);
			fwrite(&temp1[y * 512 + x], sizeof(unsigned char), 1, output_file21);
		}
	}

	//經過filter mask HW2-3
	//float q[512][512]= {0};
	printf("檢測mask運算值是否正確");
	printf("\n ");
	for (y = 0; y<512; y++) {
		for (x = 0; x<512; x++) {
			q[y][x] = (double)fd_girlfacematrix[y][x] * (double)mask[x % 8][y % 8];
			//printf("%d  ", mask[x%8][y%8]);
			//printf("%.1f  ", q[y][x]);
			//fre_tempmatrix[x][y] = q[y][x];
			//	 		if(q[y][x]<0){
			//	 			q[y][x]=0;
			//			}
			//			else if(q[y][x]>255.0){
			//				q[y][x]=255;
			//			}
			//			else{
			//				q[y][x]=q[y][x];
			//			}
			//printf("%.1f  ", fre_tempmatrix);
			fwrite(&q[y][x], sizeof(unsigned char), 1, output_file23);
		}
		//printf("\n ");
	}
	//PSNR HW2-2
	//================8 X 8  1-D IDCT  列運算開始================================== 
	double MSE = 0, PSNR = 0;
	for (y = 0; y<BmpHeight; y++) {
		int tdx = 0, tmp = 0;
		for (x = 0; x<BmpWidth; x++) {
			if (x % 8 == 0 && x != 0) {
				tdx = x;	                     //變數tdx(time domain的x)  儲存x的值 
				tmp = x;                        //變數tmp 儲存x的值     
			}
			else {
				tdx = tmp;                    //概念:如果x比8大則後續的tdx會累加1 ex: 8、 9、 10、 11、 12、 13、 14、 15 共8個一組  
			}
			if (x <= 7) {                       //如果x小於等於7時值都歸0 
				tdx = 0;
			}
			for (i = 0; i<length; i++) {
				if (tdx == 0 || tdx % 8 == 0) {
					alfa = 0.70710678118654752440084436210485; //  1  / 根號2  
				}
				else {
					alfa = 1;
				}
				int cc = 0;                   //宣告int儲存區域變數rr,cc
				double rr = 0;
				double pp = 0;               //宣告double儲存區域變數pp,oo  
				rr = (double)fd_girlfacematrix[y][tdx] * (double)mask[y % 8][tdx % 8];     //將陣列中的灰階值儲存到rr中 ,tdx為控制變數是原先的x值 但是要8個一組(也就是說有8次運算),故要對x的值做控制,y為控制變數 如果使用x值 會做不滿512
												 //printf("q:%.2f\n",rr); 
				pp = sum;                       //將前一sum的值儲存到pp                
				cc = x % 8;                       //每當x>8時將x%8取餘數，這是為了做8x8 1D DCT 而準備的  因為是每8個一組  所以每一次都是一個N=8 1D DCT 
												  //cos內容值運算式,原先範例給的參數做出來的結果不盡理想  
				sum = rr*cos((2 * cc + 1)* i* M_PI / (double)16)*alfa;                //將原先陣列中的灰階值乘上Cos   注意:原先Cos係數因該是要乘上PI/180 才會是我們計算中Cos的值 但是此處似乎不用這樣做 
				sum = sum + pp;                 //將算出來的結果作8次的累加相當於數學式中的累加符號 
				tdx = tdx + 1;                       //每做完一次tdx會累加1 意思是我們取陣列中的灰階時往下一個移動 
			}
			sum = sum*0.5;                  		 //在累加完畢後  公式上還要在乘上C(u)的值 
			fre_tempmatrix[y][x] = sum;                   //將第一次運算完的1D DCT 存回陣列中 
														  //printf("fre_tempmatrix:%.2f\n",fre_tempmatrix[x][y]);
			sum = 0;                        			//做完一次8x8  sum的值歸0  以便做下一次8x8x累加值 
		}
	}
	//================8 X 8  1-D IDCT  列運算結束==================================
	count_ = 0;
	//================8 X 8  1-D IDCT  行運算開始==================================
	for (x = 0; x<BmpWidth; x++) {
		int tdy = 0, tmp = 0;
		for (y = 0; y<BmpHeight; y++) {
			if (y % 8 == 0 && y != 0) {
				tdy = y;	                     //變數tdy(time domain的y)  儲存x的值 
				tmp = y;                        //變數tmp 儲存x的值     
			}
			else {
				tdy = tmp;                    //概念:如果x比8大則後續的tdx會累加1 ex: 8、 9、 10、 11、 12、 13、 14、 15 共8個一組  
			}
			if (y <= 7) {                       //如果x小於等於7時值都歸0 
				tdy = 0;
			}
			for (i = 0; i<length; i++) {
				if (tdy == 0 || tdy % 8 == 0) {
					alfa = 0.70710678118654752440084436210485; //  1  / 根號2  
				}
				else {
					alfa = 1;
				}
				//用來儲存作完1D DCT陣列中的值   因為有小數用dobule 
				if ((count_ == 0) || (count_ % 8 == 0)) {
					abc[i] = fre_tempmatrix[tdy][x];     //將做完1D- DCT陣列中的值給予rr       
				}
				double pp = 0;
				int cc = 0;
				pp = sum;
				cc = y % 8;
				//printf("oo:%.2f\n",oo);
				//printf("rr:%.2f \n ", abc[k]);
				sum = abc[i] * cos((2 * cc + 1)*i* M_PI / (double)16)*alfa;
				//sum =fre_tempmatrix[tdy][x]*cos((2*cc+1)*i* M_PI/(double)16)*alfa;
				sum = sum + pp;
				tdy = tdy + 1;
				//printf("sum:%.2f\n",sum);
			}
			sum = sum *0.5;    //此處因為做完的圖片結果不盡理想 故控制一個額外的參數  將原先的DCT反相 
			td_girlfacematrix[x][y] = sum;
			//HW2-4
			temp1[y * 512 + x] = round(td_girlfacematrix[x][y] + 128);
			//fwrite(&temp1[y * 512 + x], sizeof(unsigned char), 1, output_file24);
			sum = 0;
			//printf("temp:%d\n",temp1[y*512+x]);
			count_ = count_ + 1;
		}
	}
	fwrite(&temp1, sizeof(unsigned char), 512*512, output_file24);
	for (y = 0; y<BmpHeight; y++) {
		for (x = 0; x<BmpWidth; x++) {
			//printf("temp:%d\n",temp[y*512+x]);
			//printf("temp1:%d\n",temp1[y*512+x]);
			MSE = (temp[y * 512 + x] - temp1[y * 512 + x])*(temp[y * 512 + x] - temp1[y * 512 + x]) + MSE;
		}
	}

	printf("開始檢測2-2的MSE運算值");
	printf("MSE:%.2f\n", MSE / (512 * 512));
	PSNR = 10 * log10(pow(255, 2) / (MSE / (512 * 512)));
	printf("PSNR:%.2f\n", PSNR);
	//fwrite(&temp1,512*512, 1,output_file24);
	//================8 X 8  1-D IDCT  行運算結束==================================
	//printf("開始檢測2-4運算值是否正確");
	// 	for(x=0;x<16;x++){
	//    	for(y=0;y<16;y++){	
	//        	//printf("%.1f  ", td_lenamatrix[x][y]);
	//		}
	//        	//printf("\n");   
	//	}
	//	for(y=0;y<512;y++){
	//		for(x=0;x<512;x++){
	//			//cout<<"fd_lenamatrix="<<fd_lenamatrix[x][y]<<endl;
	//			temp1[y*512+x]=round(td_lenamatrix[x][y])+128;
	//			fwrite(&temp1[y*512+x],sizeof(unsigned char), 1,output_file24);
	//			printf("%d ", temp1[y*512+x]);
	//			//cout<<"temp1="<<temp1[y*512+x]<<endl;
	//		}
	//	}

	//	for(y=0;y<512;y++){
	//		for(x=0;x<512;x++){
	//			//cout<<"fd_lenamatrix="<<fd_lenamatrix[x][y]<<endl;
	//			fre_tempmatrix[x][y]=q[y][x];
	//			//printf("%.1f  ", fre_tempmatrix[x][y]);
	//			fwrite(&fre_tempmatrix[x][y],sizeof(unsigned char), 1,output_file23);
	//			//cout<<"temp1="<<temp1[y*512+x]<<endl;
	//		}
	//	}

	//================Watermaking HW2-5==================================
	float **q1 = (float **)malloc(sizex * sizeof(void *));
	float *iaq1 = (float *)malloc(sizex * sizey * sizeof(float *));
	for (y = 0; y != sizey; ++y, iaq1 += sizex) {
		q1[y] = iaq1;
	}
	for (y = 0; y != sizey; ++y) {
		for (x = 0; x != sizex; ++x) {
			q1[x][y] = y + x;
		}
	}
		for(y=0;y<512;y++){
			for(x=0;x<512;x++){
			//	q1[y][x]=(double)fd_girlfacematrix[y][x]+(double)watermarkbig[y][x]*8;
				//printf("q1:%.1f",q1[x][y]);
				//printf("fd:%.1f  ",fd_lenamatrix[x][y]);
				//printf("wa:%.2f  ",watermarkbig[x][y]*0.1);
			}
		}
	//	for(y=0;y<16;y++){
	//		for(x=0;x<16;x++){
	//			printf("fd:%.1f  ",fd_lenamatrix[x][y]);
	//			printf("wa:%.1f  ",watermarkbig[x][y]);
	//			printf("q1 :%.1f  ",q1[x][y]);
	//		}
	//		printf("\n");
	//	}
	sum = 0;
	//================8 X 8  1-D IDCT  浮水印列運算開始==================================                    
	for (y = 0; y<BmpHeight; y++) {
		int tdx = 0, tmp = 0;
		for (x = 0; x<BmpWidth; x++) {
			if (x % 8 == 0 && x != 0) {
				tdx = x;	                     //變數tdx(time domain的x)  儲存x的值 
				tmp = x;                        //變數tmp 儲存x的值     
			}
			else {
				tdx = tmp;                    //概念:如果x比8大則後續的tdx會累加1 ex: 8、 9、 10、 11、 12、 13、 14、 15 共8個一組  
			}
			if (x <= 7) {                       //如果x小於等於7時值都歸0 
				tdx = 0;
			}
			for (i = 0; i<length; i++) {
				if (tdx == 0 || tdx % 8 == 0) {
					alfa = 0.70710678118654752440084436210485; //  1  / 根號2  
				}
				else {
					alfa = 1;
				}
				int cc = 0;                   //宣告int儲存區域變數rr,cc
				double rr = 0;
				double pp = 0;               //宣告double儲存區域變數pp,oo
				//q1[y][tdx] = (double)fd_lenamatrix[x][y] + (double)watermarkbig[x][y]*0.1;
				rr = q1[y][tdx];     //將陣列中的灰階值儲存到rr中 ,tdx為控制變數是原先的x值 但是要8個一組(也就是說有8次運算),故要對x的值做控制,y為控制變數 如果使用x值 會做不滿512 
				//rr = (double)fd_lenamatrix[y][tdx] + (double)watermarkbig[y][tdx] *0.1;
				pp = sum;                       //將前一sum的值儲存到pp                
				cc = x % 8;                       //每當x>8時將x%8取餘數，這是為了做8x8 1D DCT 而準備的  因為是每8個一組  所以每一次都是一個N=8 1D DCT 
												  //cos內容值運算式,原先範例給的參數做出來的結果不盡理想  
				sum = rr*cos((2 * cc + 1)* i* M_PI / (double)16)*alfa;                //將原先陣列中的灰階值乘上Cos   注意:原先Cos係數因該是要乘上PI/180 才會是我們計算中Cos的值 但是此處似乎不用這樣做 
																					  //printf("q1:%f\n",abc[i]);
																					  //printf("sum1:%f\n",sum);
				sum = sum + pp;                 //將算出來的結果作8次的累加相當於數學式中的累加符號 
												//printf("sum2:%f\n",sum);
				tdx = tdx + 1;                       //每做完一次tdx會累加1 意思是我們取陣列中的灰階時往下一個移動 
			}
			sum = sum*0.5;                  		 //在累加完畢後  公式上還要在乘上C(u)的值 
			fre_tempmatrix[y][x] = sum;                   //將第一次運算完的1D DCT 存回陣列中 
														  //printf("q:%.2f   ",q[y][x]);
			sum = 0;
			count_ = count_ + 1;                        			//做完一次8x8  sum的值歸0  以便做下一次8x8x累加值 
		}
	}
	//================8 X 8  1-D IDCT  浮水印列運算結束==================================
	count_ = 0;
	sum = 0;
	//================8 X 8  1-D IDCT  浮水印行運算開始==================================
	for (x = 0; x<BmpWidth; x++) {
		int tdy = 0, tmp = 0;
		for (y = 0; y<BmpHeight; y++) {
			if (y % 8 == 0 && y != 0) {
				tdy = y;	                     //變數tdy(time domain的y)  儲存x的值 
				tmp = y;                        //變數tmp 儲存x的值     
			}
			else {
				tdy = tmp;                    //概念:如果x比8大則後續的tdx會累加1 ex: 8、 9、 10、 11、 12、 13、 14、 15 共8個一組  
			}
			if (y <= 7) {                       //如果x小於等於7時值都歸0 
				tdy = 0;
			}
			for (i = 0; i<length; i++) {
				if (tdy == 0 || tdy % 8 == 0) {
					alfa = 0.70710678118654752440084436210485; //  1  / 根號2  
				}
				else {
					alfa = 1;
				}
				//用來儲存作完1D DCT陣列中的值   因為有小數用dobule 
				if ((count_ == 0) || (count_ % 8 == 0)) {
					abc[i] = fre_tempmatrix[tdy][x];     //將做完1D- DCT陣列中的值給予rr 
														 //printf("q:%.2f   ",abc[i]);      
				}
				double pp = 0;
				int cc = 0;
				pp = sum;
				cc = y % 8;
				//printf("oo:%.2f\n",oo);
				//printf("rr:%.2f \n ", abc[k]);
				sum = abc[i] * cos((2 * cc + 1)*i* M_PI / (double)16)*alfa;
				//sum =q[tdy][x]*cos((2*cc+1)*i* M_PI/(double)16)*alfa;
				sum = sum + pp;
				tdy = tdy + 1;
				//printf("sum:%.2f\n",sum);
			}
			sum = sum *0.5;    //此處因為做完的圖片結果不盡理想 故控制一個額外的參數  將原先的DCT反相 
			td_girlfacematrix[x][y] = sum;
			//printf("sum:%.2f\n",sum);
			//printf("%.1f  ", td_lenamatrix[x][y]);
			sum = 0;
			count_ = count_ + 1;
		}
	}
	//================8 X 8  1-D IDCT  浮水印行運算結束==================================

	printf("再度開始檢測浮水印並IDCT後運算值是否正確");
	for (x = 0; x<16; x++) {
		for (y = 0; y<16; y++) {
			printf("%.1f  ", td_girlfacematrix[y][x]);
		}
		printf("\n");
	}

	for (y = 0; y<512; y++) {
		for (x = 0; x<512; x++) {
			//cout<<"fd_lenamatrix="<<fd_lenamatrix[x][y]<<endl;
			temp[y * 512 + x] = round(td_girlfacematrix[x][y] + 128);
			//printf("temp:%d\n",temp[y*512+x]);
			//printf("temp1:%d\n",temp1[y*512+x]);
			fwrite(&temp[y * 512 + x], sizeof(unsigned char), 1, output_file25);
			//cout<<"temp1="<<temp1[y*512+x]<<endl;
		}
	}

	//HW2-6   做檢測
	//================8 X 8  1-D DCT  列運算開始,對於圖片而言列是y軸 行是x軸==================================
	for (y = 0; y<BmpHeight; y++) {
		int tdx = 0, tmp = 0;
		for (x = 0; x<BmpWidth; x++) {
			//C(u)
			if (x == 0 || x % 8 == 0) {
				alfa = 0.70710678118654752440084436210485; //  1  / 根號2  
			}
			else {
				alfa = 1;
			}
			if (x % 8 == 0 && x != 0) {
				tdx = x;	                     //變數tdx(time domain的x)  儲存x的值 
				tmp = x;                        //變數tmp 儲存x的值     
			}
			else {
				tdx = tmp;                    //概念:如果x比8大則後續的tdx會累加1 ex: 8、 9、 10、 11、 12、 13、 14、 15 共8個一組  
			}
			if (x <= 7) {                       //如果x小於等於7時值都歸0 
				tdx = 0;
			}
			for (i = 0; i<length; i++) {
				int cc = 0;                   //宣告int儲存區域變數rr,cc
				double rr = 0;
				double pp = 0;               //宣告double儲存區域變數pp,oo  
				rr = td_girlfacematrix[tdx][y];     //將陣列中的灰階值儲存到rr中 ,tdx為控制變數是原先的x值 但是要8個一組(也就是說有8次運算),故要對x的值做控制,y為控制變數 如果使用x值 會做不滿512 
				pp = sum;                       //將前一sum的值儲存到pp                
				cc = x % 8;                       //每當x>8時將x%8取餘數，這是為了做8x8 1D DCT 而準備的  因為是每8個一組  所以每一次都是一個N=8 1D DCT 
												  //cos內容值運算式,原先範例給的參數做出來的結果不盡理想  
				sum = rr*cos((2 * i + 1)* cc* M_PI / (double)16);                //將原先陣列中的灰階值乘上Cos   注意:原先Cos係數因該是要乘上PI/180 才會是我們計算中Cos的值 但是此處似乎不用這樣做 
																				 //printf("sum:%.2f\n",sum);
				sum = sum + pp;                 //將算出來的結果作8次的累加相當於數學式中的累加符號 
				tdx = tdx + 1;                       //每做完一次tdx會累加1 意思是我們取陣列中的灰階時往下一個移動 
			}
			sum = sum*0.5*alfa;                  		 //在累加完畢後  公式上還要在乘上C(u)的值 
			fre_tempmatrix[y][x] = sum;                   //將第一次運算完的1D DCT 存回陣列中 
														  //printf("fre_tempmatrix:%.2f\n",fre_tempmatrix[x][y]);
			sum = 0;                        			//做完一次8x8  sum的值歸0  以便做下一次8x8x累加值 
		}
	}
	//================8 X 8  1-D DCT  列運算結束==================================
	//================8 X 8  1-D DCT  行運算開始==================================
	for (x = 0; x<BmpWidth; x++) {
		int tdy = 0, tmp = 0;
		for (y = 0; y<BmpHeight; y++) {
			if (y == 0 || y % 8 == 0) {
				alfa = 0.70710678118654752440084436210485; //  1  / 根號2  
			}
			else {
				alfa = 1;
			}
			if (y % 8 == 0 && y != 0) {
				tdy = y;	                     //變數tdx(time domain的x)  儲存x的值 
				tmp = y;                        //變數tmp 儲存x的值     
			}
			else {
				tdy = tmp;                    //概念:如果x比8大則後續的tdx會累加1 ex: 8、 9、 10、 11、 12、 13、 14、 15 共8個一組  
			}
			if (y <= 7) {                       //如果x小於等於7時值都歸0 
				tdy = 0;
			}
			for (i = 0; i<length; i++) {
				//用來儲存作完1D DCT陣列中的值   因為有小數用dobule 
				double pp = 0;
				int cc = 0;
				//    			if((count_==0)||(count_%8==0)){
				//    		 		abc[i] =fre_tempmatrix[tdy][x];     //將做完1D- DCT陣列中的值給予rr       
				//				}
				pp = sum;
				cc = y % 8;
				//printf("oo:%.2f\n",oo);
				//printf("rr:%.2f \n ", abc[k]);
				//sum =abc[i]*cos((2*i+1)*cc* M_PI/(double)16);
				sum = fre_tempmatrix[tdy][x] * cos((2 * i + 1)*cc* M_PI / (double)16);
				sum = sum + pp;
				tdy = tdy + 1;
				//printf("sum:%.2f\n",sum);
			}
			sum = sum *0.5*alfa;    //此處因為做完的圖片結果不盡理想 故控制一個額外的參數  將原先的DCT反相 
			q1[y][x] = sum;
			sum = 0;
			count_ = count_ + 1;
		}
	}
	for (y = 0; y<512; y++) {
		for (x = 0; x<512; x++) {
			q[y][x] = round(q1[x][y] - fd_girlfacematrix[x][y]);
			//printf("q1:%.2f   ", round(q1[y][x]- fd_lenamatrix[y][x]));
		}
	}

	fseek(output_file26, 1078L, SEEK_SET);
	for (y = 0; y<32; y++) {
		for (x = 0; x<32; x++) {
			if (q[y][x] != 0.0) {
				temp2[y * 32 + x] = 0;
				printf("%d   ", temp2[y * 32 + x]);
			}
			else {
				temp2[y * 32 + x] = 255;
				printf("%d   ", temp2[y * 32 + x]);
			}
			//printf("q1:%d   ", q1[y][x]);
			
		}
	}
	fwrite(&temp2, sizeof(unsigned char), 1024, output_file26);

	//HW2-2	
	//================8 X 8  1-D IDCT  列運算開始==================================                    
	for (y = 0; y<BmpHeight; y++) {
		int tdx = 0, tmp = 0;
		for (x = 0; x<BmpWidth; x++) {
			if (x % 8 == 0 && x != 0) {
				tdx = x;	                     //變數tdx(time domain的x)  儲存x的值 
				tmp = x;                        //變數tmp 儲存x的值     
			}
			else {
				tdx = tmp;                    //概念:如果x比8大則後續的tdx會累加1 ex: 8、 9、 10、 11、 12、 13、 14、 15 共8個一組  
			}
			if (x <= 7) {                       //如果x小於等於7時值都歸0 
				tdx = 0;
			}
			for (i = 0; i<length; i++) {
				if (tdx == 0 || tdx % 8 == 0) {
					alfa = 0.70710678118654752440084436210485; //  1  / 根號2  
				}
				else {
					alfa = 1;
				}
				int cc = 0;                   //宣告int儲存區域變數rr,cc
				double rr = 0;
				double pp = 0;               //宣告double儲存區域變數pp,oo  
				rr = fd_girlfacematrix[y][tdx];     //將陣列中的灰階值儲存到rr中 ,tdx為控制變數是原先的x值 但是要8個一組(也就是說有8次運算),故要對x的值做控制,y為控制變數 如果使用x值 會做不滿512 
				pp = sum;                       //將前一sum的值儲存到pp                
				cc = x % 8;                       //每當x>8時將x%8取餘數，這是為了做8x8 1D DCT 而準備的  因為是每8個一組  所以每一次都是一個N=8 1D DCT 
												  //cos內容值運算式,原先範例給的參數做出來的結果不盡理想  
				sum = rr*cos((2 * cc + 1)* i* M_PI / (double)16)*alfa;                //將原先陣列中的灰階值乘上Cos   注意:原先Cos係數因該是要乘上PI/180 才會是我們計算中Cos的值 但是此處似乎不用這樣做 
																					  //printf("sum:%.2f\n",sum);
				sum = sum + pp;                 //將算出來的結果作8次的累加相當於數學式中的累加符號 
				tdx = tdx + 1;                       //每做完一次tdx會累加1 意思是我們取陣列中的灰階時往下一個移動 
			}
			sum = sum*0.5;                  		 //在累加完畢後  公式上還要在乘上C(u)的值 
			fre_tempmatrix[y][x] = sum;                   //將第一次運算完的1D DCT 存回陣列中 
														  //printf("fre_tempmatrix:%.2f\n",fre_tempmatrix[x][y]);
			sum = 0;                        			//做完一次8x8  sum的值歸0  以便做下一次8x8x累加值 
		}
	}
	//================8 X 8  1-D IDCT  列運算結束==================================
	count_ = 0;
	//================8 X 8  1-D IDCT  行運算開始==================================
	for (x = 0; x<BmpWidth; x++) {
		int tdy = 0, tmp = 0;
		for (y = 0; y<BmpHeight; y++) {
			if (y % 8 == 0 && y != 0) {
				tdy = y;	                     //變數tdy(time domain的y)  儲存x的值 
				tmp = y;                        //變數tmp 儲存x的值     
			}
			else {
				tdy = tmp;                    //概念:如果x比8大則後續的tdx會累加1 ex: 8、 9、 10、 11、 12、 13、 14、 15 共8個一組  
			}
			if (y <= 7) {                       //如果x小於等於7時值都歸0 
				tdy = 0;
			}
			for (i = 0; i<length; i++) {
				if (tdy == 0 || tdy % 8 == 0) {
					alfa = 0.70710678118654752440084436210485; //  1  / 根號2  
				}
				else {
					alfa = 1;
				}
				//用來儲存作完1D DCT陣列中的值   因為有小數用dobule 
				if ((count_ == 0) || (count_ % 8 == 0)) {
					abc[i] = fre_tempmatrix[tdy][x];     //將做完1D- DCT陣列中的值給予rr       
				}
				double pp = 0;
				int cc = 0;
				pp = sum;
				cc = y % 8;
				//printf("oo:%.2f\n",oo);
				//printf("rr:%.2f \n ", abc[k]);
				sum = abc[i] * cos((2 * cc + 1)*i* M_PI / (double)16)*alfa;
				//sum =fre_tempmatrix[tdy][x]*cos((2*cc+1)*i* M_PI/(double)16)*alfa;
				sum = sum + pp;
				tdy = tdy + 1;
				//printf("sum:%.2f\n",sum);
			}
			sum = sum *0.5;    //此處因為做完的圖片結果不盡理想 故控制一個額外的參數  將原先的DCT反相 
			td_girlfacematrix[x][y] = sum;
			//printf("sum:%.2f\n",sum);
			//printf("td_lenamatrix:%.2f",td_lenamatrix[x][y]);
			sum = 0;
			count_ = count_ + 1;
		}
	}
	//================8 X 8  1-D IDCT  行運算結束==================================
	printf("再度開始檢測運算值是否正確");
	for (x = 0; x<16; x++) {
		for (y = 0; y<16; y++) {
			printf("%.1f  ", td_girlfacematrix[x][y]);
		}
		printf("\n");
	}

	for (y = 0; y<512; y++) {
		for (x = 0; x<512; x++) {
			//cout<<"fd_lenamatrix="<<fd_lenamatrix[x][y]<<endl;
			temp1[y * 512 + x] = round(td_girlfacematrix[x][y] + 128);
			//printf("temp:%d\n",temp[y*512+x]);
			//printf("temp1:%d\n",temp1[y*512+x]);
			fwrite(&temp1[y * 512 + x], sizeof(unsigned char), 1, output_file22);
			//cout<<"temp1="<<temp1[y*512+x]<<endl;
		}
	}

	



	//================8 X 8  2-D DCT  行運算開始==================================

	//	int indx=0,indy=0,tdx=0,tdy=0,fredu=0,fredv=0,i;
	//	float tmp,alfa;  
	//	
	//	for(indy=0;indy<(BmpHeight/length);indy++){
	//		for(indx=0;indx<(BmpWidth/length);indx++){
	//			for(y=0;y<length;y++){
	//				for(x=0;x<length;x++){
	//					tmp=0;
	//					fredu=indx*length+x;
	//					//cout<<"fredu="<<fredu<<endl;
	//        			fredv=indy*length+y;
	//					//cout<<"fredv="<<fredv<<endl;
	//					if (fredu == 0 && fredv == 0){
	//						alfa = 0.125;
	//       					//cout<<"alfa="<<alfa<<endl;
	//					}
	//					else if(fredu != 0 && fredv == 0){
	//						alfa=1/(4*sqrt(2.0));
	//					}
	//					else if(fredu == 0 && fredv != 0){
	//						alfa=1/(4*sqrt(2.0));
	//					}
	//    				else if(fredu != 0 && fredv != 0){
	//						alfa = 0.25;
	//					}
	//					//cout<<"alfa="<<alfa<<endl;
	//					for (i = 0; i < length * length; i++) {                          //DCT formular
	//    					int tdy = i / length;
	//        				int tdx = i % length;
	//        				//cout<<"td_lenamatrix="<<td_lenamatrix[0][0]<<endl;
	//        				tmp += td_lenamatrix[indx*length+tdx][indy*length+tdy]* cos((2 * (tdx) + 1) * x * M_PI * cos((2 * (tdy) + 1) * y * M_PI / ((2.0 * length)* (2.0 * length))));  
	//              			//cout<<"td_lenamatrix="<<td_lenamatrix[indx*length+tdx][indy*length+tdy]<<" x="<<indx*length+tdx<<" y="<<indy*length+tdy<<endl;
	//              			//cout<<"x="<<indx*length+tdx<<" y="<<indy*length+tdy<<endl;
	//    				}
	//    				//cout<<"tmp="<<tmp<<endl;
	//    				//cout<<"alfa="<<alfa<<endl;
	//    				fd_lenamatrix[fredu][fredv]= alfa * tmp;
	//    				//cout<<"fd_lenamatrix="<<fd_lenamatrix[fredu][fredv]<<endl;
	//    				//cout<<"temp1="<<temp1[fredv*512+fredu]<<endl;
	//				}
	//			}
	//		}
	//	}
	//	
	//	for(y=0;y<512;y++){
	//		for(x=0;x<512;x++){
	//			//cout<<"fd_lenamatrix="<<fd_lenamatrix[x][y]<<endl;
	//			temp1[y*512+x]=fd_lenamatrix[x][y];
	//			fwrite(&temp1[y*512+x],sizeof(unsigned char), 1,output_file);
	//			//cout<<"temp1="<<temp1[y*512+x]<<endl;
	//		}
	//	}
	//	float max = temp1[0];
	//    for(i=0;i<512*512;i++){
	//    	if(fabs(temp1[i])>max){
	//    		max = fabs(temp1[i]);
	//        	//cout<<"max="<<max<<endl;
	//		}
	//	}
	//    //cout<<"max="<<max<<endl;
	//    fseek(output_file,1078L,SEEK_SET);   //跳至第1078個byte 準備存值
	//	for(y=0;y<512;y++){
	//		for(x=0;x<512;x++){
	//			temp1[y*512+x]=(temp1[y*512+x]/max+1)*255;
	//			fwrite(&temp1[y*512+x],sizeof(unsigned char), 1,output_file);
	//			//cout<<"temp1="<<temp1[y*512+x]/max*255+128<<endl;
	//			//cout<<"temp1="<<temp1[y*512+x]<<endl;
	//		}
	//	}

	//	fseek(output_file,1078L,SEEK_SET);   //跳至第1078個byte 準備存值 
	//	fwrite(&temp1,1,BmpWidth*BmpHeight,output_file);

	free(td_girlfacematrix[0]);
	free(fd_girlfacematrix[0]);
	free(q[0]);
	free(q1[0]);
	free(td_girlfacematrix);
	free(fd_girlfacematrix);
	free(q);
	free(q1);
	return 0;
}


