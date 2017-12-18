#include "TCP_user.h"



/* 要访问的远程服务器IP和端口配置，也就是电脑端调试助手设置的IP和端口号 */
#define IP1            192
#define IP2            168
#define IP3            2
#define IP4            145

#define PORT_NUM       1001


/*
*********************************************************************************************************
*	                                     变量
*********************************************************************************************************
*/
/* RL-TCPnet API的返回值 */
const static char * ReVal_Table[]= 
{
	" 0: SCK_SUCCESS       Success                             ",
	"-1: SCK_ERROR         General Error                       ",
	"-2: SCK_EINVALID      Invalid socket descriptor           ",
	"-3: SCK_EINVALIDPARA  Invalid parameter                   ",
	"-4: SCK_EWOULDBLOCK   It would have blocked.              ",
	"-5: SCK_EMEMNOTAVAIL  Not enough memory in memory pool    ",
	"-6: SCK_ECLOSED       Connection is closed or aborted     ",
	"-7: SCK_ELOCKED       Socket is locked in RTX environment ",
	"-8: SCK_ETIMEOUT      Socket, Host Resolver timeout       ",
	"-9: SCK_EINPROGRESS   Host Name resolving in progress     ",
	"-10: SCK_ENONAME      Host Name not existing              ",
};

static uint8_t sendbuf[1024];


/*
*********************************************************************************************************
*	函 数 名: TCPnetTest
*	功能说明: TCPnet应用
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void BSDSocketClientTest(void)
{  
	char dbuf[10];
	int len;
	int sock, res;
	unsigned long sck_mode;
	SOCKADDR_IN addr;
	
	
	while (1) 
	{
		/* 创建一个socket 
		   第1个参数AF_INET：当前仅支持这个类型的地址族。
		   第2个参数SOCK_STREAM：表示数据流通信类型，即使用的TCP。
		   第3个参数0 ：配置为0的话，自动跟第2个参数进行协议匹配，这里就是TCP协议。
		*/
		sock = socket (AF_INET, SOCK_STREAM, 0);
		
		/* 设置使能KEEP ALIVE，让客户端和服务器保存连接 */
		sck_mode = 1;
		res = ioctlsocket (sock, FIO_KEEP_ALIVE, &sck_mode);
		if (res == SCK_SUCCESS) 
		{
			u1_printf("KEEP ALIVE设置成功\r\n");
		}
		else 
		{
			u1_printf("KEEP ALIVE设置失败\r\n");
		}

		/* 端口号设置为1001 */
		addr.sin_port = htons(PORT_NUM);
		
		/* 与函数socket中的AF_INET作用一样 */
		addr.sin_family = PF_INET;
		
		addr.sin_addr.s_b1 = IP1;
		addr.sin_addr.s_b2 = IP2;
		addr.sin_addr.s_b3 = IP3;
		addr.sin_addr.s_b4 = IP4;

		/* 客户端连接远程服务器，如果远程服务器还未创建，此函数会立即返回 */
		res = connect (sock, (SOCKADDR *)&addr, sizeof (addr));
		u1_printf("客户端连接远程服务器状态%s\r\n", ReVal_Table[abs(res)]);

		while (1) 
		{
			/*
			  socket数据接收函数，如果recv工作在阻塞模式，使用这个函数注意以下事项：
			  1. 此函数的溢出时间受到Net_Config.c中宏定义 BSD_RCVTOUT 的限制。溢出时间到会自动退出。
			  2. 这个函数接收到一次数据包就会返回，大于或者小于设置的缓冲区大小都没有关系，如果数据量
			     大于接收缓冲区大小，用户只需多次调用函数recv进行接收即可。
			  3. 实际接收到数据大小通过判断此函数的返回值即可。
			*/
			res = recv (sock, dbuf, sizeof(dbuf), 0);
			if (res <= 0) 
			{
				u1_printf("接收函数返回状态%s\r\n", ReVal_Table[abs(res)]);
				if((res == SCK_EINVALID) || (res == SCK_ECLOSED))
				{
					break;	
				}
			}
			else
			{
				u1_printf("Receive Data Length = %d\r\n", res);
				switch(dbuf[0])
				{
					/* 字符命令 1 */
					case '1':
						sendbuf[0] = '1';
						sendbuf[1] = '2';
						sendbuf[2] = '3';
						sendbuf[3] = '4';
						sendbuf[4] = '5';
						sendbuf[5] = '6';
						sendbuf[6] = '7';
						sendbuf[7] = '8';
						sendbuf[8] = '\r';
						sendbuf[9] = '\n';						
						res = send (sock, (char *)sendbuf, 10, 0);
						if (res < 0) 
						{
							u1_printf("函数send发送数据失败\r\n");
						}
						else
						{
							u1_printf("函数send发送数据成功\r\n");							
						}
						break;
					
					/* 字符命令 2 */
					case '2':
						/* 将数据缓冲区清成字符0，方便网络调试助手查看数据 */
						len = sizeof(sendbuf);
						memset(sendbuf, 48, len);
					
						/* 这里仅初始化了数据包的前4个字节和最后4个字节 */
						sendbuf[0] = 'a';
						sendbuf[1] = 'b';
						sendbuf[2] = 'c';
						sendbuf[3] = 'd';
						sendbuf[len - 4] = 'e';
						sendbuf[len - 3] = 'f';
						sendbuf[len - 2] = 'g';
						sendbuf[len - 1] = 'h';					
						res = send (sock, (char *)sendbuf, len, 0);
						if (res < 0) 
						{
							u1_printf("函数send发送数据失败%s\r\n", ReVal_Table[abs(res)]);
						}
						else
						{
							u1_printf("函数send成功发送数据 = %d字节\r\n", res);							
						}
						break;
				
					/* 其它数值不做处理 */
					default:                     
						break;
				}
			}

		}
		
		/* 
		   远程服务器断开连接和sock句柄无效，程序都会执行到这里，我们在这里关闭socket，
		   程序返回到第一个大while循环的开头重新创建socket并连接。
		*/
		closesocket (sock);
	}
}

