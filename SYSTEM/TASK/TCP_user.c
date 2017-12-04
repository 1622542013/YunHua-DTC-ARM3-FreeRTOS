#include "TCP_user.h"

/*TCP server socket 回调函数
参数1：TCP socket 句柄
参数2：事件类型
    TCP_EVT_CONREQ   远程客户端连接消息。
    TCP_EVT_CONNECT  连接成功消息。
    TCP_EVT_CLOSE    连接断开消息。
    TCP_EVT_ABORT    连接终止消息。
    TCP_EVT_ACK      发送数据后，收到远程主机的应答。
    TCP_EVT_DATA     接收到 TCP 数据包。
参数3：
参数4：
    当参数2，输入类型是 TCP_EVT_DATA：
        参数3 ptr 指向的是接收到的数据地址
        参数4 par 是接收到数据的个数
    当参数2，输入类型 “不是”TCP_EVT_DATA，即其他事件类型：
        参数3 ptr 是 远程IP
        参数4 par 是远程端口号    
*/
U16 tcpserver_callback (U8 soc, U8 evt, U8 *ptr, U16 par)
{
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
      u1_printf("----连接终止\r\n");
			break;
		
		/* Socket远程连接已经建立 */
		case TCP_EVT_CONNECT:
      u1_printf("----Socket远程连接已经建立\r\n");
			break;
		
		/* 连接断开 */
		case TCP_EVT_CLOSE: 
      u1_printf("----连接断开\r\n");      
			break;
		
		/* 发送的数据收到远程设备应答 */
		case TCP_EVT_ACK:
  //   u1_printf("----发送的数据收到远程设备应答\r\n");     
			break;
		
		/* 接收到TCP数据帧，ptr指向数据地址，par记录数据长度，单位字节 */
		case TCP_EVT_DATA:
      u1_printf("接收到TCP数据帧，ptr指向数据地址，数据长度 = %d\r\n", par);

      USART_OUT(USART_1, (uint8_t*)ptr, par);    
	}
	return (0);
}

uint8_t TCP_Client_StatusCheck(uint8_t socket_fp,uint8_t ip[4],uint16_t server_portnum,uint16_t local_port_num) 
{	
	switch (tcp_get_state(socket_fp)) 
	{
		case TCP_STATE_FREE:
		case TCP_STATE_CLOSED:
			tcp_connect (socket_fp, ip, server_portnum, local_port_num);
      u1_printf("尝试重新连接\r\n");
			break;
		
		case TCP_STATE_LISTEN:
			break;
		
		case TCP_STATE_CONNECT:
      u1_printf("TCP连接以建立\r\n");
			return (__TRUE);
			
		default:  
			break;
	}
	
	return (__FALSE);
}


