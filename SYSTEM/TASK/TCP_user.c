#include "TCP_user.h"
#include "stm32f4xx.h"
#include "usr_FreeRTOS.h"
#include "usart.h"

uint8_t socket_tcp;
#define PORT_NUM       1001    /* TCP服务器监听端口号 */

void TcpInit(void)
{
  
}

U16 tcp_callback (U8 soc, U8 evt, U8 *ptr, U16 par)
{
	char buf[100];
	uint16_t i;
  uint32_t num;
	
	/* 确保是socket_tcp的回调 */
	if (soc != socket_tcp) 
	{
		return (0);
	}

	switch (evt) 
	{
		/*
			远程客户端连接消息
		    1、数组ptr存储远程设备的IP地址，par中存储端口号。
		    2、返回数值1允许连接，返回数值0禁止连接。
		*/
		case TCP_EVT_CONREQ:
//			num = snprintf(buf,100,"远程客户端请求连接IP: %d.%d.%d.%d\r\n端口port:%d\r\n", ptr[0], ptr[1], ptr[2], ptr[3],par);
//    
//      USART_SetSendData(USART1, buf, num);
			return (1);
		
		/* 连接终止 */
		case TCP_EVT_ABORT:
			break;
		
		/* Socket远程连接已经建立 */
		case TCP_EVT_CONNECT:
      
//      num = snprintf(buf,100,"Socket is connected to remote peer\r\n");
//			USART_SetSendData(USART1, buf, num);
			break;
		
		/* 连接断开 */
		case TCP_EVT_CLOSE:     
//      num = snprintf(buf,100,"Connection has been closed\r\n");
//			USART_SetSendData(USART1, buf, num);
			break;
		
		/* 发送的数据收到远程设备应答 */
		case TCP_EVT_ACK:
			break;
		
		/* 接收到TCP数据帧，ptr指向数据地址，par记录数据长度，单位字节 */
		case TCP_EVT_DATA:
//      num = snprintf(buf,100,"Data length = %d\r\n", par);
//			USART_SetSendData(USART1, buf, num);  
//    
//			for(i = 0; i < par; i++)
//			{
//        num = snprintf(buf,100,"ptr[%d] = %d\r\n", i, ptr[i]);
//        USART_SetSendData(USART1, buf, num); 
//			}
			break;
	}
	return (0);
}

uint8_t TCP_StatusCheck(void) 
{
	uint8_t res;
	
	switch (tcp_get_state(socket_tcp)) 
	{
		case TCP_STATE_FREE:
		case TCP_STATE_CLOSED:
			res = tcp_listen (socket_tcp, PORT_NUM);
      char buf[50];
      uint32_t num;
//      num = snprintf(buf,100,"tcp listen res = %d\r\n", res);
//      USART_SetSendData(USART1, buf, num); 
			break;
		
		case TCP_STATE_LISTEN:
			break;
		
		case TCP_STATE_CONNECT:
			return (__TRUE);
			
		default:  
			break;
	}
	
	return (__FALSE);
}

void TcpNetTest(void)
{
  char buf[100];
  uint32_t num;
  
	uint8_t tcp_status;
	uint16_t maxlen;
	uint8_t res;
	const TickType_t xTicksToWait = 2; 
	
  socket_tcp = tcp_get_socket (TCP_TYPE_SERVER|TCP_TYPE_KEEP_ALIVE, 0, 10, tcp_callback);
  
	if(socket_tcp != 0)
	{
		res = tcp_listen (socket_tcp, PORT_NUM);   
//    num = snprintf(buf,100,"tcp listen res = %d\r\n", res);
//    USART_SetSendData(USART1, buf, num); 
	}
	
	while (1) 
	{
		/* RL-TCPnet处理函数 */
		main_TcpNet();
		
////		/* 用于网线插拔的处理 */
//		tcp_status = TCP_StatusCheck();

	}
}
