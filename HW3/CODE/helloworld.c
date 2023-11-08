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

#include "xparameters.h"//�]�t�B�z�����a�}�P�]��ID�w�q
#include "xgpio.h"		//GPIO�禡�w
#include "xscugic.h"	//GIC�禡�w

#define Z700_INTC_ID	XPAR_PS7_SCUGIC_0_DEVICE_ID 		  // for ZYNQ 700�t�C���_�s��
#define BTN_ID 	XPAR_AXI_GPIO_1_DEVICE_ID					  //�]�t�b"xparameters.h"��
#define LED_ID	XPAR_AXI_GPIO_0_DEVICE_ID					  //�]�t�b"xparameters.h"��
#define INTC_GPIO_ID XPAR_FABRIC_AXI_GPIO_1_IP2INTC_IRPT_INTR //GPIO���_�s�� �]�t�b"xparameters.h"��
#define BTN_INT 		XGPIO_IR_CH1_MASK					  //GPIO���_�q�D
#define GIC_ID XPAR_SCUGIC_SINGLE_DEVICE_ID 				  //GIC�q�Τ��_�s��

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
	init_platform();//�t�Ϊ�l�Ƴ]�w
	XGpio_Initialize(&LED, LED_ID);//��l��GPIO LED�}��
	XGpio_SetDataDirection(&LED, 1, 0);//�q�D1, ��X

	XGpio_Initialize(&BTN, BTN_ID);//��l��GPIO �����}��
	XGpio_SetDataDirection(&BTN, 1, 1);//�q�D1, ��J

	Intr_Setup(&INTCInst, &BTN);//���_��l��
	print("Init successful");

	while(1)
	{
		XGpio_DiscreteWrite(&LED, 1, LED_num);//����LED��X���
		printf("LED_num=%x\n",LED_num);//��ܷ�eLED��X
		LED_num = LED_num +1;//�L�a�j��p��LED
		delay(500);

	}
}

void Intr_Setup(XScuGic *GicInstancePtr, XGpio *GpioInstancePtr)//���_��l�Ƴ]�w
{
	IntcConfig = XScuGic_LookupConfig(GIC_ID);//�M���eSOC�����_�t�m
	XScuGic_CfgInitialize(&INTCInst, IntcConfig, IntcConfig->CpuBaseAddress);//��l�Ƴ]�w��eSOC�����_�t�m

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,(Xil_ExceptionHandler)XScuGic_InterruptHandler, GicInstancePtr);//�s�W���_�B�z�{��
	Xil_ExceptionEnableMask(XIL_EXCEPTION_IRQ);//SOC���_�ҥ�
	XScuGic_Connect(GicInstancePtr, INTC_GPIO_ID, (Xil_ExceptionHandler)Intr_Handler, (void *)GpioInstancePtr);
	//�s�����_�BGPIO���_�B�S�w���_�禡�BGPIO�}��

	XScuGic_Enable(GicInstancePtr, INTC_GPIO_ID);//�Ϥ��_����౵���S�wGPIO���_
	XGpio_InterruptGlobalEnable(GpioInstancePtr);//��GPIO������_
	XGpio_InterruptEnable(&BTN, BTN_INT);//�}��GPIO���_
}

void Intr_Handler()//ISR�禡
{
	delay(500);//����0.5��N�����u��
	XGpio_InterruptDisable(&BTN, BTN_INT);//�������_
	XGpio_InterruptClear(&BTN, BTN_INT);//���_�лx�M��
	Intr_CTN++;//���_���ƭp��
	printf("Interrupt :%x\n",Intr_CTN);

	delay(2000);//�����eLED�O2��
	LED_num = 0b00000000;//LED�M0

	XGpio_InterruptEnable(&BTN, BTN_INT);//�}�үS�wGPIO�q�D���_
}

void delay(int dly) //����禡
{
	int i, j;
	for(i = 0;i < dly;i ++){
		for (j = 0;j <0xffff; j++){
			;
		}
	}
}
