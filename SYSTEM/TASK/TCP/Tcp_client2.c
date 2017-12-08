#include "TCP_user.h"

#define IP1 192
#define IP2 168
#define IP3 1
#define IP4 2

#define RemotePort_NUM 1001

/* 这个是本地端口 */
#define LocalPort_NUM 1025

TCP_status TCP_c2;

void TcpClientTest2(void)
{
	uint8_t socket_status = 0;
  uint8_t connect_status = 0;
  
  uint32_t maxsend_size = 0;
  int32_t send_size = 0;
  
  uint8_t* sendbuf;
  static uint32_t count = 0;
  
  TCP_c2.mode = TCP_CLIENT;/*tcp 模式*/
  
  TCP_c2.client.ip[0] = IP1;/*远程ip*/
  TCP_c2.client.ip[1] = IP2;/*远程ip*/
  TCP_c2.client.ip[2] = IP3;/*远程ip*/
  TCP_c2.client.ip[3] = IP4;/*远程ip*/

  TCP_c2.client.local_port = LocalPort_NUM;
  TCP_c2.client.remote_port = RemotePort_NUM;
  
  /* tcp  socket 句柄 */                  /* tcp 客户端        一直连接 */
  TCP_c2.socket_fp = tcp_get_socket (TCP_TYPE_CLIENT|TCP_TYPE_KEEP_ALIVE, 0, 1, tcp_callback);
  
	if(TCP_c2.socket_fp != 0)
	{
    /*创建socket tcp链接， ip地址，远程端口号，自己端口号*/
    socket_status = tcp_connect (TCP_c2.socket_fp, TCP_c2.client.ip, TCP_c2.client.remote_port, TCP_c2.client.local_port);
    if(socket_status == __TRUE)
    {
      u1_printf("tcp socket 创建成功\r\n");   
    }
    else
    {
      u1_printf("tcp socket 创建失败\r\n");   
    } 
	}
	
	while (1) 
	{
		/* RL-TCPnet处理函数(需要一直调用，作用不明！？) */
		main_TcpNet();
    
    /*检测 tcp 客户端链接状态*/
    connect_status = TCP_StatusCheck(TCP_c2);
    
    if((tcp_check_send (TCP_c2.socket_fp) == __TRUE)&&(connect_status == TCP_STATE_CONNECT)) /*判断是否能发送*/
    {
      main_TcpNet();
         
      uint8_t sendbuf_temp[100];
      uint32_t send_pt = 0;
      
      send_size = snprintf((char*)sendbuf_temp,100,"客户端2 向外发送数据--2222--： %d\r\n",count);
      count++;
      
      do
      {
        maxsend_size = tcp_max_dsize (TCP_c2.socket_fp);/*获取最大能发送字节*/
        
        send_size -= maxsend_size;
        
        if(send_size > 0)/*发送长度，大于最大能发送长度*/
        {
          sendbuf = tcp_get_buf(maxsend_size); 

          memcpy(sendbuf,sendbuf_temp + send_pt,maxsend_size);
          send_pt += maxsend_size;
          
          tcp_send (TCP_c2.socket_fp, sendbuf, maxsend_size);/* 测试发现只能使用获取的内存 */ 
        }
        else/*发送长度，小于最大能发送长度*/
        {
          sendbuf = tcp_get_buf(send_size + maxsend_size);
          
          memcpy(sendbuf,sendbuf_temp + send_pt,send_size + maxsend_size);
          
          tcp_send (TCP_c2.socket_fp, sendbuf, send_size + maxsend_size);/* 测试发现只能使用获取的内存 */ 
        } 
      }
      while(send_size > 0);/*还有未发送数据，继续发送*/
      
    }
    vTaskDelay(1000);
	}
}
