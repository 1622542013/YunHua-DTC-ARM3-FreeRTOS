#include "lan8720.h"
#include "stm32f4x7_eth.h"
#include "usart.h" 
#include "delay.h"


ETH_DMADESCTypeDef *DMARxDscrTab;	//��̫��DMA�������������ݽṹ��ָ��
ETH_DMADESCTypeDef *DMATxDscrTab;	//��̫��DMA�������������ݽṹ��ָ�� 
uint8_t *Rx_Buff; 					//��̫���ײ���������buffersָ�� 
uint8_t *Tx_Buff; 					//��̫���ײ���������buffersָ��
  
static void ETHERNET_NVICConfiguration(void);
//LAN8720��ʼ��
//����ֵ:0,�ɹ�;
//    ����,ʧ��
u8 LAN8720_Init(void)
{
	u8 rval=0;
	GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOʱ�� RMII�ӿ�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);   //ʹ��SYSCFGʱ��
  
	SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_RMII); //MAC��PHY֮��ʹ��RMII�ӿ�

	/*������������ RMII�ӿ�
	  ETH_MDIO -------------------------> PA2
	  ETH_MDC --------------------------> PC1
	  ETH_RMII_REF_CLK------------------> PA1
	  ETH_RMII_CRS_DV ------------------> PA7
	  ETH_RMII_RXD0 --------------------> PC4
	  ETH_RMII_RXD1 --------------------> PC5
	  ETH_RMII_TX_EN -------------------> PG11
	  ETH_RMII_TXD0 --------------------> PG13
	  ETH_RMII_TXD1 --------------------> PG14
	  ETH_RESET-------------------------> PD3*/
					
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
	
  
  GPIO_WriteBit(GPIOD, GPIO_PinSource3,Bit_RESET);//Ӳ����λLAN8720				
	for(int i = 0;i<9999;i++);
	GPIO_WriteBit(GPIOD, GPIO_PinSource3,Bit_SET);//��λ����
  
	ETHERNET_NVICConfiguration();	//�����ж����ȼ�
	rval=ETH_MACDMA_Config();		//����MAC��DMA
  

	return !rval;					//ETH�Ĺ���Ϊ:0,ʧ��;1,�ɹ�;����Ҫȡ��һ�� 
}

//��̫���жϷ�������
void ETHERNET_NVICConfiguration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = ETH_IRQn;  //��̫���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0X00;  //�жϼĴ�����2������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0X00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


//�õ�8720���ٶ�ģʽ
//����ֵ:
//001:10M��˫��
//101:10Mȫ˫��
//010:100M��˫��
//110:100Mȫ˫��
//����:����.
u8 LAN8720_Get_Speed(void)
{
	u8 speed;
	speed=((ETH_ReadPHYRegister(0x00,31)&0x1C)>>2); //��LAN8720��31�żĴ����ж�ȡ�����ٶȺ�˫��ģʽ
	return speed;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
//���²���ΪSTM32F407��������/�ӿں���.

//��ʼ��ETH MAC�㼰DMA����
//����ֵ:ETH_ERROR,����ʧ��(0)
//		ETH_SUCCESS,���ͳɹ�(1)
u8 ETH_MACDMA_Config(void)
{
	u8 rval;
	ETH_InitTypeDef ETH_InitStructure; 
	
	//ʹ����̫��MAC�Լ�MAC���պͷ���ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_ETH_MAC | RCC_AHB1Periph_ETH_MAC_Tx |RCC_AHB1Periph_ETH_MAC_Rx, ENABLE);
                        
	ETH_DeInit();  								//AHB����������̫��
	ETH_SoftwareReset();  						//�����������
	while (ETH_GetSoftwareResetStatus() == SET);//�ȴ��������������� 
	ETH_StructInit(&ETH_InitStructure); 	 	//��ʼ������ΪĬ��ֵ  

	///����MAC�������� 
	ETH_InitStructure.ETH_AutoNegotiation 				 = ETH_AutoNegotiation_Enable;   			//������������Ӧ����
	ETH_InitStructure.ETH_LoopbackMode    				 = ETH_LoopbackMode_Disable;					//�رշ���
	ETH_InitStructure.ETH_RetryTransmission 			 = ETH_RetryTransmission_Disable; 		//�ر��ش�����
	ETH_InitStructure.ETH_AutomaticPadCRCStrip 		 = ETH_AutomaticPadCRCStrip_Disable; 	//�ر��Զ�ȥ��PDA/CRC���� 
	ETH_InitStructure.ETH_ReceiveAll 							 = ETH_ReceiveAll_Disable;						//�رս������е�֡
	ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;//����������й㲥֡
	ETH_InitStructure.ETH_PromiscuousMode 				 = ETH_PromiscuousMode_Disable;			//�رջ��ģʽ�ĵ�ַ����  
	ETH_InitStructure.ETH_MulticastFramesFilter 	 = ETH_MulticastFramesFilter_Perfect;//�����鲥��ַʹ��������ַ����   
	ETH_InitStructure.ETH_UnicastFramesFilter 		 = ETH_UnicastFramesFilter_Perfect;	//�Ե�����ַʹ��������ַ���� 
#ifdef CHECKSUM_BY_HARDWARE
	ETH_InitStructure.ETH_ChecksumOffload 				 = ETH_ChecksumOffload_Enable; 			//����ipv4��TCP/UDP/ICMP��֡У���ж��   
#endif
	//������ʹ��֡У���ж�ع��ܵ�ʱ��һ��Ҫʹ�ܴ洢ת��ģʽ,�洢ת��ģʽ��Ҫ��֤����֡�洢��FIFO��,
	//����MAC�ܲ���/ʶ���֡У��ֵ,����У����ȷ��ʱ��DMA�Ϳ��Դ���֡,����Ͷ�������֡
	ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable; //��������TCP/IP����֡
	ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;     //�����������ݵĴ洢ת��ģʽ    
	ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;   //�����������ݵĴ洢ת��ģʽ  

	ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;     	//��ֹת������֡  
	ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;	//��ת����С�ĺ�֡ 
	ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;  		//�򿪴���ڶ�֡����
	ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;  	//����DMA����ĵ�ַ���빦��
	ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;            			//�����̶�ͻ������    
	ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;     		//DMA���͵����ͻ������Ϊ32������   
	ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;			//DMA���յ����ͻ������Ϊ32������
	ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;
	rval=ETH_Init(&ETH_InitStructure,LAN8720_PHY_ADDRESS);		//����ETH
	if(rval==ETH_SUCCESS)//���óɹ�
	{
		ETH_DMAITConfig(ETH_DMA_IT_NIS|ETH_DMA_IT_R,ENABLE);  	//ʹ����̫�������ж�	
	}
	return rval;
}

////��̫��DMA�����жϷ�����
//void ETH_IRQHandler(void)
//{
//	while(ETH_GetRxPktSize(DMARxDescToGet)!=0) 	//����Ƿ��յ����ݰ�
//	{ 
//		
//	}
//	ETH_DMAClearITPendingBit(ETH_DMA_IT_R); 	//���DMA�жϱ�־λ
//	ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS);	//���DMA�����жϱ�־λ
//}  
















