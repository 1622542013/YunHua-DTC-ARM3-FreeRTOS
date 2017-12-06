#include "TCP_user.h"

#define PORT_NUM       1001    /* TCP服务器监听端口号 */

TCP_status TCP_S;

void TcpServer(void)
{
  uint32_t maxsend_size = 0;
  int32_t send_size = 0;
  
  uint8_t* sendbuf;
  static uint32_t count = 0;
  
  uint8_t connect_status = 0;
  uint8_t socket_status = 0;
  
  TCP_S.mode = TCP_SERVER;
  TCP_S.server.local_port = PORT_NUM;
  
  TCP_S.socket_fp = tcp_get_socket (TCP_TYPE_SERVER | TCP_TYPE_KEEP_ALIVE, 0, 10, tcp_callback);
  
  if(TCP_S.socket_fp != 0)
  {
      socket_status = tcp_listen (TCP_S.socket_fp, PORT_NUM);
    
      if(socket_status == __TRUE)
      {
        u1_printf("tcp 监听 成功\r\n");
      }
      else
      {
        u1_printf("tcp 监听 失败\r\n");
      }     
  }
  
  while (1) 
	{
		/* RL-TCPnet处理函数(需要一直调用，作用不明！？) */
		main_TcpNet();
    
    /*检测 tcp 客户端链接状态*/
    connect_status = TCP_StatusCheck(TCP_S);
    
    if((tcp_check_send (TCP_S.socket_fp) == __TRUE)&&(connect_status == TCP_STATE_CONNECT)) /*判断是否能发送*/
    {
      main_TcpNet();
      
      uint8_t sendbuf_temp[100];
      uint32_t send_pt = 0;
      
      send_size = snprintf((char*)sendbuf_temp,100,"客户端1 向外发送数据--1111--： %d\r\n",count);
      count++;
      
      do
      {
        maxsend_size = tcp_max_dsize (TCP_S.socket_fp);/*获取最大能发送字节*/
        
        send_size -= maxsend_size;
        
        if(send_size > 0)/*发送长度，大于最大能发送长度*/
        {
          sendbuf = tcp_get_buf(maxsend_size); 

          memcpy(sendbuf,sendbuf_temp + send_pt,maxsend_size);
          send_pt += maxsend_size;
          
          tcp_send (TCP_S.socket_fp, sendbuf, maxsend_size);/* 测试发现只能使用获取的内存 */ 
        }
        else/*发送长度，小于最大能发送长度*/
        {
          sendbuf = tcp_get_buf(send_size + maxsend_size);
          
          memcpy(sendbuf,sendbuf_temp + send_pt,send_size + maxsend_size);
          
          tcp_send (TCP_S.socket_fp, sendbuf, send_size + maxsend_size);/* 测试发现只能使用获取的内存 */ 
        } 
      }
      while(send_size > 0);/*还有未发送数据，继续发送*/
    }
      
    vTaskDelay(1000);
	}
}
