#include "UDP_user.h"


/*
*********************************************************************************************************
*	                          �궨�壬Զ�̷�������IP�Ͷ˿�
*********************************************************************************************************
*/
/* Ҫ���ʵ�Զ�̷�����IP�Ͷ˿����ã�Ҳ���ǵ��Զ˵����������õ�IP�Ͷ˿ں� */
#define IP1            192
#define IP2            168
#define IP3            1
#define IP4            2

#define PORT_NUM         1001

/* ����Ǳ��ض˿� */
#define LocalPort_NUM    1024


/*
*********************************************************************************************************
*	                                     ����
*********************************************************************************************************
*/
extern LOCALM localm[]; 
uint8_t udp_soc;
uint8_t DHCP_Status = __FALSE;
uint8_t CacheARP_Status = __FALSE;
uint8_t Rem_IP[4] = {IP1,IP2,IP3,IP4};


/*
*********************************************************************************************************
*	�� �� ��: DCHP_ARP_Check
*	����˵��: ���DHCP�Ƿ��ȡ��IP��ַ�Լ�UDPͨ��Ҫ���ʵ�Զ��IP��ַ�Ƿ���Խ�������Ӧ��MAC��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DCHP_ARP_Check(void)
{
	if(DHCP_Status == __FALSE)	
	{
		if(mem_test(localm[NETIF_ETH].IpAdr, 0, 4) == __FALSE) 
		{
			DHCP_Status = __TRUE;
			u1_printf("DHCP�Ѿ���ʼ���ɹ���ע��δʹ��DHCPҲ�Ƿ��سɹ���\r\n");		  
		}
	}

	if(CacheARP_Status == __FALSE)	
	{
		if(arp_cache_ip (Rem_IP, ARP_FIXED_IP) == __FALSE) 
		{
			CacheARP_Status = __TRUE;
			u1_printf("ͨ��IP��ַ���Խ�����MAC\r\n");		  			
		}
	}	
}

/*
*********************************************************************************************************
*	�� �� ��: tcp_callback
*	����˵��: TCP Socket�Ļص�����
*	��    ��: socket   UDP Socket����
*             remip    Զ���豸��IP��ַ
*             remport  Զ���豸�Ķ˿ں�
*             buf      Զ���豸���������ݵ�ַ
*             len      Զ���豸���������ݳ��ȣ���λ�ֽ�
*	�� �� ֵ: Ĭ�Ϸ���0���ɣ�һ���ò���
*********************************************************************************************************
*/
U16 udp_callback(U8 socket, U8 *remip, U16 remport, U8 *buf, U16 len) 
{
	char buffer[50];
	U16 i;

	/* ȷ����udp_soc�Ļص� */
	if (socket != udp_soc)
	{
		return (0);
	}

	/* ����Ϣ��Զ��IP����ӡIP�Ͷ˿ں� */
	sprintf(buffer, "Զ������IP: %d.%d.%d.%d", remip[0], remip[1], remip[2], remip[3]);
	u1_printf("%s  port:%d\r\n", buffer, remport);

	/* ���յ������ݳ��ȣ���λ�ֽ� */
	u1_printf("Data length = %d\r\n", len);
	/* ��ӡ���յ�����Ϣ */
	for(i = 0; i < len; i++)
	{
		u1_printf("buf[%d] = %d\r\n", i, buf[i]);
	}

	return (0);
}

/*
*********************************************************************************************************
*	�� �� ��: TCPnetTest
*	����˵��: TCPnetӦ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void UDPnetTest(void)
{  
	uint8_t *sendbuf;

	/* ��ȡһ��UDP Socket  */
	udp_soc = udp_get_socket (0, UDP_OPT_SEND_CS | UDP_OPT_CHK_CS, udp_callback);
	if (udp_soc != 0) 
	{
		/* ��UDP�˿ں� */
		udp_open (udp_soc, LocalPort_NUM);
	}
	
	while (1) 
	{
		/* RL-TCPnet������ */
		main_TcpNet();
		
		/* ���DHCP�Ƿ��ȡ��IP��ַ�Լ�UDPͨ��Ҫ���ʵ�Զ��IP��ַ�Ƿ���Խ�������Ӧ��MAC */
		DCHP_ARP_Check();

    main_TcpNet();

    /* ����8�ֽڵĿռ� */
    sendbuf = udp_get_buf (8);
  
    if(sendbuf != NULL)
    {
      /* ��ʼ��8���ֽڱ��� */
      sendbuf[0] = '1';
      sendbuf[1] = '2';
      sendbuf[2] = '3';
      sendbuf[3] = '4';
      sendbuf[4] = '5';
      sendbuf[5] = '6';
      sendbuf[6] = '7';
      sendbuf[7] = '8';
      
      udp_send (udp_soc, Rem_IP, PORT_NUM, sendbuf, 8);
    }

    vTaskDelay(1000);
	}
}

