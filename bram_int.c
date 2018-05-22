/*
 * bram_int.c
 *
 *  Created on: 2018Äê4ÔÂ23ÈÕ
 *      Author: Administrator
 */

#include <stdio.h>
#include "xparameters.h"
#include "xstatus.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "xil_io.h"
#include "xscu_timer.h"
#include "bram_int.h"
#include "calc_main.h"

#define BRAM_READ_INT 62
#define BRAM_WRITE_INT 61


#define BRAM_WriteReg(RegOffset, Data) \
	(Xil_Out32(((BRAM_READ_BASE_ADDR) + (RegOffset)), ((u32)(Data))))

#define BRAM_ReadReg(RegOffset) \
	(Xil_In32((BRAM_READ_BASE_ADDR) + (RegOffset)))
#if 0
static XScuGic BramIntc;
#endif

int BramData[BRAM_READ_DATA_SIZE / 4] = {0};
UINT32 CalcData[M*(M-1)] = {0};


int DataIsReady = 0;
int BramIsReady = 1;

void BramReadTest(void)
{
	int i;
	//u32 stat;

	printf("bram test begin\n");

	for (i = 0; i < BRAM_READ_DATA_SIZE / 4; i++)
	{
		BramData[i] = Xil_In32(BRAM_READ_BASE_ADDR + i * 4);
	}
	printf("BramData[0~9]:%x,%x,%x,%x,%x,%x,%x,%x,%x,%x\n",
			BramData[0],BramData[1],BramData[2],BramData[3],
			BramData[4],BramData[5],BramData[6],BramData[7],
			BramData[8],BramData[9]);
}

void BramWriteData(void)
{
	int i;

	//printf("bram Write begin\n");

	memcpy(CalcData, UN, sizeof(CalcData));
	//printf("sizeof %u\n",sizeof(CalcData));
	for (i = 0; i < M * (M - 1); i++) {
	//for (i = 0; i < 0x2; i++) {
		Xil_Out32(BRAM_WRITE_BASE_ADDR + i * 4, CalcData[i]);
	}

}
void BramWriteDataTest(void)
{
	int i;

	//printf("bram Write begin\n");

	for (i = 0; i < sizeof(UN); i++) {
		Xil_Out32(BRAM_WRITE_BASE_ADDR + i * 4, i);
	}

}


void BramReadISR(void)
{
	int i;
	int stat = 0;

	//printf("enter read isr 62!!\n");

	/*Clean Bram readable interrupt*/
	stat = Xil_In32(BRAM_READ_INT_CLEAN);
	//printf("Bram int stat: %u\n",stat);

	for (i = 0; i < (BRAM_READ_DATA_SIZE / 4); i++)
	{
		BramData[i] = Xil_In32(BRAM_READ_BASE_ADDR + i*4);
	}

	/*for (i = 0; i < (BRAM_READ_DATA_SIZE / 4); i++)
	{
		printf("%08x ",BramData[i]);
		if (i % 2 == 1)
			printf("\n");
	}
	printf("\n");*/
	DataIsReady = 1;

}


void BramWriteISR(void)
{

	//printf("enter write isr 61!!\n");
	/*clean the write interrupt*/
	Xil_Out32(BRAM_WRITE_INT_CLEAN, 0);
	BramIsReady = 1;

}

int BramReadInterruptSetup(XScuGic *InstancePtr)
{

	int Status;
#if 0
	static XScuGic_Config *GicConfig;
	/*
	 * Initialize the interrupt controller driver so that it is ready to
	 * use.
	 */
	GicConfig = XScuGic_LookupConfig(0);
	if (NULL == GicConfig) {
		return XST_FAILURE;
	}

	Status = XScuGic_CfgInitialize(&BramIntc, GicConfig,
					GicConfig->CpuBaseAddress);
		if (Status != XST_SUCCESS) {
			return XST_FAILURE;
		}

	/*
	 * Connect the interrupt controller interrupt handler to the hardware
	 * interrupt handling logic in the processor.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
				(Xil_ExceptionHandler)XScuGic_InterruptHandler,
				&BramIntc);
#endif
	/*
	 * Connect the Fault ISR
	 */
	Status = XScuGic_Connect(InstancePtr,
				BRAM_READ_INT,
				 (Xil_InterruptHandler)BramReadISR,
				 (void *)InstancePtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	//Only used for edge sensitive Interrupts
	XScuGic_SetPriorityTriggerType(InstancePtr, BRAM_READ_INT, 0xa0, 1);

	/*
	 * Enable the interrupts for the device
	 */
	XScuGic_Enable(InstancePtr, BRAM_READ_INT);
#if 0
	/*
	 * Enable exceptions.
	 */
	Xil_ExceptionEnable();
#endif
	return XST_SUCCESS;

}

int BramWriteInterruptSetup(XScuGic *InstancePtr)
{

	int Status;

	/*
	 * Connect the Fault ISR
	 */
	Status = XScuGic_Connect(InstancePtr,
				BRAM_WRITE_INT,
				 (Xil_InterruptHandler)BramWriteISR,
				 (void *)InstancePtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	//Only used for edge sensitive Interrupts
	XScuGic_SetPriorityTriggerType(InstancePtr, BRAM_WRITE_INT, 0xa0, 1);

	/*
	 * Enable the interrupts for the device
	 */
	XScuGic_Enable(InstancePtr, BRAM_WRITE_INT);

	return XST_SUCCESS;

}

