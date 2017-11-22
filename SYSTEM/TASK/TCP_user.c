#include "TCP_user.h"
#include "stm32f4xx.h"
#include "usr_FreeRTOS.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

uint8_t socket_tcp;
#define PORT_NUM       1001    /* TCP服务器监听端口号 */

void TcpInit(void)
{
  
}

U16 tcp_callback (U8 soc, U8 evt, U8 *ptr, U16 par)
{
	uint16_t i;
	
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
      u1_printf("远程客户端请求连接IP: %d.%d.%d.%d\r\n端口port:%d\r\n", ptr[0], ptr[1], ptr[2], ptr[3],par);
			return (1);
		
		/* 连接终止 */
		case TCP_EVT_ABORT:
			break;
		
		/* Socket远程连接已经建立 */
		case TCP_EVT_CONNECT:
      u1_printf("Socket远程连接已经建立\r\n");
			break;
		
		/* 连接断开 */
		case TCP_EVT_CLOSE: 
      u1_printf("连接断开\r\n");      
			break;
		
		/* 发送的数据收到远程设备应答 */
		case TCP_EVT_ACK:
			break;
		
		/* 接收到TCP数据帧，ptr指向数据地址，par记录数据长度，单位字节 */
		case TCP_EVT_DATA:
      u1_printf("接收到TCP数据帧，ptr指向数据地址，数据长度 = %d\r\n", par);
          
			for(i = 0; i < par; i++)
			{
        u1_printf("ptr[%d] = %d\r\n", i, ptr[i]);
			}
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
      u1_printf("tcp listen res = %d\r\n", res);
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
uint32_t send_size = 0;
void TcpNetTest(void)
{
	uint8_t res = 0;
  uint32_t maxlen = 0;
  
  uint8_t* sendbuf;
  static uint32_t count = 0;
  
  socket_tcp = tcp_get_socket (TCP_TYPE_SERVER|TCP_TYPE_KEEP_ALIVE, 0, 10, tcp_callback);
  
	if(socket_tcp != 0)
	{
		res = tcp_listen (socket_tcp, PORT_NUM); 
    if(res == __TRUE)
    {
      u1_printf("tcp listen 创建成功\r\n");   
    }
    else
    {
      u1_printf("tcp listen 创建失败\r\n");   
    } 
	}
	
	while (1) 
	{
		/* RL-TCPnet处理函数 */
		main_TcpNet();
    
    if (tcp_check_send (socket_tcp) == __TRUE) /*判断是否能发送*/
    {
      maxlen = tcp_max_dsize (socket_tcp);
      send_size = strlen("tcp向外发送数据 ： %d\r\n");
      
      if(send_size > maxlen)
      {
        send_size = maxlen;
      }
      
      sendbuf = tcp_get_buf(send_size);
      
      send_size = snprintf((char*)sendbuf,100,"tcp向外发送数据 ： %d\r\n",count);
      count++;
      
      /* 测试发现只能使用获取的内存 */
      tcp_send (socket_tcp, sendbuf, send_size);
      
      TickType_t xLastSYSTime;
      vTaskDelay(1000);
    }
	}
}
