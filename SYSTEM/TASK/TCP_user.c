#include "TCP_user.h"
#include "stm32f4xx.h"
#include "usr_FreeRTOS.h"

void TcpInit(void)
{
  
}

void TcpNetTest(void)
{
  int32_t iCount;
	uint8_t *sendbuf;
	uint8_t tcp_status;
	uint16_t maxlen;
	uint8_t res;
	const TickType_t xTicksToWait = 2; /* ?¨®3¨´2ms */
	EventBits_t uxBits;
	
	/* 
	   ¡ä¡ä?¡§TCP Socket2¡é¡ä¡ä?¡§?¨¤¨¬y¡ê??¨ª?¡ì??¨¢??¨®¡¤t???¡Âo¨®¡ê?10???¨²?T¨ºy?Y¨ª¡§D??????a¨¢??¨®?¡ê
	   ¦Ì?¨º?¨®¨¦¨®¨²?a¨¤?¨º1?¨¹¨¢?TCP_TYPE_KEEP_ALIVE¡ê??¨¢¨°??¡À¡À¡ê3?¨¢??¨®¡ê?2?¨º¨¹10??¦Ì?¨º¡À???T???¡ê
	*/
    socket_tcp = tcp_get_socket (TCP_TYPE_SERVER|TCP_TYPE_KEEP_ALIVE, 0, 10, tcp_callback);
	if(socket_tcp != 0)
	{
		res = tcp_listen (socket_tcp, PORT_NUM);
		printf_debug("tcp listen res = %d\r\n", res);
	}
	
	while (1) 
	{
		/* RL-TCPnet¡ä|¨¤¨ªo¡¥¨ºy */
		main_TcpNet();
		
		/* ¨®?¨®¨²¨ª???2?¡ã?¦Ì?¡ä|¨¤¨ª */
		tcp_status = TCP_StatusCheck();

		/* ¦Ì¨¨¡äy?¨´¨®D¨¨???¡¤¡é¨¤¡ä¨º??t¡À¨º?? */
		uxBits = xEventGroupWaitBits(xCreatedEventGroup, /* ¨º??t¡À¨º??¡Á¨¦??¡À¨² */
							         0xFFFF,       		 /* ¦Ì¨¨¡äy0xFFFF?3¨°???¡À?¨¦¨¨?? */
							         pdTRUE,             /* ¨ª?3??¡ã0xFFFF??¡À???3y¡ê??a¨¤?¨º?¨¨?¨°a0xFFFF??¡À?¨¦¨¨???¨ª?¡ã¨ª?3??¡À*/
							         pdFALSE,            /* ¨¦¨¨???apdTRUE¡À¨ª¨º?¦Ì¨¨¡äy0xFFFF¨¨?¨°a??¡À?¨¦¨¨??*/
							         xTicksToWait); 	 /* ¦Ì¨¨¡äy?¨®3¨´¨º¡À?? */

		if((uxBits != 0)&&(tcp_status == __TRUE))
		{
			switch (uxBits)
			{
				/* ?¨®¨º?¦Ì?K1?¨¹¡ã¡ä??¡ê?????3¨¬TCP?¨ª?¡ì??¡¤¡é?¨ª8¡Á??¨²¨ºy?Y */
				case KEY1_BIT0:			  
					printf_debug("tcp_get_state(socket_tcp) = %d\r\n", tcp_get_state(socket_tcp));
					iCount = 8;
					do
					{
						main_TcpNet();
						if (tcp_check_send (socket_tcp) == __TRUE) 
						{
							maxlen = tcp_max_dsize (socket_tcp);
							iCount -= maxlen;
							
							if(iCount < 0)
							{
								/* ?a?¡ä???????¨º¨¬a¦Ì? */
								maxlen = iCount + maxlen;
							}
							
							sendbuf = tcp_get_buf(maxlen);
							sendbuf[0] = '1';
							sendbuf[1] = '2';
							sendbuf[2] = '3';
							sendbuf[3] = '4';
							sendbuf[4] = '5';
							sendbuf[5] = '6';
							sendbuf[6] = '7';
							sendbuf[7] = '8';
							
							/* 2a¨º?¡¤¡é?????¨¹¨º1¨®???¨¨?¦Ì??¨²¡ä? */
							tcp_send (socket_tcp, sendbuf, maxlen);
						}
						
					}while(iCount > 0);
					break;

				/* ?¨®¨º?¦Ì?K2?¨¹¡ã¡ä??¡ê?????3¨¬TCP?¨ª?¡ì??¡¤¡é?¨ª1024¡Á??¨²¦Ì?¨ºy?Y */
				case KEY2_BIT1:		
					printf_debug("tcp_get_state(socket_tcp) = %d\r\n", tcp_get_state(socket_tcp));
					iCount = 1024;
					do
					{
						main_TcpNet();
						if (tcp_check_send (socket_tcp) == __TRUE) 
						{
							maxlen = tcp_max_dsize (socket_tcp);
							iCount -= maxlen;
							
							if(iCount < 0)
							{
								/* ?a?¡ä???????¨º¨¬a¦Ì? */
								maxlen = iCount + maxlen;
							}
							
							/* ?a¨¤???3?¨º??¡¥¨¢???¡ä??¨´¡¤¡é?¨ª¨ºy?Y¡ã¨¹¦Ì??¡ã8??¡Á??¨² */
							sendbuf = tcp_get_buf(maxlen);
							sendbuf[0] = 'a';
							sendbuf[1] = 'b';
							sendbuf[2] = 'c';
							sendbuf[3] = 'd';
							sendbuf[4] = 'e';
							sendbuf[5] = 'f';
							sendbuf[6] = 'g';
							sendbuf[7] = 'h';
							
							/* 2a¨º?¡¤¡é?????¨¹¨º1¨®???¨¨?¦Ì??¨²¡ä? */
							tcp_send (socket_tcp, sendbuf, maxlen);
						}
						
					}while(iCount > 0);					
					break;
					
				/* ?¨®¨º?¦Ì?K3?¨¹¡ã¡ä??¡ê?????3¨¬TCP?¨ª?¡ì??¡¤¡é?¨ª5MB¨ºy?Y */
				case KEY3_BIT2:			  
					printf_debug("tcp_get_state(socket_tcp) = %d\r\n", tcp_get_state(socket_tcp));
					iCount = 5*1024*1024;
					do
					{
						main_TcpNet();
						if (tcp_check_send (socket_tcp) == __TRUE) 
						{
							maxlen = tcp_max_dsize (socket_tcp);
							iCount -= maxlen;
							
							if(iCount < 0)
							{
								/* ?a?¡ä???????¨º¨¬a¦Ì? */
								maxlen = iCount + maxlen;
							}
							
							/* ?a¨¤???3?¨º??¡¥¨¢???¡ä??¨´¡¤¡é?¨ª¨ºy?Y¡ã¨¹¦Ì??¡ã8??¡Á??¨² */
							sendbuf = tcp_get_buf(maxlen);
							sendbuf[0] = 'a';
							sendbuf[1] = 'b';
							sendbuf[2] = 'c';
							sendbuf[3] = 'd';
							sendbuf[4] = 'e';
							sendbuf[5] = 'f';
							sendbuf[6] = 'g';
							sendbuf[7] = 'h';
							
							/* 2a¨º?¡¤¡é?????¨¹¨º1¨®???¨¨?¦Ì??¨²¡ä? */
							tcp_send (socket_tcp, sendbuf, maxlen);
						}
						
					}while(iCount > 0);
					break;
				
				 /* ????¦Ì??¨¹?¦Ì2?¡ä|¨¤¨ª */
				default:                     
					break;
			}
		}
	}
}
