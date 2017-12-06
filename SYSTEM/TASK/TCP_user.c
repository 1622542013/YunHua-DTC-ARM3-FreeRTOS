#include "TCP_user.h"

/*TCP socket 回调函数
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
U16 tcp_callback (U8 soc, U8 evt, U8 *ptr, U16 par)
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
uint8_t TCP_StatusCheck(TCP_status tcp) 
{	 
  uint8_t status = 0;
  uint16_t  local_port = 0;
  
  if(tcp.mode == TCP_CLIENT)
  {
    local_port = tcp.client.local_port;
  }
  else if(tcp.mode == TCP_SERVER)
  {
    local_port = tcp.server.local_port;
  }
  
  status = tcp_get_state(tcp.socket_fp);
  
	switch(status) 
	{
		case TCP_STATE_FREE:
      u1_printf("tcp 句柄无效\r\n");break;
		case TCP_STATE_CLOSED:
      if(tcp.mode == TCP_CLIENT)
      {
                      /*socket句柄  远程服务器ip    远程服务器端口          本地端口*/
        tcp_connect (tcp.socket_fp, tcp.client.ip, tcp.client.remote_port, tcp.client.local_port);
        u1_printf("TCP 客户端 %d 尝试 重新连接 --> 远程服务端\r\n",tcp.client.local_port);  
      }
      else if(tcp.mode == TCP_SERVER)
      {
        uint8_t res;
        res = tcp_listen (tcp.socket_fp, tcp.client.local_port);
        if(res == __TRUE)
        {
          u1_printf("tcp 服务端 监听 %d端口成功\r\n", tcp.server.local_port);
        }
        else
        {
          u1_printf("tcp 服务端 %d 监听 失败\r\n",tcp.server.local_port);
        }       
      }
      else
      {
        u1_printf("tcp 模式未选择\r\n");
      }
			break;	
      
		case TCP_STATE_LISTEN:   u1_printf("Socket 等待连接中（%d 监听ing）\r\n",local_port);break; 
    case TCP_STATE_SYN_REC:  u1_printf("%d 接收到SYN包\r\n",local_port);break;    
    case TCP_STATE_SYN_SENT: u1_printf("%d 发送SYN包,建立连接\r\n",local_port);break;
    case TCP_STATE_FINW1:    u1_printf("%d 发送FIN包，准备结束连接\r\n",local_port);break;
    case TCP_STATE_FINW2:    u1_printf("%d 发送的 FIN 包已经收到远程机器的应答，当前正在等待远程机器发送 FIN 包\r\n",local_port);break;
    case TCP_STATE_CLOSING:  u1_printf("%d 从远程机器收到 FIN 包。\r\n",local_port);break;
    case TCP_STATE_LAST_ACK: u1_printf("%d 对于已经发出的 FIN 包，最后一次等待 ACK 应答。\r\n",local_port);break;
    case TCP_STATE_TWAIT:    u1_printf("%d 关闭前等待 2ms。\r\n",local_port);break;
    
		case TCP_STATE_CONNECT:  u1_printf("TCP %d 连接已建立，可以通信\r\n",local_port);break;
			
		default:  	break;	
	}
  
  return  status;
}



