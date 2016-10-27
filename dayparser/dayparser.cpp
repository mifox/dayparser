// dayparser.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


// stock.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

typedef struct
{
	short  date1;
	short  date2;
	float open;
	float high;
	float low;
	float close;
	int amount;
	int vol;
	int reservation;
} StockDataMin; 

typedef struct
{
	int  date1;
	float open;
	float high;
	float low;
	float close;
	int amount;
	int vol;
	int reservation;
} StockDataDay; 

StockDataMin stockData;
int read_data1m(FILE *);
int read_data1day(FILE *);
int read_datagbbq(FILE *fp);
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	string filename;
	string mode="";
	if (argc < 2)
	{
		printf("filename\n");
		return 0;
	}
	else
	{
		mode = argv[1];
		filename = argv[2];
	}


	FILE *fp;
	if((fp = fopen(filename.c_str(),"rb")) == NULL) // 打开招商银行日线
	{ 
		printf("Error: Can^t open sh600036.DAY ! ");
		exit(0);
	}

	if (mode==string("day"))
	{
		read_data1day(fp);
	}
	else if (mode==string("gbbq"))
	{
		read_datagbbq(fp);
	}
	else
	{
		read_data1m(fp);
	}
	
	fclose(fp);
	if(getch()==0) getch();
	exit(0);
}

int read_data1m(FILE *fp)
{
	float fn;
	//fread(&stockData,10,1,fp);
	int i=0;
	while (! feof(fp)) {
		//printf("%d ",i++);
		i++;
		fread(&stockData,sizeof(StockDataMin),1,fp);
		int    year=int(stockData.date1/2048)+2004;
        int    month=int((stockData.date1%2048)/100);
        int    day=stockData.date1%2048%100;
// 		printf("%d %x %d %d %d %d %d %d ",i,stockData.date1,stockData.date1,year,month,day,stockData.date2/60,stockData.date2%60);
// 		fn=stockData.open;
// 		printf("%8.2f ",fn);		
// 		fn=stockData.high;
// 		printf("%8.2f ",fn);
// 		fn=stockData.low;
// 		printf("%8.2f ",fn);
// 		fn=stockData.close;
// 		printf("%8.2f ",fn);
// 		printf("%8lu ",stockData.amount);
// 		fn=float(stockData.vol);
// 		printf("%8.2f \n",fn);
		printf("%d,%04d%02d%02d %02d:%02d:00+00,%.2f,%.2f,%.2f,%.2f,%d,%d\n",i,year,month,day,stockData.date2/60\
			,stockData.date2%60,stockData.open,stockData.high,stockData.low,stockData.close,stockData.amount,stockData.vol);

	}
	printf(" ");
	return 0;
}

int read_data1day(FILE *fp)
{
	float fn;
	//fread(&stockData,10,1,fp);
	int i=0;
	StockDataDay stockDataDay;
	while (! feof(fp)) {
		//printf("%d ",i++);
		i++;
		fread(&stockDataDay,sizeof(StockDataDay),1,fp);
// 		int    year=int(stockData.date1/2048)+2004;
// 		int    month=int((stockData.date1%2048)/100);
// 		int    day=stockData.date1%2048%100;
		printf("%d ",stockDataDay.date1);
		fn=stockDataDay.open;
		printf("%8.2f ",fn);		
		fn=stockDataDay.high;
		printf("%8.2f ",fn);
		fn=stockDataDay.low;
		printf("%8.2f ",fn);
		fn=stockDataDay.close;
		printf("%8.2f ",fn);
		printf("%8lu ",stockDataDay.amount);
		fn=float(stockDataDay.vol);
		printf("%8.2f \n",fn);
	}
	printf(" ");
	return 0;
}



int read_datagbbq(FILE *fp)
{
	float fn;
	int len =100;
	fread(&len,4,1,fp);

	int i,j;
	int eax;
	int ebx;
	int num;
	int numold;
	int* p;
    unsigned int* pInt;
	int codeNow;
	
	char* pCodeNow;
	char* pDataNow;
	//char dataNow[30]={0};
	char dataNow[30]={0};
	while (len)
	{
		fread(dataNow,29,1,fp);
		//pCodeNow = tmp;
		pDataNow = dataNow;
		for (i = 0; i < 3; i++)
		{
			eax = *((int*)(pCodeNow + 0x44));
			ebx=*((int*)(pDataNow));
			num = eax^ebx;
			numold = *((int*)(pDataNow + 0x4));

			for (j = 0x40; j > 0; j = j - 4)
			{
				ebx = (num & 0xff0000) >> 16;
				eax = *((int*)(pCodeNow + ebx * 4 + 0x448));
				ebx = num >> 24;
				eax += *((int*)(pCodeNow + ebx * 4 + 0x48));
				ebx = (num & 0xff00) >> 8;
				eax ^= *((int*)(pCodeNow + ebx * 4 + 0x848));
				ebx = num & 0xff;
				eax += *((int*)(pCodeNow + ebx * 4 + 0xC48));
				eax ^= *((int*)(pCodeNow + j));

				ebx = num;
				num = numold^eax;
				numold = ebx;
			}
			numold ^= *((int*)pCodeNow);
			pInt = (unsigned int*)pDataNow;
			*pInt = numold;
			pInt = (unsigned int*)(pDataNow+4);
			*pInt = num;
			pDataNow = pDataNow + 8;
		}
		pDataNow = pDataNow + 5;
		len--;
	}
	printf(" ");
	return 0;
}