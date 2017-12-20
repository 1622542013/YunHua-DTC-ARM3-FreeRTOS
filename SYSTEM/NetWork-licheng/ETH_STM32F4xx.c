/*
*********************************************************************************************************
*
*	ģ������ : STM32F4XX��MAC����
*	�ļ����� : ETH_STM32F4XX.C
*	��    �� : V2.0
*	˵    �� : �ɹٷ��������޸Ķ�����
*              1. ԭʼ����֧��DP83848C ��ST802RT1���޸�Ϊ��֧��DM9161/9162�����������Ĺ淶����
*              2. ����PHY���жϴ������ܣ���Ҫ���ڼ�����ߵ�����״̬��
*              3. ����g_ucEthLinkStatus���ڱ�ʾ����״̬��
*                         
*	�޸ļ�¼ :
*		�汾��    ����         ����            ˵��
*       V1.0    2015-12-22    Eric2013         �׷�
*       V2.0    2017-04-17    Eric2013         ������ʹ��淶��
*
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include <Net_Config.h>
#include <stm32f4xx.h>             
#include "ETH_STM32F4xx.h"
#include "stdio.h"
#include "USART.h"

#include "usr_FreeRTOS.h"
/*
*********************************************************************************************************
*	                  	STM32-V5������ʹ�õ���RMII�ӿڣ�PHYоƬ��DM9161/9162                            
*********************************************************************************************************
*/
/*
	PA1/ETH_RMII_RX_CLK
	PA2/ETH_MDIO
	PA7/RMII_CRS_DV
	PC1/ETH_MDC
	PC4/ETH_RMII_RX_D0
	PC5/ETH_RMII_RX_D1
	PG11/ETH_RMII_TX_EN
	PG13/FSMC_A24/ETH_RMII_TXD0
	PG14/ETH_RMII_TXD1
	PH6/MII_INT ----- �ж�����,���ｫ���������߶Ͽ��������ӵ�״̬����
*/


/*
*********************************************************************************************************
*	                                  ���ڱ��ļ��ĵ���
*********************************************************************************************************
*/
#if 1
	#define printf_eth printf
#else
	#define printf_eth(...)
#endif


/*
*********************************************************************************************************
*	                                     �궨��
*********************************************************************************************************
*/
/* 
  Ĭ������£�����ѡ�����Զ�ʶ�𣬼�ʹ��PHYоƬ֧�ֵ�Auto-Negotiationʵ������Ӧ10Mbps�������100Mbps����
  ��������ʱ���Գ�������û�ȷ����ʹ�õ�������10Mbps����100Mbps��ֱ��ͨ������ĺ궨��ѡ�񼴿ɣ����ʹ�õ�
  ����Ӧ������������Ҫѡ��
*/
//#define  _10MBIT_
//#define  _100MBIT_


/*
*********************************************************************************************************
*	                                     ����
*********************************************************************************************************
*/
__IO uint8_t  g_ucEthLinkStatus = 0;  /* ��̫������״̬��0 ��ʾδ���ӣ�1 ��ʾ���� */

extern U8 own_hw_adr[];  /* ���ļ�Net_Config.c���涨�� */

static U8 TxBufIndex;
static U8 RxBufIndex;

static RX_Desc Rx_Desc[NUM_RX_BUF];  /* DMA���������� */
static TX_Desc Tx_Desc[NUM_TX_BUF];  /* DMA���������� */

static U32 rx_buf[NUM_RX_BUF][ETH_BUF_SIZE>>2]; /* DMA�������������� */
static U32 tx_buf[NUM_TX_BUF][ETH_BUF_SIZE>>2]; /* DMA�������������� */


/*
*********************************************************************************************************
*	                                     ����
*********************************************************************************************************
*/
/*
  �ײ����������ַ�ʽ��һ���ǲ�ѯ�ģ�һ�����жϵģ����������ǲ��õ��жϷ�ʽ��
  a. ��ѯ��ʽ��Ҫ�ĺ���: - void init_ethernet ()
                         - void send_frame (OS_FRAME *frame)
                         - void poll_ethernet (void)
  b. �жϷ�ʽ��Ҫ�ĺ���: - void init_ethernet ()
                         - void send_frame (OS_FRAME *frame)
                         - void int_enable_eth ()
                         - void int_disable_eth ()
                         - interrupt function 
 */

static void rx_descr_init (void);
static void tx_descr_init (void);
static void write_PHY (U32 PhyReg, U16 Value);
static U16  read_PHY (U32 PhyReg);
static void Eth_Link_EXTIConfig(void);


/*
*********************************************************************************************************
*	�� �� ��: init_ethernet
*	����˵��: ��ʼ����̫��RMII��ʽ���ţ�����PHY������MAC����DMA��ʽ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
#include "usart.h"
#include "delay.h"
void init_ethernet (void) 
{
	U32 regv,tout,conn; 

	/* �ر�PHY�жϴ������� */
	NVIC_DisableIRQ(EXTI9_5_IRQn);

	/* ʹ��ϵͳ���ÿ�����ʱ�� */
	RCC->APB2ENR |= (1 << 14);

	/* ��λ��̫��MAC */
	RCC->AHB1RSTR |=  0x02000000;

	/* ѡ��RMII�ӿڣ������� MAC ���ڸ�λ״̬����ʹ�� MAC ʱ��֮ǰ��ɴ����� */
	SYSCFG->PMC |=  (1 << 23);

	/* ֹͣ��λ��̫��MAC */
	RCC->AHB1RSTR &= ~0x02000000;

  GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOʱ�� RMII�ӿ�
	  //����PA1 PA2 PA7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_ETH); //���Ÿ��õ�����ӿ���
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_ETH);

	//����PC1,PC4 and PC5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_ETH); //���Ÿ��õ�����ӿ���
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_ETH);
                                
	//����PG11, PG14 and PG13 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource13, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_ETH);
	
	//����PD3Ϊ�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;  
	GPIO_Init(GPIOD, &GPIO_InitStructure);          
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_ETH_MAC | RCC_AHB1Periph_ETH_MAC_Tx |RCC_AHB1Periph_ETH_MAC_Rx, ENABLE);
  
  GPIO_ResetBits(GPIOD, GPIO_Pin_3);
  
  Delay_ms(50);
   
  GPIO_SetBits(GPIOD, GPIO_Pin_3);
  
	/* 
	  �Ĵ���ETH->DMABMR��SRλ��1��MAC DMA�������Ḵλ����MAC��ϵͳ���ڲ��Ĵ������߼�����������
	  ��ʱ������ɸ�λ�����󣬸�λ�Զ����㡣���±���κ��ں˼Ĵ���֮ǰ���ڸ�λ�ж�ȡ0 ֵ��
	*/
	ETH->DMABMR  |= DBMR_SR;
	while (ETH->DMABMR & DBMR_SR);
	conn = 0;

	/* 
	   HCLK��ʱ����168MHz������ѡ��CRλΪ100��CRռ�üĴ���ETH->MACMIIAR��bit4��bit3��bit2��
	   CR ʱ�ӷ�Χѡ���ȷ�� HCLK Ƶ�ʲ����ھ��� MDC ʱ��Ƶ�ʣ�
	    ѡ��     HCLK        MDC ʱ��
	    000    60-100MHz     HCLK/42
		001    100-150MHz    HCLK/62
		010    20-35MHz      HCLK/16
		011    35-60MHz      HCLK/26
		100    150-168MHz    HCLK/102
		101��110��111 ����
    */
	ETH->MACMIIAR = 0x00000010;

//	/*
//	  ע�����DM9161�����ϵ��Ͷ�ȡ��ID�Ĵ���������DM9162���У���Ҫ�ӳ�һ��ʱ������Ϊ�˷��������
//	  ֱ�ӽ��临λ�����͸�λָ���������ִ�С�
//	*/
//	/* ��1������λDM9161/9162 ***********************************************************/
//	printf_eth("===============================================================\r\n");
//	printf_eth("������DM9161/9162��Ӳ����ʼ����\r\n");
//	printf_eth("1. Start PHY_ID_DM9161/9162 Init\r\n");

  u1_printf("������PHY��Ӳ����ʼ����\r\n");
	
	/* ���͸�λ���� */
	write_PHY (PHY_REG_BMCR, 0x8000);

	/* �ȴ���λ��� */
	for (tout = 0; tout < 0x10000; tout++) 
	{
		regv = read_PHY (PHY_REG_BMCR);
		if (!(regv & 0x8800)) 
		{
      u1_printf("��λ���\r\n");
			/* ��λ��� */
			break;
		}
	}

	/* ��2��������DM9161/9162 ***********************************************************/
#if defined (_10MBIT_)
	write_PHY (PHY_REG_BMCR, PHY_FULLD_10M);  /* ���ӵ�10Mbps������ */
#elif defined (_100MBIT_)
	write_PHY (PHY_REG_BMCR, PHY_FULLD_100M); /* ���ӵ�100Mbps������ */
#else
	/* ͨ��Auto-Negotiationʵ������Ӧ10Mbps�������100Mbps���� */
	write_PHY (PHY_REG_BMCR, PHY_AUTO_NEG);
	
	/* �ȴ����Auto-Negotiation */
	for (tout = 0; tout < 0xF00000; tout++) 
	{
		regv = read_PHY (PHY_REG_BMSR);
		if (regv & 0x0020) 
		{
      u1_printf("�������Ӧ����\r\n");
			/* ���Auto-Negotiation */
			break;
		}
	}
#endif
	
	/* ��3�����������״̬ ***********************************************************/
	for (tout = 0; tout < 0xF0000; tout++) 
	{
		regv = read_PHY (PHY_REG_BMSR);
		if (regv & (1 << 2)) 
		{
      u1_printf("���ӳɹ�\r\n");
			
			/* PHY�Ѿ����������� */ 
			g_ucEthLinkStatus = 1;
			
			/* ��ȡ������Ϣ */
			regv = read_PHY (PHY_REG_DSCSR);
			
			if ((regv & (1 << 15))|(regv & (1 << 13))) 
			{
        u1_printf("ȫ˫��\r\n");
				/* ȫ˫�� */
				conn |= PHY_CON_SET_FULLD;
			}
			
			if ((regv & (1 << 15))|(regv & (1 << 14))) 
			{
        u1_printf("�ٶ�100Mbps������\r\n");
				/* �ٶ�100Mbps������ */

				conn |= PHY_CON_SET_100M;
			}
			break;
		}
		else
		{
      u1_printf("����ʧ��\r\n");
			
			/* δ������ */ 
			g_ucEthLinkStatus = 0;
		}
	}

	/* ��4����ʹ��DM9161/9162�ж� ***********************************************************/
	/* ʹ��DM9161/9162�������ж� */
	write_PHY (PHY_REG_INTERRUPT, 1<<12);

	/* ��������PH6�������ж��ź� */
	Eth_Link_EXTIConfig();

	/* ��5����ʹ��DM9161/9162�ж� ***********************************************************/	
	/* 
	   ��ʼ��MAC���üĴ���
	   ��1������λMCR_ROD��1ʱ��MAC��ֹ�ڰ�˫��ģʽ�½���֡��
	   ��2������λMCR_ROD��0ʱ��MAC����PHY���͵��������ݰ���
	   ��3�����MAC��ȫ˫��ģʽ�¹�������λ�����á�
	*/
	ETH->MACCR  = MCR_ROD;

	/* ����MAC������ȫ˫��ģʽ */
	if (conn & PHY_CON_SET_FULLD) 
	{
		/* ʹ��ȫ˫�� */
		ETH->MACCR |= MCR_DM;
	}

	/* 
		ͨ��λMCR_FES����MACͨ���ٶ�
		��1��0��ʾ10Mbps
		��2��1��ʾ100Mbps
	*/
	if (conn & PHY_CON_SET_100M) 
	{
		/* ����Ϊ100Mbps */
		ETH->MACCR |= MCR_FES;
	}

	/* MACFFR ��̫��֡���˼Ĵ��������ÿɽ�������MAC�鲥������MAC��ַ��һ���ֽڵ�bit0 = 1 */
	ETH->MACFFR = MFFR_HPF | MFFR_PAM;
	
	/* MACFCR ��̫�������ƼĴ�����ZQPD��ʱ��Ƭ��ͣ��ֹ */
	ETH->MACFCR = MFCR_ZQPD;

	/* ������̫��MAC��ַ�Ĵ��� */
	ETH->MACA0HR = ((U32)own_hw_adr[5] <<  8) | (U32)own_hw_adr[4];
	ETH->MACA0LR = ((U32)own_hw_adr[3] << 24) | (U32)own_hw_adr[2] << 16 |
				   ((U32)own_hw_adr[1] <<  8) | (U32)own_hw_adr[0];

	/* ��ʼ��DMA���ͺͽ��������� */
	rx_descr_init ();
	tx_descr_init ();

	/* 
	    ˢ��FIFO������DMA���ͺͽ��չ���
		DMAOMR ����ģʽ�Ĵ���
		λ20 DOMR_FTF��ˢ�·��� FIFO (Flush transmit FIFO)��
		               ��λ��1ʱ������FIFO�������߼��ḴλΪĬ��ֵ����ˣ�TX FIFO�е��������ݾ���
			           ��ʧ/ˢ�¡�ˢ�²�������ʱ��λ���ڲ����㡣��λ����֮ǰ���öԹ���ģʽ�Ĵ���ִ
			           ��д������
		λ13 DOMR_ST������/ֹͣ���� (Start/stop transmission)
		              ��λ��1ʱ���������ͣ�DMA���鵱ǰλ�õķ����б������Ҵ����͵�֡��
		λ1  DOMR_SR������/ֹͣ���� (Start/stop receive)
		              ��λ��1ʱ���������գ�DMA���Դӽ����б��л�ȡ��������������֡��
	*/
	ETH->DMAOMR = DOMR_FTF | DOMR_ST | DOMR_SR;

	/* ʹ�ܷ��ͺͽ��� */
	ETH->MACCR |= MCR_TE | MCR_RE;

	/* ��λ����MAC�ж� */
	ETH->DMASR  = 0xFFFFFFFF;

	/*  
	    ʹ�ܷ��ͺͽ����ж�
		DMAIER �ж�ʹ�ܼĴ���
		λ16 NISE��ʹ�����������ж�(Normal interrupt summary enable)
		λ15 AISE��ʹ�������쳣�ж�(Abnormal interrupt summary enable)
		λ7 RBUIE�����ջ������������ж�ʹ��(Receive buffer unavailable interrupt enable)
		           ����λ��AISEλ����1�󣬿�ʹ�ܽ��ջ������������жϡ���λ����ʱ�����ֹ��
				   �ջ������������жϡ�
		λ6   RIE�������ж�ʹ�� (Receive interrupt enable)
		           ����λ��AISE����1�󣬿�ʹ�ܽ����жϡ���λ����ʱ�����ֹ�����жϡ�
	*/
	ETH->DMAIER = ETH_DMAIER_NISE | ETH_DMAIER_AISE | ETH_DMAIER_RBUIE | ETH_DMAIER_RIE;

	/* ����Ϊ������ȼ���������NVIC->ISER���õ�Ĭ�����ȼ�Ҳ��������ȼ�0 */
	NVIC_SetPriority(ETH_IRQn, 1);
	
  u1_printf("��ʼ�����\r\n");

}

/*
*********************************************************************************************************
*	�� �� ��: int_enable_eth
*	����˵��: ʹ����̫���ж�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void int_enable_eth (void) 
{
	NVIC->ISER[1] = 1 << 29;
}

/*
*********************************************************************************************************
*	�� �� ��: int_disable_eth
*	����˵��: ʹ����̫���ж�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void int_disable_eth (void) 
{
	NVIC->ICER[1] = 1 << 29;
}

/*
*********************************************************************************************************
*	�� �� ��: send_frame
*	����˵��: ��������֡��MAC DMA��������������ʹ�ܷ��͡�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void send_frame (OS_FRAME *frame) 
{
	U32 *sp,*dp;
	U32 i,j;

	j = TxBufIndex;
	
	/* �ȴ���һ֡���ݷ������ */
	while (Tx_Desc[j].CtrlStat & DMA_TX_OWN);

	sp = (U32 *)&frame->data[0];
	dp = (U32 *)(Tx_Desc[j].Addr & ~3);

	/* ����Ҫ���͵����ݵ�DMA������������ */
	for (i = (frame->length + 3) >> 2; i; i--) 
	{
		*dp++ = *sp++;
	}
	
	/* ��������֡��С */
	Tx_Desc[j].Size      = frame->length;
	
	/* ������������DMA���Ʒ��� */
	Tx_Desc[j].CtrlStat |= DMA_TX_OWN;
	
	if (++j == NUM_TX_BUF) j = 0;
	TxBufIndex = j;
	
	/* ��ʼ֡���� */
	/* 
	   DMASR ��̫�� DMA ״̬�Ĵ���
	   ��ETH_DMASR�Ĵ���[16:0]�еģ�δ������λд��1�Ὣ�����㣬д�� 0 �������á�
	   λ1 TPSS�����͹���ֹͣ״̬ (Transmit process stopped status)
                 ������ֹͣʱ����λ�� 1��
	*/
	ETH->DMASR   = DSR_TPSS;
	
	/*
	   DMATPDR ��̫��DMA������ѯ����Ĵ���
       Ӧ�ó���ʹ�ô˼Ĵ�����ָʾDMA��ѯ�����������б�
       λ 31:0 TPD��������ѯ����(Transmit poll demand)
                    ����Щλд���κ�ֵʱ��DMA�����ȡETH_DMACHTDR�Ĵ���ָ��ĵ�ǰ�����������
                    �������������ã���CPU���У������ͻ᷵�ص�����״̬������ETH_DMASR�Ĵ���λ2
                    ������λ����������������ã����ͻ�������С�	   
	*/
	ETH->DMATPDR = 0;
	
	xEventGroupSetBits(Main_tcpnet_group,MAIN_TCP);
}

/*
*********************************************************************************************************
*	�� �� ��: ETH_IRQHandler
*	����˵��: ��̫���жϣ���Ҫ�����MAC DMA�������������յ�������֡�Լ������־�Ĵ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ETH_IRQHandler (void) 
{
	OS_FRAME *frame;
	U32 i, RxLen;
	U32 *sp,*dp;

	i = RxBufIndex;
	
	/* ѭ�����н����������б�����δ���յ����ݵ��˳�ѭ�� */
	do 
	{
		/*
			#define DMA_RX_ERROR_MASK   (DMA_RX_ES | DMA_RX_LE | DMA_RX_RWT | \
								         DMA_RX_RE | DMA_RX_CE)
			
			�д��󣬷�����֡���ݣ��������Ͱ������£�
			λ15 DMA_RX_ES���������(Error summary)����CRC���󣬽��մ��󣬿��Ź���ʱ���ӳٳ�ͻ�ȡ�
		    λ12 DMA_RX_LE�����ȴ���(Length error)
		                    ��λ��1ʱ��ָʾ����֡��ʵ�ʳ����볤��/�����ֶε�ֵ���������ֶν���֡��
		                    ��λ(RDES0[5])��λ����Ч��
			λ4 DMA_RX_RWT�����տ��Ź���ʱ (Receive watchdog timeout)
		                    ��λ��1ʱ����ʾ���տ��Ź���ʱ���ڽ��յ�ǰ֡ʱ��ʱ���ҵ�ǰ֡�ڿ��Ź���
		                    ʱ�󱻽ض���
			λ3 DMA_RX_RE�� ���մ��� (Receive error)
                            ��λ��1ʱ����ʾ��֡�����ڼ䣬������RX_DV�ź�ʱ���ᷢ��RX_ERR�źš�
			λ1 DMA_RX_CE�� CRC ����(CRC error)
                            ��λ��1ʱ����ʾ���յ�֡����ѭ������У��(CRC)����ֻ�����һ��������
		                    (RDES0[8])��1ʱ�����ֶβ���Ч
		*/
		if (Rx_Desc[i].Stat & DMA_RX_ERROR_MASK)
		{
			goto rel;
		}
		
		/*
			#define DMA_RX_SEG_MASK   (DMA_RX_FS | DMA_RX_LS)
		    λ9 FS����һ�������� (First descriptor)
                    ��λ��1ʱ��ָʾ������������֡�ĵ�һ���������������һ���������Ĵ�СΪ0����ڶ�
                    ��������������֡��֡ͷ������ڶ����������Ĵ�СΪ0������һ��������������֡��֡ͷ��
		
		    λ8 LS�����һ�������� (Last descriptor)
                    ��λ��1ʱ��ָʾ��������ָ��Ļ�����Ϊ֡�����һ����������
		
		    ����ĺ��������жϴ�֡�����Ƿ�ֻ��һ�����壬��ʼ�������������б��ʱ��ÿ����������������
		    һ�����塣
		*/
		if ((Rx_Desc[i].Stat & DMA_RX_SEG_MASK) != DMA_RX_SEG_MASK) 
		{
			goto rel;
		}
		
		RxLen = ((Rx_Desc[i].Stat >> 16) & 0x3FFF) - 4;
		if (RxLen > ETH_MTU) 
		{
			/* ���ݰ�̫��ֱ�ӷ��� */
			goto rel;
		}
		
		/* ���붯̬�ڴ棬RxLen����0x80000000��ʾ��̬�ڴ治���˲�����ú���sys_error() */
		frame = alloc_mem (RxLen | 0x80000000);
		
		/* �����̬�ڴ�����ʧ���ˣ�������֡���ݣ��ɹ��ˣ�ͨ������put_in_queue��������� */
		if (frame != NULL) 
		{
			sp = (U32 *)(Rx_Desc[i].Addr & ~3);
			dp = (U32 *)&frame->data[0];
			for (RxLen = (RxLen + 3) >> 2; RxLen; RxLen--) 
			{
				*dp++ = *sp++;
			}
			put_in_queue (frame);
		}
		
		/* ���ô˽������������������µ����� */
		rel: Rx_Desc[i].Stat = DMA_RX_OWN;

		if (++i == NUM_RX_BUF) i = 0;
	}
	while (!(Rx_Desc[i].Stat & DMA_RX_OWN));
	
	RxBufIndex = i;

	/*
	   DMASR DMA��״̬�Ĵ�����DMA status register��
	   λ7 RBUS�����ջ�����������״̬ (Receive buffer unavailable status)
                 ��λָʾ�����б��е���һ����������CPU��ӵ�У�DMA�޷���ȡ�����չ��̽������״̬��
	             Ҫ�ָ����������������CPUӦ������������ӵ�й�ϵ��Ȼ�󷢳�������ѯ����������
	             δ����������ѯ��������򵱽��յ���һ��ʶ��Ĵ���֡ʱ�����չ��̻�ָ���������һ
	             ������������DMA��ӵ��ʱ�����ܽ�ETH_DMASR[7]��1��
	
	   DMAIER�Ľ��ջ������������ж�RBUIE��bit7�����ڵĽ��ջ�����������״̬��DMA״̬�Ĵ�����Ҳ��bit7��
	*/
	if (ETH->DMASR & INT_RBUIE) 
	{
		/* ���ջ����������ã����»ָ�DMA���� */
		ETH->DMASR = ETH_DMASR_RBUS;
		ETH->DMARPDR = 0;
	}
	
	/* 
	   DMASR DMA��״̬�Ĵ�����DMA status register��
	   ����ʵ������жϹ����־ 
	   λ16 ETH_DMASR_NIS�����������ж� (Normal interrupt summary)
	   λ15 ETH_DMASR_AIS�������쳣�ж� (Abnormal interrupt summary)
	   λ6  ETH_DMASR_RS ������״̬ (Receive status)
	                       ��λָʾ֡��������ɣ������֡״̬��Ϣ�Ѿ��������������У������Ա�������״̬��
	*/
	ETH->DMASR = ETH_DMASR_NIS | ETH_DMASR_AIS | ETH_DMASR_RS;
	
	
	BaseType_t set_maintcp_status;
	BaseType_t isr_group = pdFALSE;
	
	set_maintcp_status = xEventGroupSetBitsFromISR(Main_tcpnet_group,0x0001,&isr_group);
	
	if(set_maintcp_status != pdFALSE)
	{
		portYIELD_FROM_ISR(isr_group);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: rx_descr_init
*	����˵��: MAC DMA������������ʼ����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void rx_descr_init (void) 
{
	U32 i,next;

	/*
	   1. RDES0��������������0����ӦRx_Desc[i].Stat
	      λ31 OWN�����й�ϵλ (Own bit)
					��λ��1ʱ��ָʾ��������MAC��ϵͳ��DMA��ӵ�С�
	                ��λ����ʱ��ָʾ��������������ӵ�У���CPU��
	                DMA��֡������ɻ���������Ĺ�������������ʱ����λ���㡣
	
	   2. RDES1��������������1����ӦRx_Desc[i].Ctrl
	      λ14   RCH�� ���ӵĵڶ�����ַ (Second address chained)
					   ��λ��1ʱ����ʾ�������еĵڶ�����ַ����һ����������ַ�����ǵڶ�����������ַ����
                       λ��1ʱ��RBS2(RDES1[28:16])Ϊ�޹�ֵ��RDES1[15]��RDES1[14]���ȴ���
	      λ12:0 RBS1�����ջ�����1��С (Receive buffer 1 size)
                       ��һ�����ݻ������Ĵ�С���ֽ�Ϊ��λ����ʹRDES2��������1��ַָ�룩��ֵδ���룬��
	                   ������СҲ����Ϊ4��8��16�ı���������ȡ�������߿��32��64��128�������������С��
	                   ��4��8��16�ı�������������Ľ����δ���塣������ֶ�Ϊ0����DMA����Ըû�������
	                   ʹ�û�����2����һ��������������ȡ����RCH��λ14����ֵ��
	
	   3. RDES2��������������2����ӦRx_Desc[i].Addr
	      λ31:0 RBAP1/RTSL�����ջ�����1��ַָ��/����֡ʱ�����λ
		                     Receive buffer 1 address pointer
                             Receive frame time stamp low
	
	   4. RDES3��������������3����ӦRx_Desc[i].Next
	      λ31:0 RBAP2/RTSH�����ջ�����2��ַָ�루��һ����������ַ��/ ����֡ʱ�����λ
		                     Receive buffer 2 address pointer (next descriptor address) 
		                     Receive frame time stamp high
	*/
	RxBufIndex = 0;
	
	for (i = 0, next = 0; i < NUM_RX_BUF; i++) 
	{
		if (++next == NUM_RX_BUF) next = 0;
		Rx_Desc[i].Stat = DMA_RX_OWN;
		Rx_Desc[i].Ctrl = DMA_RX_RCH | ETH_BUF_SIZE;
		Rx_Desc[i].Addr = (U32)&rx_buf[i];
		Rx_Desc[i].Next = (U32)&Rx_Desc[next];
	}
	
	/* �����������б��ַ�Ĵ���ָ������������б����ʼ�� */
	ETH->DMARDLAR = (U32)&Rx_Desc[0];
}

/*
*********************************************************************************************************
*	�� �� ��: tx_descr_init
*	����˵��: MAC DMA������������ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void tx_descr_init (void) 
{
	U32 i,next;

	/*
	   1. TDES0��������������0����ӦTx_Desc[i].CtrlStat
	      λ29 LS ��ĩ�� (Last segment)
                    ��λ��1ʱ��ָʾ�������а���֡��ĩ�Ρ�
	      λ28 FS ���׶� (First segment)
                    ��λ��1ʱ��ָʾ�������а���֡���׶�
          λ20 TCH�����ӵĵڶ�����ַ (Second address chained)
                    ��λ��1ʱ����ʾ�������еĵڶ�����ַ����һ����������ַ�����ǵڶ�����������ַ��
                    TDES0[20]��1ʱ��TBS2(TDES1[28:16])Ϊ�޹�ֵ��TDES0[21]��TDES0[20]���ȴ���
	
	   2. TDES1��������������1����ӦTx_Desc[i].Size
	
	   3. TDES2��������������2����ӦTx_Desc[i].Addr
	      λ31:0 TBAP1�����ͻ�����1��ַָ��/����֡ʱ�����λ 
	                    Transmit buffer 1 address pointer / Transmitframe time stamp low
	
	   4. TDES3��������������3����ӦTx_Desc[i].Next
	      λ 1:0 TBAP2�����ͻ�����2��ַָ�루��һ����������ַ��/ ����֡ʱ�����λ
     		            Transmit buffer 2 address pointer (Next descriptor address) 
						Transmit frame time stamp high
	*/
	TxBufIndex = 0;
	for (i = 0, next = 0; i < NUM_TX_BUF; i++) 
	{
		if (++next == NUM_TX_BUF) next = 0;
		Tx_Desc[i].CtrlStat = DMA_TX_TCH | DMA_TX_LS | DMA_TX_FS;
		Tx_Desc[i].Addr     = (U32)&tx_buf[i];
		Tx_Desc[i].Next     = (U32)&Tx_Desc[next];
	}
	
	/* �����������б��ַ�Ĵ���ָ�����������б����ʼ�� */
	ETH->DMATDLAR = (U32)&Tx_Desc[0];
}


/*
*********************************************************************************************************
*	�� �� ��: write_PHY
*	����˵��: д���ݵ�PHY�Ĵ���
*	��    ��: PhyReg  PHY�Ĵ�����ַ
*             Value   д�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void write_PHY (U32 PhyReg, U16 Value)
{
	U32 tout;

	/* ���ݼĴ���������Ҫд��PHY��16λ���� */
	ETH->MACMIIDR = Value;

	/*
	   MACMIIAR ��̫��MAC MII ��ַ�Ĵ���
       λ 15:11 PA��PHY ��ַ (PHY address)����Ӧ�����PHY_DEF_ADDR
					���ֶ�ָʾ���ڷ���32�����ܵ�PHY�����е���һ����	
	   
	   λ 10:6  MR��MII�Ĵ��� (MII register)����Ӧ�����PhyReg
					��Щλ����ѡ��PHY������ѡ��Ҫ���ʵ�MII�Ĵ�����
	   λ 1     MW��MIIд(MII write)����Ӧ�����MMAR_MW
	                ��λ��1���ڸ�֪PHY����Ҫ����һ��ʹ��MII���ݼĴ�����д������
	                �����λδ�ã����ʾ������һ���������������ݷ���MII���ݼĴ�����
	   λ 0     MB��MIIæµ (MII busy)����Ӧ�����MMAR_MB
	                ��ETH_MACMIIAR��ETH_MACMIIDRд��ǰ����λӦ��ȡ�߼�0����ETH_MACMIIARд������У���
	                λҲ���븴λΪ0����PHY�Ĵ������ʹ����У���λ��Ӧ�ó�����1��ָʾ����д�������ڽ����С�
	                �ڶ�PHY����д���������У�ETH_MACMIIDR��MII���ݣ�Ӧʼ�ձ�����Ч��ֱ��MAC����λ���㡣
	                �ڶ�PHY���ж����������У�ETH_MACMIIDRʼ����Ч��ֱ��MAC����λ���㡣�ڴ�λ����󣬲�
					������ETH_MACMIIAR��MII��ַ��д�롣
	*/
	ETH->MACMIIAR = PHY_DEF_ADDR << 11 | PhyReg << 6 | MMAR_MW | MMAR_MB;

	/* �ȴ�������ɣ����ȴ�MMAR_MBλ������ */
	tout = 0;
	for (tout = 0; tout < MII_WR_TOUT; tout++) 
	{
		if ((ETH->MACMIIAR & MMAR_MB) == 0) 
		{
			break;
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: read_PHY
*	����˵��: ��PHYоƬ�ж�ȡ�Ĵ�������
*	��    ��: PhyReg  �Ĵ�����ַ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static U16 read_PHY (U32 PhyReg) 
{
	U32 tout;

	/*
	   MACMIIAR ��̫��MAC MII ��ַ�Ĵ���
       λ 15:11 PA��PHY ��ַ (PHY address)����Ӧ�����PHY_DEF_ADDR
					���ֶ�ָʾ���ڷ���32�����ܵ�PHY�����е���һ����	
	   
	   λ 10:6  MR��MII�Ĵ��� (MII register)����Ӧ�����PhyReg
					��Щλ����ѡ��PHY������ѡ��Ҫ���ʵ�MII�Ĵ�����
	   λ 1     MW��MIIд(MII write)����Ӧ�����MMAR_MW
	                ��λ��1���ڸ�֪PHY����Ҫ����һ��ʹ��MII���ݼĴ�����д������
	                �����λδ�ã����ʾ������һ���������������ݷ���MII���ݼĴ�����
	   λ 0     MB��MIIæµ (MII busy)����Ӧ�����MMAR_MB
	                ��ETH_MACMIIAR��ETH_MACMIIDRд��ǰ����λӦ��ȡ�߼�0����ETH_MACMIIARд������У���
	                λҲ���븴λΪ0����PHY�Ĵ������ʹ����У���λ��Ӧ�ó�����1��ָʾ����д�������ڽ����С�
	                �ڶ�PHY����д���������У�ETH_MACMIIDR��MII���ݣ�Ӧʼ�ձ�����Ч��ֱ��MAC����λ���㡣
	                �ڶ�PHY���ж����������У�ETH_MACMIIDRʼ����Ч��ֱ��MAC����λ���㡣�ڴ�λ����󣬲�
					������ETH_MACMIIAR��MII��ַ��д�롣
	*/
	ETH->MACMIIAR = PHY_DEF_ADDR << 11 | PhyReg << 6 | MMAR_MB;

	/* �ȴ�������ɣ����ȴ�MMAR_MBλ������ */
	tout = 0;
	for (tout = 0; tout < MII_RD_TOUT; tout++) 
	{
		if ((ETH->MACMIIAR & MMAR_MB) == 0) 
		{
			break;
		}
	}
	
	/* �� PHY �ж�ȡ16bit������ֵ */
	return (ETH->MACMIIDR & MMDR_MD);
}

/*
*********************************************************************************************************
*	�� �� ��: Eth_Link_EXTIConfig
*	����˵��: PHY���жϴ���ʹ��STM32��PH6����ʶ�𣬴˹�����Ҫ���ڼ�����ߵ�����״̬�б�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void Eth_Link_EXTIConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* ������STM32-V6������ʹ��PH6��Ϊ�ж������, �½��ر�ʾ�ж��ź� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* �����ж����������� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOH, &GPIO_InitStructure);

	/* �����ⲿ�ж������ӵ���Ӧ���� */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOH, EXTI_PinSource6);

	/* �����ⲿ�ж��� */
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* ʹ���ж�ͨ�� */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: EXTI9_5_IRQHandler
*	����˵��: PH6���ŵ��жϴ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
#define ETH_CONSTATUS
#define ETH_CONNECT    "ETH_LINK Connect\r\n"
#define ETH_DISCONNECT "ETH_LINK Disconnect\r\n"
void EXTI9_5_IRQHandler(void)
{
	U32 regv, tout;
	
	if (EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
		/* ���Կ����ڴ˴������ӳ٣���ʱ����״̬���ˣ����ǼĴ���û�м�ʱ����*/
		regv = read_PHY(PHY_REG_INTERRUPT);
		if(regv & (1 << 2))
		{
			/* ���²����Ҫ������Σ���֤�Ĵ���BMSR������ */
			for(tout = 0; tout < 10; tout++) 
			{
				regv = read_PHY (PHY_REG_BMSR);
				if (regv & (1 << 2)) 
				{
					break;
				}
			}

			/* ���������� */
			if(regv & (1 << 2)) 
			{
				#ifdef ETH_CONSTATUS
					const char *pError = ETH_CONNECT;
					uint8_t i;
				#endif
				
				g_ucEthLinkStatus = 1;
				
				#ifdef ETH_CONSTATUS
					for (i = 0; i < sizeof(ETH_CONNECT); i++)
					{
						USART1->DR = pError[i];
						/* �ȴ����ͽ��� */
						while ((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);
					}
				#endif 
			}
			/* ���߶Ͽ� */
			else
			{
				#ifdef ETH_CONSTATUS
					const char *pError = ETH_DISCONNECT;
					uint8_t i;
				#endif
				
				g_ucEthLinkStatus = 0;
				
				#ifdef ETH_CONSTATUS
					for (i = 0; i < sizeof(ETH_DISCONNECT); i++)
					{
						USART1->DR = pError[i];
						/* �ȴ����ͽ��� */
						while ((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);
					}
				#endif
				
			}
			
		}
		/* ���жϹ���λ */
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
