#include "TCP_user.h"

#define IP1 192
#define IP2 168
#define IP3 1
#define IP4 2

#define PORT_NUM 1001

/* 这个是本地端口 */
#define LocalPort_NUM 1024

void TcpClientTest1(void)
{
	uint8_t res = 0;
  uint32_t maxlen = 0;
  uint32_t send_size = 0;
  uint8_t* sendbuf;
  static uint32_t count = 0;

  uint8_t Rem_IP[4] = {IP1,IP2,IP3,IP4};/*远程ip*/
  uint8_t socket_tcp_fp;/*socket 句柄*/
  
                                 /* tcp 客户端        一直连接 */
  socket_tcp_fp = tcp_get_socket (TCP_TYPE_CLIENT|TCP_TYPE_KEEP_ALIVE, 0, 1, tcpserver_callback);
  
	if(socket_tcp_fp != 0)
	{
    /*创建socket tcp链接， ip地址，远程端口号，自己端口号*/
    res = tcp_connect (socket_tcp_fp, Rem_IP, PORT_NUM, LocalPort_NUM);
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
		/* RL-TCPnet处理函数(需要一直调用，作用不明！？) */
		main_TcpNet();
    
    /*检测 tcp 客户端链接状态*/
    TCP_Client_StatusCheck(socket_tcp_fp,Rem_IP,PORT_NUM,LocalPort_NUM);
    
    if (tcp_check_send (socket_tcp_fp) == __TRUE) /*判断是否能发送*/
    {
      main_TcpNet();
      maxlen = tcp_max_dsize (socket_tcp_fp);/*获取最大能发送字节*/
      send_size = strlen("tcp向外发送数据 ： %d\r\n");
      
      if(send_size > maxlen)
      {
        send_size = maxlen;
      }
      
      sendbuf = tcp_get_buf(send_size);
      
      send_size = snprintf((char*)sendbuf,100,"客户端1 向外发送数据 1111： %d\r\n",count);
      count++;
      
      /* 测试发现只能使用获取的内存 */
      tcp_send (socket_tcp_fp, sendbuf, send_size);     
    }
    vTaskDelay(1000);
	}
}
