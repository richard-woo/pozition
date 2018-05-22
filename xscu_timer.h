/*
 * xscu_timer.h
 *
 *  Created on: 2018Äê4ÔÂ23ÈÕ
 *      Author: Administrator
 */

#ifndef SRC_XSCU_TIMER_H_
#define SRC_XSCU_TIMER_H_

#include "xscutimer.h"
#include "xscugic.h"


/************************** Constant Definitions *****************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are only defined here such that a user can easily
 * change all the needed parameters in one place.
 */

#define TIMER_DEVICE_ID		XPAR_XSCUTIMER_0_DEVICE_ID
#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID
#define TIMER_IRPT_INTR		XPAR_SCUTIMER_INTR

extern XScuTimer TimerInstance;
#define GET_TIMER_VALUE() XScuTimer_GetCounterValue(&TimerInstance);

extern volatile int TimerExpired;
extern int ScuTimerIntrInit(XScuGic *IntcInstancePtr, XScuTimer * TimerInstancePtr,
			u16 TimerDeviceId, u16 TimerIntrId);
extern int TimerInit(XScuGic *IntcInstancePtr);



#endif /* SRC_XSCU_TIMER_H_ */
