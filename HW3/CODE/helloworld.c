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
#include "platform.h"

#include "xparameters.h"//包含處理器的地址與設備ID定義
#include "xgpio.h"		//GPIO函式庫
#include "xscugic.h"	//GIC函式庫

#define Z700_INTC_ID	XPAR_PS7_SCUGIC_0_DEVICE_ID 		  // for ZYNQ 700系列中斷編號
#define BTN_ID 	XPAR_AXI_GPIO_1_DEVICE_ID					  //包含在"xparameters.h"內
#define LED_ID	XPAR_AXI_GPIO_0_DEVICE_ID					  //包含在"xparameters.h"內
#define INTC_GPIO_ID XPAR_FABRIC_AXI_GPIO_1_IP2INTC_IRPT_INTR //GPIO中斷編號 包含在"xparameters.h"內
#define BTN_INT 		XGPIO_IR_CH1_MASK					  //GPIO中斷通道
#define GIC_ID XPAR_SCUGIC_SINGLE_DEVICE_ID 				  //GIC通用中斷編號

XGpio LED, BTN;
XScuGic INTCInst;
XScuGic_Config *IntcConfig;
//static int LED_data, btn_value;
void Intr_Handler();
void Intr_Setup(XScuGic *GicInstancePtr, XGpio *GpioInstancePtr);
void delay(int dly);
int LED_num = 0b00000000;
static int Intr_CTN;


int main()
{
	init_platform();//系統初始化設定
	XGpio_Initialize(&LED, LED_ID);//初始化GPIO LED腳位
	XGpio_SetDataDirection(&LED, 1, 0);//通道1, 輸出

	XGpio_Initialize(&BTN, BTN_ID);//初始化GPIO 指撥開關
	XGpio_SetDataDirection(&BTN, 1, 1);//通道1, 輸入

	Intr_Setup(&INTCInst, &BTN);//中斷初始化
	print("Init successful");

	while(1)
	{
		XGpio_DiscreteWrite(&LED, 1, LED_num);//控制LED輸出顯示
		printf("LED_num=%x\n",LED_num);//顯示當前LED輸出
		LED_num = LED_num +1;//無窮迴圈計數LED
		delay(500);

	}
}

void Intr_Setup(XScuGic *GicInstancePtr, XGpio *GpioInstancePtr)//中斷初始化設定
{
	IntcConfig = XScuGic_LookupConfig(GIC_ID);//尋找當前SOC的中斷配置
	XScuGic_CfgInitialize(&INTCInst, IntcConfig, IntcConfig->CpuBaseAddress);//初始化設定當前SOC的中斷配置

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,(Xil_ExceptionHandler)XScuGic_InterruptHandler, GicInstancePtr);//新增中斷處理程序
	Xil_ExceptionEnableMask(XIL_EXCEPTION_IRQ);//SOC中斷啟用
	XScuGic_Connect(GicInstancePtr, INTC_GPIO_ID, (Xil_ExceptionHandler)Intr_Handler, (void *)GpioInstancePtr);
	//連接中斷、GPIO中斷、特定中斷函式、GPIO腳位

	XScuGic_Enable(GicInstancePtr, INTC_GPIO_ID);//使中斷控制能接受特定GPIO中斷
	XGpio_InterruptGlobalEnable(GpioInstancePtr);//使GPIO能夠中斷
	XGpio_InterruptEnable(&BTN, BTN_INT);//開啟GPIO中斷
}

void Intr_Handler()//ISR函式
{
	delay(500);//延遲0.5秒代替防彈跳
	XGpio_InterruptDisable(&BTN, BTN_INT);//關閉中斷
	XGpio_InterruptClear(&BTN, BTN_INT);//中斷標誌清除
	Intr_CTN++;//中斷次數計數
	printf("Interrupt :%x\n",Intr_CTN);

	delay(2000);//延遲當前LED燈2秒
	LED_num = 0b00000000;//LED清0

	XGpio_InterruptEnable(&BTN, BTN_INT);//開啟特定GPIO通道中斷
}

void delay(int dly) //延遲函式
{
	int i, j;
	for(i = 0;i < dly;i ++){
		for (j = 0;j <0xffff; j++){
			;
		}
	}
}
