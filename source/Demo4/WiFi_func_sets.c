//  Ⓒ Copyright Tianyi Liu 2019, All Rights Reserved.
//  For reference only.
//  If you use or partially use this repo, you shall formally acknowledge this repo.
//  Latest Update: 15:32 May 31st, 2019 CST

#include "header.h"

char WiFi_Connect_String[50] = "AT+CWJAP=\"IntegratedProject\",\"31053106\"\r\n";
char WiFi_Start_String[50] = "AT+CIPSTART=\"UDP\",\"0\",0,3105,2\r\n";

extern struct loc_Update_struct loc_Update;
extern struct wifi_struct wifi;

void get_loc(void)
{
	GPIO_WriteBit(LED_GPIO, LED_PIN, Bit_SET);
	int flag = 0;
	char buffer[100] = {'\0'};
	int pos = 0;
	for ( ; ; )
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
		wifi.Rcv = (unsigned char)USART_ReceiveData(USART2);
		
		if (flag == 1)
		{
			if (wifi.Rcv == 'M')
				break;
			else if (wifi.Rcv != '\r' && wifi.Rcv != '\n')
				buffer[pos++] = wifi.Rcv;
			else 
				continue;
		}
		else if (wifi.Rcv == ':')
			flag = 1;
	}
	
	char *p = buffer;
	int res = 0;

	for (int len = strlen(buffer); len > 1; len -= 3)
	{
		strncpy (wifi.Rcv_Temp, p, 3);
		if (wifi.Flag_X == 0 && wifi.Flag_Y == 0)
		{
			if (strcmp(wifi.Rcv_Temp, wifi.Tag_Car) == 0)
			{
				loc_Update.object = CAR;
				wifi.Flag_X = 1;
			}
			else if (strcmp(wifi.Rcv_Temp, wifi.Tag_Ball) == 0)
			{
				loc_Update.object = BALL;
				wifi.Flag_X = 1;
			}
		}
		else if (wifi.Flag_X == 1)
		{
			sscanf(wifi.Rcv_Temp, "%x", &res);
			loc_Update.X = res;
			wifi.Flag_X = 0;
			wifi.Flag_Y = 1;
		}
		else if (wifi.Flag_Y == 1)
		{
			sscanf(wifi.Rcv_Temp, "%x", &res);
			loc_Update.Y = res;
			update_loc(loc_Update);
			wifi.Flag_X = 0;
			wifi.Flag_Y = 0;
		}
		p += 3;
	}
	GPIO_WriteBit(LED_GPIO, LED_PIN, Bit_RESET);
}

void WiFi_Connect(void)
{
	GPIO_WriteBit(LED_GPIO, LED_PIN, Bit_SET);
	USART_Send_String(WiFi_Connect_String,sizeof(WiFi_Connect_String));
	for (;;)
	{	
		while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
		wifi.Rcv = (unsigned char)USART_ReceiveData(USART2);
		if (wifi.Rcv == 'I')
		{
			while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
			wifi.Rcv = (unsigned char)USART_ReceiveData(USART2);
			if (wifi.Rcv == 'P')
			{
				wifi.Rcv = 0;
				break;
			}
		}
	}
	USART_Send_String(WiFi_Start_String,sizeof(WiFi_Start_String));
	for (;;)
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
		wifi.Rcv = (unsigned char)USART_ReceiveData(USART2);
		if (wifi.Rcv == '+')
		{	
			wifi.Rcv = 0;
			break;
		}
	}	
	GPIO_WriteBit(LED_GPIO, LED_PIN, Bit_RESET);
}
