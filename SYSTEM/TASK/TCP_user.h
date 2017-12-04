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



/*============================================================================*/
/*                                   Macros                                   */
/*============================================================================*/



/*============================================================================*/
/*                               declaration                             */
/*============================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
  
uint8_t TCP_StatusCheck(uint8_t socket_fp);
U16 tcpserver_callback(U8 soc, U8 evt, U8 *ptr, U16 par);
uint8_t TCP_Client_StatusCheck(uint8_t socket_fp,uint8_t ip[4],uint16_t server_portnum,uint16_t local_port_num);
  
void TcpClientTest1(void);
void TcpClientTest2(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
