/************************************************************************
  Title     : Robot Body Protocol Source File
  File name : robot_protocol.c    

  Author    : adc inc. (oxyang@adc.co.kr)
  History
		+ v0.0  2007/2/14
		+ v1.0  2008/8/6
************************************************************************/
#include <stdio.h>
#include <string.h>
#include "robot_protocol.h"
#include "uart_api.h"
//////////////////////////////////////////////////// Protocol Test

void DelayLoop(int delay_time)
{
	while(delay_time)
		delay_time--;
}

void Send_Command(unsigned char Ldata, unsigned char Ldata1)
{
	unsigned char Command_Buffer[6] = {0,};

	Command_Buffer[0] = START_CODE;	// Start Byte -> 0xff
	Command_Buffer[1] = START_CODE1; // Start Byte1 -> 0x55
	Command_Buffer[2] = Ldata;
	Command_Buffer[3] = Ldata1;
	Command_Buffer[4] = Hdata;  // 0x00
	Command_Buffer[5] = Hdata1; // 0xff

	uart1_buffer_write(Command_Buffer, 6);
}

#define ERROR	0
#define OK	1
void Stand(){
	Send_Command(0x01, 0xfe);
}
void Right(){
	Send_Command(0x02, 0xfd);
}

void Left(){
	Send_Command(0x03, 0xfc);
}

void Turn_left(){
	Send_Command(0x04, 0xfb);
}

void Turn_Right(){
	Send_Command(0x05, 0xfa);
}

void Left_Walk(){
	Send_Command(0x08, 0xf7);
}

void Right_Walk(){
	Send_Command(0x09, 0xf6);
}

void Right_Punch(){
	Send_Command(0x0d, 0xf2);
}

void Left_Punch(){
	Send_Command(0x14, 0xeb);
}

void Right_BackPunch(){
	Send_Command(0x15, 0xea);
}

void Left_BackPunch(){
	Send_Command(0x16, 0xe9);
}

void Right_BackThrow(){
	Send_Command(0x17, 0xe8);
}

void Left_BackThrow(){
	Send_Command(0x18, 0xe7);
}

void Shadow_Punch(){
	Send_Command(0x19, 0xe6);
}

void Push(){
	Send_Command(0x20, 0xdf);
}

void Left_Punch_Walk(){
	Send_Command(0x22, 0xdd);
}

void Right_Punch_Walk(){
	Send_Command(0x23, 0xdc);
}

void Left_Head_Punch(){
	Send_Command(0x24, 0xdb);
}

void Right_Head_Punch(){
	Send_Command(0x25, 0xda);
}
