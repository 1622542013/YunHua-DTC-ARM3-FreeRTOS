#include "UDP_user.h"


/*
*********************************************************************************************************
*	                          宏定义，远程服务器的IP和端口
*********************************************************************************************************
*/
/* 要访问的远程服务器IP和端口配置，也就是电脑端调试助手设置的IP和端口号 */
#define IP1            192
#define IP2            168
#define IP3            1
#define IP4            2

#define PORT_NUM         1001

/* 这个是本地端口 */
#define LocalPort_NUM    1024


/*
*********************************************************************************************************
*	                                     变量
*********************************************************************************************************
*/
extern LOCALM localm[]; 
uint8_t udp_soc;
uint8_t DHCP_Status = __FALSE;
uint8_t CacheARP_Status = __FALSE;
uint8_t Rem_IP[4] = {IP1,IP2,IP3,IP4};


/*
*********************************************************************************************************
*	函 数 名: DCHP_ARP_Check
*	功能说明: 检测DHCP是否获取了IP地址以及UDP通信要访问的远程IP地址是否可以解析出对应的MAC。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void DCHP_ARP_Check(void)
{
	if(DHCP_Status == __FALSE)	
	{
		if(mem_test(localm[NETIF_ETH].IpAdr, 0, 4) == __FALSE) 
		{
			DHCP_Status = __TRUE;
			u1_printf("DHCP已经初始化成功，注意未使能DHCP也是返回成功的\r\n");		  
		}
	}

	if(CacheARP_Status == __FALSE)	
	{
		if(arp_cache_ip (Rem_IP, ARP_FIXED_IP) == __FALSE) 
		{
			CacheARP_Status = __TRUE;
			u1_printf("通过IP地址可以解析出MAC\r\n");		  			
		}
	}	
}

/*
*********************************************************************************************************
*	函 数 名: tcp_callback
*	功能说明: TCP Socket的回调函数
*	形    参: socket   UDP Socket类型
*             remip    远程设备的IP地址
*             remport  远程设备的端口号
*             buf      远程设备发来的数据地址
*             len      远程设备发来的数据长度，单位字节
*	返 回 值: 默认返回0即可，一般用不上
*********************************************************************************************************
*/
U16 udp_callback(U8 socket, U8 *remip, U16 remport, U8 *buf, U16 len) 
{
	char buffer[50];
	U16 i;

	/* 确保是udp_soc的回调 */
	if (socket != udp_soc)
	{
		return (0);
	}

	/* 发消息的远程IP，打印IP和端口号 */
	sprintf(buffer, "远程连接IP: %d.%d.%d.%d", remip[0], remip[1], remip[2], remip[3]);
	u1_printf("%s  port:%d\r\n", buffer, remport);

	/* 接收到的数据长度，单位字节 */
	u1_printf("Data length = %d\r\n", len);
	/* 打印接收到的消息 */
	for(i = 0; i < len; i++)
	{
		u1_printf("buf[%d] = %d\r\n", i, buf[i]);
	}

	return (0);
}

/*
*********************************************************************************************************
*	函 数 名: TCPnetTest
*	功能说明: TCPnet应用
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void UDPnetTest(void)
{  
	int32_t iCount;
	uint8_t *sendbuf;
	uint8_t res;
	const TickType_t xTicksToWait = 2; /* 延迟2ms */
	EventBits_t uxBits;
	
	
	/* 获取一个UDP Socket  */
	udp_soc = udp_get_socket (0, UDP_OPT_SEND_CS | UDP_OPT_CHK_CS, udp_callback);
	if (udp_soc != 0) 
	{
		/* 打开UDP端口号 */
		udp_open (udp_soc, LocalPort_NUM);
	}
	
	while (1) 
	{
		/* RL-TCPnet处理函数 */
		main_TcpNet();
		
		/* 检测DHCP是否获取了IP地址以及UDP通信要访问的远程IP地址是否可以解析出对应的MAC */
		DCHP_ARP_Check();

    main_TcpNet();

    /* 申请8字节的空间 */
    sendbuf = udp_get_buf (8);
  
    if(sendbuf != NULL)
    {
      /* 初始化8个字节变量 */
      sendbuf[0] = '1';
      sendbuf[1] = '2';
      sendbuf[2] = '3';
      sendbuf[3] = '4';
      sendbuf[4] = '5';
      sendbuf[5] = '6';
      sendbuf[6] = '7';
      sendbuf[7] = '8';
      
      res = udp_send (udp_soc, Rem_IP, PORT_NUM, sendbuf, 8);
    }

    vTaskDelay(1000);
	}
}

