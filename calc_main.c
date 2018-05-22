/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include <math.h>
#include "platform.h"
#include "xil_printf.h"
#include "xscu_timer.h"
#include "sleep.h"
#include "bram_int.h"
#include "calc_main.h"




INT32 COVAR[M*M+M] =
{
    35610405,          0,
    -28276466,   59562815,
    -67290316,  154844492,
    -50101984,  -46121336,
    1206289,  -32327666,
    -51184034,    2537346,
    31476526,   38501769,
    -15471155,  -57352438,
    30917913,  -57089115,
    86603767,  -25931750,
    47942538,  -17697282,
    -58215213,  -50754648,
    -4249834,     501047,
    -52656427,  -84667156,
    64607751,   29818319,
    123269823,          0,
    314988399,  -10407422,
    -37487884,  121680819,
    -55331273,   24003017,
    45370378,   84468265,
    39892966,  -83913498,
    -84153824,   72130218,
    -121128124,   -6492174,
    -113332626, -125291289,
    -68401727,  -66594885,
    -38811569,  138974261,
    4046919,    6672184,
    -100732377,  156837680,
    -1562495, -132706399,
    808759284,          0,
    -106306650,  308941262,
    -143718759,   57122505,
    109015680,  220733610,
    109615515, -211611692,
    -221530854,  177741334,
    -309779560,  -26880156,
    -280165739, -330844222,
    -169777715, -176361183,
    -111301486,  353126605,
    9501154,   17200464,
    -272041531,  393329502,
    7042811, -340037414,
    132730188,          0,
    40999852,   47426116,
    70303898,  -70685835,
    -95124965,  -14272654,
    97343678,   61085518,
    30341891,  122013791,
    -89787328,  150890867,
    -45043702,   88401690,
    149876743,   -3946787,
    5090528,   -5702625,
    186176569,   52541032,
    -130840651,   42252220,
    30031172,          0,
    -3582639,  -46884058,
    -34295180,   29746876,
    52090415,  -16133786,
    53132750,   26667953,
    26351569,   78817883,
    17756820,   43525354,
    44949997,  -54934627,
    -683979,   -3599428,
    76135603,  -50494570,
    -25180752,   60158352,
    75475199,          0,
    -42903873,  -58643354,
    18776450,   84257281,
    -49279863,   81011346,
    -128208863,   32078575,
    -71001220,   22756859,
    81650305,   77976996,
    5810730,     -46600,
    70812321,  127571245,
    -91681214,  -47589971,
    70606557,          0,
    -76360509,  -33813130,
    -34996327,  -84800839,
    48455558, -118146061,
    23020608,  -68268287,
    -107403913,   18862513,
    -3424577,    4772885,
    -140005495,  -17748814,
    89834020,  -44115769,
    100304817,          0,
    78907439,   75518948,
    3735607,  152288880,
    7588310,   85841873,
    108268428,  -72092319,
    926125,   -6855714,
    160874368,  -47810714,
    -76821111,   91869646,
    119977949,          0,
    118455793,  117411188,
    71012940,   61854979,
    30860401, -139093797,
    -4149896,   -6334025,
    91158066, -160016152,
    8662919,  130526964,
    233047928,          0,
    131190728,   -8536061,
    -106056338, -168079836,
    -10105592,   -1968519,
    -66590581, -247880077,
    136912130,  120484570,
    74543368,          0,
    -53641299,  -98630241,
    -5644940,   -1374932,
    -28497033, -141984533,
    72905941,   72902708,
    170109276,          0,
    6024458,   -6372062,
    209370273,   64790110,
    -149361687,   43907795,
    955990,          0,
    5337434,   10187583,
    -7117803,   -4416368,
    283618091,          0,
    -167583131,  110922126,
    143527914,            0
};

FP64 A[N][N] = {{0},{0}};   //input square matrix A
FP64 Q[N][N] = {{0},{0}};   //Householder变换中的乘积矩阵
FP64 B[N] = {0};      //对称三对角阵中的主对角元素
FP64 C[N] = {0};      //前N-1个元素为对称三对角中的次对角线元素

UINT32 UN[M-1][M] = {{0},{0}};

//input: A (real -imag; imag real)
void InitSquare()
{
    int n=0, i=0, j=0, idx=M, center=0;

    // 30*30 [real -imag; imag real]
    for (n=0; n<(M*M+M)/2; n++)
    {
        if (center)
            j = (n%center+i);
        else
            j = n;

        A[i][j] = COVAR[2*n]*1.0;
        A[j][i] = COVAR[2*n]*1.0;

        A[i+M][j] = COVAR[2*n+1]*1.0;
        A[j+M][i] = (-COVAR[2*n+1])*1.0;

        if (j==(M-1))
        {
            i++;
            center+=idx;
            idx--;
        }
    }

    for (i=0; i<M; i++)
    {
        for (j=M; j<N; j++)
        {
            A[i][j] = -A[i+M][j-M];
            A[i+M][j] = A[i][j-M];
        }
    }
}

//执行Householder变换
void a_strq()
{
    int i,j,k;
    FP64 h,f,g,h2;

    for (i=0; i<=N-1; i++)
        for (j=0; j<=N-1; j++)
            Q[i][j]=A[i][j];

    for (i=N-1; i>=1; i--)
    {
        h=0.0;

        if (i>1)
            for (k=0; k<=i-1; k++)
                h=h+Q[i][k]*Q[i][k];

        if (h+1.0==1.0)
        {
            C[i]=0.0;
            if (i==1)
                C[i]=Q[i][i-1];
            B[i]=0.0;
        }
        else
        {
            C[i]=sqrt(h);

            if (Q[i][i-1]>0.0)
                C[i]=-C[i];

            h=h-Q[i][i-1]*C[i];
            Q[i][i-1]=Q[i][i-1]-C[i];

            f=0.0;
            for (j=0; j<=i-1; j++)
            {
                Q[j][i]=Q[i][j]/h;

                g=0.0;
                for (k=0; k<=j; k++)
                    g=g+Q[j][k]*Q[i][k];

                if (j+1<=i-1)
                    for (k=j+1; k<=i-1; k++)
                        g=g+Q[k][j]*Q[i][k];

                C[j]=g/h;
                f=f+g*Q[j][i];
            }

            h2=f/(h+h);
            for (j=0; j<=i-1; j++)
            {
                f=Q[i][j];
                g=C[j]-h2*f;
                C[j]=g;

                for (k=0; k<=j; k++)
                {
                    Q[j][k]=Q[j][k]-f*C[k]-g*Q[i][k];
                }
            }

            B[i]=h;
        }
    }

    for (i=0; i<=N-2; i++)
        C[i]=C[i+1];

    C[N-1]=0.0;

    B[0]=0.0;
    for (i=0; i<=N-1; i++)
    {
        if ((B[i]!=0.0)&&(i-1>=0))
            for (j=0; j<=i-1; j++)
            {
                g=0.0;
                for (k=0; k<=i-1; k++)
                    g=g+Q[i][k]*Q[k][j];

                for (k=0; k<=i-1; k++)
                {
                    Q[k][j]=Q[k][j]-g*Q[k][i];
                }
            }

        B[i]=Q[i][i];
        Q[i][i]=1.0;

        if (i-1>=0)
            for (j=0; j<=i-1; j++)
            {
                Q[i][j]=0.0; Q[j][i]=0.0;
            }
    }
}

//计算全部特征值和特征向量
void sstq()
{
    int i,j,k,m,it;
    FP64 d,f,h,g,p,r,e,s;

    C[N-1]=0.0;
    d=0.0;
    f=0.0;

    for (j=0; j<=N-1; j++)
    {
        it=0;
        h=ESP*(fabs(B[j])+fabs(C[j]));

        if (h>d) d=h;
            m=j;

        while ((m<=N-1)&&(fabs(C[m])>d))
            m=m+1;

        if (m!=j)
        {
            do
            {
                if (it==MAXITER)
                {
                    return;
                }

                it=it+1;
                g=B[j];
                p=(B[j+1]-g)/(2.0*C[j]);
                r=sqrt(p*p+1.0);

                if (p>=0.0)
                    B[j]=C[j]/(p+r);
                else
                    B[j]=C[j]/(p-r);

                h=g-B[j];

                for (i=j+1; i<=N-1; i++)
                    B[i]=B[i]-h;

                f=f+h;
                p=B[m];
                e=1.0;
                s=0.0;

                for (i=m-1; i>=j; i--)
                {
                    g=e*C[i];
                    h=e*p;

                    if (fabs(p)>=fabs(C[i]))
                    {
                        e=C[i]/p;
                        r=sqrt(e*e+1.0);
                        C[i+1]=s*p*r;
                        s=e/r;
                        e=1.0/r;
                    }
                    else
                    {
                        e=p/C[i];
                        r=sqrt(e*e+1.0);
                        C[i+1]=s*C[i]*r;
                        s=1.0/r;
                        e=e/r;
                    }

                    p=e*B[i]-s*g;
                    B[i+1]=h+s*(e*g+s*B[i]);

                    for (k=0; k<=N-1; k++)
                    {
                        h=Q[k][i+1]; Q[k][i+1]=s*Q[k][i]+e*h;
                        Q[k][i]=e*Q[k][i]-s*h;
                    }
                }

                C[j]=s*p;
                B[j]=e*p;
            } while (fabs(C[j])>d);
        }

        B[j]=B[j]+f;
    }

    for (i=0; i<=N-1; i++)
    {
        k=i;
        p=B[i];
        if (i+1<=N-1)
        {
            j=i+1;
            while ((j<=N-1)&&(B[j]<=p))
            {
                k=j;
                p=B[j];
                j=j+1;
            }
        }

        if (k!=i)
        {
            B[k]=B[i];
            B[i]=p;
            for (j=0; j<=N-1; j++)
            {
              p=Q[j][i];
              Q[j][i]=Q[j][k];
              Q[j][k]=p;
            }
        }
    }
}

void SortEigenVector(int j1, int j2)
{
    int i;
    FP64 temp;

    for (i=0; i<N; i++)
    {
        temp = Q[i][j1];
        Q[i][j1] = Q[i][j2];
        Q[i][j2] = temp;
    }
}

void SortEigenValue()
{
    int i, j;
    FP64 temp;

    for(i=0; i<N; ++i)
    {
        for(j=i+1; j<N; ++j)
        {
            if(B[j] < B[i])
            {
                //如果后一个元素小于前一个元素则交换
                temp = B[i];
                B[i] = B[j];
                B[j] = temp;
                SortEigenVector(i, j);
            }
        }
    }
}


//output: B and Q
void CheckEig()
{
    int i=0, j=0;
    INT16 unr=0, uni=0;

    SortEigenValue();

    //14*15
    for (i=0; i<(M-1); i++)
    {
        for (j=0; j<M; j++)
        {
            unr = (INT16)floor(Q[j][i*2]*(32768));
            uni = (INT16)floor(Q[j+M][i*2]*(32768));
            UN[i][j] = (uni<<16)+unr;
        }
    }
}


// 0-UINT32, 1-dec
void DumpMatrix32(char *str, UINT32 *pSrcBufVA, unsigned int row, unsigned int col, char type)
{
    UINT32 *pt;
    unsigned int x;

    pt = pSrcBufVA;
    printf("%s: %p, row=%d, col=%d\n", str, pSrcBufVA, row, col);

    for (x = 0; x < row*col; x++)
    {
        if (type==0)
            printf("%08x ", ((UINT32)pt[x]));
        else if (type==1)
            printf("%d ", ((INT32)pt[x]));

        if (x % col == (col-1))
            printf("\n");
    }

    printf("\n");
}

void delay(int ms)
{
	int end, i = 0;

	end = TimerExpired + ms;
	//printf(">>%u",TimerExpired);
	do
	{
		i = TimerExpired;
	} while (i < end);
	//printf(">>>%u",TimerExpired);
	return;
}

void PrintRunning(void)
{
	char a[]={'-','\\','|','/'};
	int i = 0;
	while(1)
	{
		for(i=0; i<4; i++)
		{
			printf("%c",a[i]);
			//usleep(1000);
			delay(1000);
			printf("\b");
		}
	}
}

XScuGic IntcInstance;		/* Interrupt Controller Instance */

int GicInterruptSetup(void)
{

	int Status;
	XScuGic_Config *IntcConfig;

	/*
	 * Initialize the interrupt controller driver so that it is ready to
	 * use.
	 */
	IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
	if (NULL == IntcConfig) {
		return XST_FAILURE;
	}

	Status = XScuGic_CfgInitialize(&IntcInstance, IntcConfig,
					IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	Xil_ExceptionInit();
	/*
	 * Connect the interrupt controller interrupt handler to the hardware
	 * interrupt handling logic in the processor.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
				(Xil_ExceptionHandler)XScuGic_InterruptHandler,
				&IntcInstance);

	/*
	 * Enable exceptions.
	 */
	Xil_ExceptionEnable();


	return XST_SUCCESS;

}

int main()
{
	volatile UINT32 start = 0;
	volatile UINT32 end = 0;
	int i = 100;

	init_platform();

	//BramReadTest();
	//BramWriteTest();

	GicInterruptSetup();
	TimerInit(&IntcInstance);
	if (0 != BramReadInterruptSetup(&IntcInstance))
	{
		printf("BRAM Read interrupt is fail! \n");
	}

	if (0 != BramWriteInterruptSetup(&IntcInstance))
	{
		printf("BRAM Wite interrupt is fail! \n");
	}

	start = TimerExpired;

	printf("Start = %u !\n", start);

	do
	{
		InitSquare();
		a_strq ();          //执行Householder变换
		sstq();             //计算全部特征值和特征向量
		CheckEig();
		i--;
	} while (i);

	end = TimerExpired;
	printf("End = %u!\n", end);
	//printf("time=%f (ms)\n",(double)(end-start)*1000.0/CLOCKS_PER_SEC);
	printf("time=%f (ms)\n",(double)(end-start));
	DumpMatrix32("UN", &UN[0][0], 1, M, 0);

	while(1)
	{
		if (DataIsReady == 1)
		{
			start = TimerExpired;
			printf("calc start!! %u\n", start);
			memcpy(COVAR, BramData, sizeof(COVAR));
			//DumpMatrix32("COVER", (UINT32 *)COVAR, 120, 2, 0);
			InitSquare();
			a_strq ();          //执行Householder变换
			sstq();             //计算全部特征值和特征向量
			CheckEig();
			DataIsReady = 0;
			end = TimerExpired;
			//printf("%u,%u\n", end, end - start);
			//DumpMatrix32("UN", &UN[0][0], M -1, M, 0);
			if (BramIsReady == 1)
			{
				BramIsReady = 0;
				BramWriteData();
				Xil_Out32(BRAM_WRITE_OK, 0xa);
				end = TimerExpired;
				printf("calc end!! %u,%u\n", end, end - start);
			}
		}


	}
	cleanup_platform();
	return 0;
}
