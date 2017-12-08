/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    delay.h
 * Date:         2017.04.19
 * Description:  delay time
*****************************************************************/

/*============================================================================*/
/*                               Header include                               */
/*============================================================================*/

#ifndef __TCP_USER_H_
#define __TCP_USER_H_

#include "stm32f4xx.h"

#include "usr_FreeRTOS.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

/*============================================================================*/
/*                                   Macros                                   */
/*============================================================================*/

#define TCP_SERVER     1
#define TCP_CLIENT     2



/*============================================================================*/
/*                                   Macros                                   */
/*============================================================================*/

#pragma pack(push)
#pragma pack(1)

typedef struct
{
  uint16_t local_port; 
}TCP_Server;

typedef struct
{
  uint16_t local_port; 
  uint16_t remote_port;
  uint8_t ip[4];
}TCP_Client;

typedef struct
{
  uint8_t mode;
  uint8_t socket_fp;
  
  TCP_Client client;
  TCP_Server server;
  
}TCP_status;

#pragma pack(pop)

/*============================================================================*/
/*                               declaration                             */
/*============================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

U16 tcp_callback(U8 soc, U8 evt, U8 *ptr, U16 par);
uint8_t TCP_Client_StatusCheck(uint8_t socket_fp,uint8_t ip[4],uint16_t server_portnum,uint16_t local_port_num);

uint8_t TCP_StatusCheck(TCP_status tcp_sta); 
  
void TcpClientTest1(void);
void TcpClientTest2(void);
void TcpServer(void);
  
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
