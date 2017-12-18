#include "TCP_user.h"



/* Ҫ���ʵ�Զ�̷�����IP�Ͷ˿����ã�Ҳ���ǵ��Զ˵����������õ�IP�Ͷ˿ں� */
#define IP1            192
#define IP2            168
#define IP3            2
#define IP4            145

#define PORT_NUM       1001


/*
*********************************************************************************************************
*	                                     ����
*********************************************************************************************************
*/
/* RL-TCPnet API�ķ���ֵ */
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
*	�� �� ��: TCPnetTest
*	����˵��: TCPnetӦ��
*	��    ��: ��
*	�� �� ֵ: ��
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
		/* ����һ��socket 
		   ��1������AF_INET����ǰ��֧��������͵ĵ�ַ�塣
		   ��2������SOCK_STREAM����ʾ������ͨ�����ͣ���ʹ�õ�TCP��
		   ��3������0 ������Ϊ0�Ļ����Զ�����2����������Э��ƥ�䣬�������TCPЭ�顣
		*/
		sock = socket (AF_INET, SOCK_STREAM, 0);
		
		/* ����ʹ��KEEP ALIVE���ÿͻ��˺ͷ������������� */
		sck_mode = 1;
		res = ioctlsocket (sock, FIO_KEEP_ALIVE, &sck_mode);
		if (res == SCK_SUCCESS) 
		{
			u1_printf("KEEP ALIVE���óɹ�\r\n");
		}
		else 
		{
			u1_printf("KEEP ALIVE����ʧ��\r\n");
		}

		/* �˿ں�����Ϊ1001 */
		addr.sin_port = htons(PORT_NUM);
		
		/* �뺯��socket�е�AF_INET����һ�� */
		addr.sin_family = PF_INET;
		
		addr.sin_addr.s_b1 = IP1;
		addr.sin_addr.s_b2 = IP2;
		addr.sin_addr.s_b3 = IP3;
		addr.sin_addr.s_b4 = IP4;

		/* �ͻ�������Զ�̷����������Զ�̷�������δ�������˺������������� */
		res = connect (sock, (SOCKADDR *)&addr, sizeof (addr));
		u1_printf("�ͻ�������Զ�̷�����״̬%s\r\n", ReVal_Table[abs(res)]);

		while (1) 
		{
			/*
			  socket���ݽ��պ��������recv����������ģʽ��ʹ���������ע���������
			  1. �˺��������ʱ���ܵ�Net_Config.c�к궨�� BSD_RCVTOUT �����ơ����ʱ�䵽���Զ��˳���
			  2. ����������յ�һ�����ݰ��ͻ᷵�أ����ڻ���С�����õĻ�������С��û�й�ϵ�����������
			     ���ڽ��ջ�������С���û�ֻ���ε��ú���recv���н��ռ��ɡ�
			  3. ʵ�ʽ��յ����ݴ�Сͨ���жϴ˺����ķ���ֵ���ɡ�
			*/
			res = recv (sock, dbuf, sizeof(dbuf), 0);
			if (res <= 0) 
			{
				u1_printf("���պ�������״̬%s\r\n", ReVal_Table[abs(res)]);
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
					/* �ַ����� 1 */
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
							u1_printf("����send��������ʧ��\r\n");
						}
						else
						{
							u1_printf("����send�������ݳɹ�\r\n");							
						}
						break;
					
					/* �ַ����� 2 */
					case '2':
						/* �����ݻ���������ַ�0����������������ֲ鿴���� */
						len = sizeof(sendbuf);
						memset(sendbuf, 48, len);
					
						/* �������ʼ�������ݰ���ǰ4���ֽں����4���ֽ� */
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
							u1_printf("����send��������ʧ��%s\r\n", ReVal_Table[abs(res)]);
						}
						else
						{
							u1_printf("����send�ɹ��������� = %d�ֽ�\r\n", res);							
						}
						break;
				
					/* ������ֵ�������� */
					default:                     
						break;
				}
			}

		}
		
		/* 
		   Զ�̷������Ͽ����Ӻ�sock�����Ч�����򶼻�ִ�е��������������ر�socket��
		   ���򷵻ص���һ����whileѭ���Ŀ�ͷ���´���socket�����ӡ�
		*/
		closesocket (sock);
	}
}

