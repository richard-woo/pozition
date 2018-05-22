/*
 * bram_int.h
 *
 *  Created on: 2018Äê4ÔÂ23ÈÕ
 *      Author: Administrator
 */

#ifndef SRC_BRAM_INT_H_
#define SRC_BRAM_INT_H_

#define BRAM_READ_BASE_ADDR 0x40000000
#define BRAM_WRITE_BASE_ADDR 0x40010000//x40001000

#define BRAM_READ_INT_CLEAN (BRAM_READ_BASE_ADDR + 0x400)//0x40000400
#define BRAM_WRITE_OK (BRAM_WRITE_BASE_ADDR + 0x400)//0x4000134c//0x40001400
#define BRAM_WRITE_INT_CLEAN (BRAM_WRITE_BASE_ADDR + 0x404)//0x40001404

#define BRAM_READ_DATA_SIZE (0x400003ff -0x40000000)
#define BRAM_WRITE_DATA_SIZE (0x400013ff -0x40001000)


extern int DataIsReady;
extern int BramIsReady;
extern int BramData[ BRAM_READ_DATA_SIZE / 4];

extern void WriteToContronReg(int value);
extern int BramReadInterruptSetup(XScuGic *InstancePtr);
extern int BramWriteInterruptSetup(XScuGic *InstancePtr);
extern void BramReadTest(void);
void BramWriteData(void);
void BramWriteDataTest(void);

#endif /* SRC_BRAM_INT_H_ */
