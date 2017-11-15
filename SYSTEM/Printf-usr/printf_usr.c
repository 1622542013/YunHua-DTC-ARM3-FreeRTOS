#include "printf_usr.h"
#include "usart.h"

#include <stdio.h>

//void printf_usr(char buff[100])
//{
//  char send_buff[100];
//  uint16_t num;
//  
//  num = snprintf(send_buff,100,buff);
//  USART_SetSendData(USART1, send_buff, num); 
//}

int fputc(int ch, FILE *f)
{ 
  USART_SetSendData(USART1, f, ch); 
}

