/*
*********************************************************************************************************
*
*	模块名称 : STM32F4XX的MAC驱动
*	文件名称 : ETH_STM32F4XX.C
*	版    本 : V2.0
*	说    明 : 由官方的驱动修改而来。
*              1. 原始驱动支持DP83848C 和ST802RT1，修改为仅支持DM9161/9162，方便驱动的规范化。
*              2. 增加PHY的中断触发功能，主要用于检测网线的连接状态。
*              3. 变量g_ucEthLinkStatus用于表示连接状态。
*                         
*	修改记录 :
*		版本号    日期         作者            说明
*       V1.0    2015-12-22    Eric2013         首发
*       V2.0    2017-04-17    Eric2013         整理，并使其规范化
*
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include <Net_Config.h>
#include <stm32f4xx.h>             
#include "ETH_STM32F4xx.h"
#include "stdio.h"



/*
*********************************************************************************************************
*	                  	STM32-V5开发板使用的是RMII接口，PHY芯片是DM9161/9162                            
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
	PH6/MII_INT ----- 中断引脚,这里将其用于网线断开或者连接的状态触发
*/


/*
*********************************************************************************************************
*	                                  用于本文件的调试
*********************************************************************************************************
*/
#if 1
	#define printf_eth printf
#else
	#define printf_eth(...)
#endif


/*
*********************************************************************************************************
*	                                     宏定义
*********************************************************************************************************
*/
/* 
  默认情况下，我们选择是自动识别，即使用PHY芯片支持的Auto-Negotiation实现自适应10Mbps网络或者100Mbps网络
  但是这种时间稍长，如果用户确定了使用的网络是10Mbps还是100Mbps，直接通过下面的宏定义选择即可，如果使用的
  自适应，两个都不需要选择。
*/
//#define  _10MBIT_
//#define  _100MBIT_


/*
*********************************************************************************************************
*	                                     变量
*********************************************************************************************************
*/
__IO uint8_t  g_ucEthLinkStatus = 0;  /* 以太网连接状态，0 表示未连接，1 表示连接 */

extern U8 own_hw_adr[];  /* 在文件Net_Config.c里面定义 */

static U8 TxBufIndex;
static U8 RxBufIndex;

static RX_Desc Rx_Desc[NUM_RX_BUF];  /* DMA接收描述符 */
static TX_Desc Tx_Desc[NUM_TX_BUF];  /* DMA发送描述符 */

static U32 rx_buf[NUM_RX_BUF][ETH_BUF_SIZE>>2]; /* DMA接收描述符缓冲 */
static U32 tx_buf[NUM_TX_BUF][ETH_BUF_SIZE>>2]; /* DMA发送描述符缓冲 */


/*
*********************************************************************************************************
*	                                     变量
*********************************************************************************************************
*/
/*
  底层驱动有两种方式，一种是查询的，一种是中断的，我们这里是采用的中断方式。
  a. 查询方式需要的函数: - void init_ethernet ()
                         - void send_frame (OS_FRAME *frame)
                         - void poll_ethernet (void)
  b. 中断方式需要的函数: - void init_ethernet ()
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
*	函 数 名: init_ethernet
*	功能说明: 初始化以太网RMII方式引脚，驱动PHY，配置MAC及其DMA方式。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
#include "usart.h"
void init_ethernet (void) 
{
	U32 regv,tout,conn; 

	/* 关闭PHY中断触发引脚 */
	NVIC_DisableIRQ(EXTI9_5_IRQn);

	/* 使能系统配置控制器时钟 */
	RCC->APB2ENR |= (1 << 14);

	/* 复位以太网MAC */
	RCC->AHB1RSTR |=  0x02000000;

	/* 选择RMII接口，必须在 MAC 处于复位状态且在使能 MAC 时钟之前完成此配置 */
	SYSCFG->PMC |=  (1 << 23);

	/* 停止复位以太网MAC */
	RCC->AHB1RSTR &= ~0x02000000;

  GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIO时钟 RMII接口
	  //配置PA1 PA2 PA7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_ETH); //引脚复用到网络接口上
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_ETH);

	//配置PC1,PC4 and PC5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_ETH); //引脚复用到网络接口上
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_ETH);
                                
	//配置PG11, PG14 and PG13 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource13, GPIO_AF_ETH);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_ETH);
	
	//配置PD3为推完输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//推完输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;  
	GPIO_Init(GPIOD, &GPIO_InitStructure);          
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_ETH_MAC | RCC_AHB1Periph_ETH_MAC_Tx |RCC_AHB1Periph_ETH_MAC_Rx, ENABLE);
  
  GPIO_ResetBits(GPIOD, GPIO_Pin_3);
  
   
   for(int i = 0; i<99999;i++)
   {
      __nop();__nop();__nop(); __nop();__nop();__nop(); __nop();__nop();__nop(); __nop();__nop();__nop();
   }
   
   GPIO_SetBits(GPIOD, GPIO_Pin_3);
  
	/* 
	  寄存器ETH->DMABMR的SR位置1后，MAC DMA控制器会复位所有MAC子系统的内部寄存器和逻辑。在所有内
	  核时钟域完成复位操作后，该位自动清零。重新编程任何内核寄存器之前，在该位中读取0 值。
	*/
	ETH->DMABMR  |= DBMR_SR;
	while (ETH->DMABMR & DBMR_SR);
	conn = 0;

	/* 
	   HCLK的时钟是168MHz，这里选项CR位为100，CR占用寄存器ETH->MACMIIAR的bit4，bit3和bit2。
	   CR 时钟范围选项可确定 HCLK 频率并用于决定 MDC 时钟频率：
	    选项     HCLK        MDC 时钟
	    000    60-100MHz     HCLK/42
		001    100-150MHz    HCLK/62
		010    20-35MHz      HCLK/16
		011    35-60MHz      HCLK/26
		100    150-168MHz    HCLK/102
		101、110、111 保留
    */
	ETH->MACMIIAR = 0x00000010;

//	/*
//	  注意事项：DM9161可以上电后就读取其ID寄存器，但是DM9162不行，需要延迟一段时间这里为了方便起见，
//	  直接将其复位，发送复位指令可以立即执行。
//	*/
//	/* 第1步：复位DM9161/9162 ***********************************************************/
//	printf_eth("===============================================================\r\n");
//	printf_eth("下面是DM9161/9162的硬件初始化：\r\n");
//	printf_eth("1. Start PHY_ID_DM9161/9162 Init\r\n");

  uint8_t send_buff[50];
  uint16_t num;
  
  num = snprintf((char *)send_buff,50,"下面是PHY的硬件初始化：\r\n");
  USART_SetSendData(USART1, send_buff, num); 
	
	/* 发送复位命令 */
	write_PHY (PHY_REG_BMCR, 0x8000);

	/* 等待复位完成 */
	for (tout = 0; tout < 0x10000; tout++) 
	{
		regv = read_PHY (PHY_REG_BMCR);
		if (!(regv & 0x8800)) 
		{
			/* 复位完成 */
  uint8_t send_buff[50];
  uint16_t num;
  
  num = snprintf((char *)send_buff,50,"复位完成\r\n");
  USART_SetSendData(USART1, send_buff, num); 
			break;
		}
	}

	/* 第2步：配置DM9161/9162 ***********************************************************/
#if defined (_10MBIT_)
	write_PHY (PHY_REG_BMCR, PHY_FULLD_10M);  /* 连接到10Mbps的网络 */
#elif defined (_100MBIT_)
	write_PHY (PHY_REG_BMCR, PHY_FULLD_100M); /* 连接到100Mbps的网络 */
#else
	/* 通过Auto-Negotiation实现自适应10Mbps网络或者100Mbps网络 */
	write_PHY (PHY_REG_BMCR, PHY_AUTO_NEG);
	
	/* 等待完成Auto-Negotiation */
	for (tout = 0; tout < 0xF00000; tout++) 
	{
		regv = read_PHY (PHY_REG_BMSR);
		if (regv & 0x0020) 
		{
			/* 完成Auto-Negotiation */
  uint8_t send_buff[50];
  uint16_t num;
  
  num = snprintf((char *)send_buff,50,"完成自适应设置\r\n");
  USART_SetSendData(USART1, send_buff, num); 
			break;
		}
	}
#endif
	
	/* 第3步：检测连接状态 ***********************************************************/
	for (tout = 0; tout < 0xF0000; tout++) 
	{
		regv = read_PHY (PHY_REG_BMSR);
		if (regv & (1 << 2)) 
		{
  uint8_t send_buff[50];
  uint16_t num;
  
  num = snprintf((char *)send_buff,50,"连接成功\r\n");
  USART_SetSendData(USART1, send_buff, num); 
			
			/* PHY已经连接上网络 */ 
			g_ucEthLinkStatus = 1;
			
			/* 获取连接信息 */
			regv = read_PHY (PHY_REG_DSCSR);
			
			if ((regv & (1 << 15))|(regv & (1 << 13))) 
			{
				/* 全双工 */
  uint8_t send_buff[50];
  uint16_t num;
  
  num = snprintf((char *)send_buff,50,"全双工\r\n");
  USART_SetSendData(USART1, send_buff, num); 
				conn |= PHY_CON_SET_FULLD;
			}
			
			if ((regv & (1 << 15))|(regv & (1 << 14))) 
			{
				/* 速度100Mbps的网络 */
  uint8_t send_buff[50];
  uint16_t num;
  
  num = snprintf((char *)send_buff,50,"速度100Mbps的网络\r\n");
  USART_SetSendData(USART1, send_buff, num); 
				conn |= PHY_CON_SET_100M;
			}
			break;
		}
		else
		{
  uint8_t send_buff[50];
  uint16_t num;
  
  num = snprintf((char *)send_buff,50,"连接失败\r\n");
  USART_SetSendData(USART1, send_buff, num); 
			
			/* 未连接上 */ 
			g_ucEthLinkStatus = 0;
		}
	}

	/* 第4步：使能DM9161/9162中断 ***********************************************************/
	/* 使能DM9161/9162的连接中断 */
	write_PHY (PHY_REG_INTERRUPT, 1<<12);

	/* 配置引脚PH6来接收中断信号 */
	Eth_Link_EXTIConfig();

	/* 第5步：使能DM9161/9162中断 ***********************************************************/	
	/* 
	   初始化MAC配置寄存器
	   （1）当该位MCR_ROD置1时，MAC禁止在半双工模式下接收帧。
	   （2）当该位MCR_ROD清0时，MAC接收PHY发送的所有数据包。
	   （3）如果MAC在全双工模式下工作，该位不适用。
	*/
	ETH->MACCR  = MCR_ROD;

	/* 设置MAC工作在全双工模式 */
	if (conn & PHY_CON_SET_FULLD) 
	{
		/* 使能全双工 */
		ETH->MACCR |= MCR_DM;
	}

	/* 
		通过位MCR_FES配置MAC通信速度
		（1）0表示10Mbps
		（2）1表示100Mbps
	*/
	if (conn & PHY_CON_SET_100M) 
	{
		/* 配置为100Mbps */
		ETH->MACCR |= MCR_FES;
	}

	/* MACFFR 以太网帧过滤寄存器，配置可接收所有MAC组播包，即MAC地址第一个字节的bit0 = 1 */
	ETH->MACFFR = MFFR_HPF | MFFR_PAM;
	
	/* MACFCR 以太网流控制寄存器，ZQPD零时间片暂停禁止 */
	ETH->MACFCR = MFCR_ZQPD;

	/* 设置以太网MAC地址寄存器 */
	ETH->MACA0HR = ((U32)own_hw_adr[5] <<  8) | (U32)own_hw_adr[4];
	ETH->MACA0LR = ((U32)own_hw_adr[3] << 24) | (U32)own_hw_adr[2] << 16 |
				   ((U32)own_hw_adr[1] <<  8) | (U32)own_hw_adr[0];

	/* 初始化DMA发送和接收描述符 */
	rx_descr_init ();
	tx_descr_init ();

	/* 
	    刷新FIFO，启动DMA发送和接收功能
		DMAOMR 工作模式寄存器
		位20 DOMR_FTF：刷新发送 FIFO (Flush transmit FIFO)：
		               该位置1时，发送FIFO控制器逻辑会复位为默认值，因此，TX FIFO中的所有数据均会
			           丢失/刷新。刷新操作结束时该位在内部清零。此位清零之前不得对工作模式寄存器执
			           行写操作。
		位13 DOMR_ST：启动/停止发送 (Start/stop transmission)
		              该位置1时，启动发送，DMA会检查当前位置的发送列表来查找待发送的帧。
		位1  DOMR_SR：启动/停止接收 (Start/stop receive)
		              该位置1时，启动接收，DMA尝试从接收列表中获取描述符并处理传入帧。
	*/
	ETH->DMAOMR = DOMR_FTF | DOMR_ST | DOMR_SR;

	/* 使能发送和接收 */
	ETH->MACCR |= MCR_TE | MCR_RE;

	/* 复位所有MAC中断 */
	ETH->DMASR  = 0xFFFFFFFF;

	/*  
	    使能发送和接收中断
		DMAIER 中断使能寄存器
		位16 NISE：使能所有正常中断(Normal interrupt summary enable)
		位15 AISE：使能所有异常中断(Abnormal interrupt summary enable)
		位7 RBUIE：接收缓冲区不可用中断使能(Receive buffer unavailable interrupt enable)
		           当该位和AISE位都置1后，可使能接收缓冲区不可用中断。该位清零时，会禁止接
				   收缓冲区不可用中断。
		位6   RIE：接收中断使能 (Receive interrupt enable)
		           当该位和AISE都置1后，可使能接收中断。该位清零时，会禁止接收中断。
	*/
	ETH->DMAIER = ETH_DMAIER_NISE | ETH_DMAIER_AISE | ETH_DMAIER_RBUIE | ETH_DMAIER_RIE;

	/* 设置为最高优先级，仅调用NVIC->ISER设置的默认优先级也是最高优先级0 */
	NVIC_SetPriority(ETH_IRQn, 0);
	
  uint8_t send_buff1[50];
  uint16_t num1;
  
  num1 = snprintf((char *)send_buff1,50,"=======================\r\n");
  USART_SetSendData(USART1, send_buff1, num1); 

}

/*
*********************************************************************************************************
*	函 数 名: int_enable_eth
*	功能说明: 使能以太网中断
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void int_enable_eth (void) 
{
	NVIC->ISER[1] = 1 << 29;
}

/*
*********************************************************************************************************
*	函 数 名: int_disable_eth
*	功能说明: 使能以太网中断
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void int_disable_eth (void) 
{
	NVIC->ICER[1] = 1 << 29;
}

/*
*********************************************************************************************************
*	函 数 名: send_frame
*	功能说明: 传递数据帧给MAC DMA发送描述符，并使能发送。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void send_frame (OS_FRAME *frame) 
{
	U32 *sp,*dp;
	U32 i,j;

	j = TxBufIndex;
	
	/* 等待上一帧数据发送完成 */
	while (Tx_Desc[j].CtrlStat & DMA_TX_OWN);

	sp = (U32 *)&frame->data[0];
	dp = (U32 *)(Tx_Desc[j].Addr & ~3);

	/* 复制要发送的数据到DMA发送描述符中 */
	for (i = (frame->length + 3) >> 2; i; i--) 
	{
		*dp++ = *sp++;
	}
	
	/* 设置数据帧大小 */
	Tx_Desc[j].Size      = frame->length;
	
	/* 发送描述符由DMA控制发送 */
	Tx_Desc[j].CtrlStat |= DMA_TX_OWN;
	
	if (++j == NUM_TX_BUF) j = 0;
	TxBufIndex = j;
	
	/* 开始帧传输 */
	/* 
	   DMASR 以太网 DMA 状态寄存器
	   向ETH_DMASR寄存器[16:0]中的（未保留）位写入1会将其清零，写入 0 则不起作用。
	   位1 TPSS：发送过程停止状态 (Transmit process stopped status)
                 当发送停止时，此位置 1。
	*/
	ETH->DMASR   = DSR_TPSS;
	
	/*
	   DMATPDR 以太网DMA发送轮询请求寄存器
       应用程序使用此寄存器来指示DMA轮询发送描述符列表。
       位 31:0 TPD：发送轮询请求(Transmit poll demand)
                    向这些位写入任何值时，DMA都会读取ETH_DMACHTDR寄存器指向的当前描述符。如果
                    该描述符不可用（由CPU所有），则发送会返回到挂起状态，并将ETH_DMASR寄存器位2
                    进行置位。如果该描述符可用，则发送会继续进行。	   
	*/
	ETH->DMATPDR = 0;
}

/*
*********************************************************************************************************
*	函 数 名: ETH_IRQHandler
*	功能说明: 以太网中断，主要处理从MAC DMA接收描述符接收到的数据帧以及错误标志的处理。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void ETH_IRQHandler (void) 
{
	OS_FRAME *frame;
	U32 i, RxLen;
	U32 *sp,*dp;

	i = RxBufIndex;
	
	/* 循环所有接受描述符列表，遇到未接收到数据的退出循环 */
	do 
	{
		/*
			#define DMA_RX_ERROR_MASK   (DMA_RX_ES | DMA_RX_LE | DMA_RX_RWT | \
								         DMA_RX_RE | DMA_RX_CE)
			
			有错误，放弃此帧数据，错误类型包含如下：
			位15 DMA_RX_ES：错误汇总(Error summary)，即CRC错误，接收错误，看门狗超时，延迟冲突等。
		    位12 DMA_RX_LE：长度错误(Length error)
		                    该位置1时，指示接收帧的实际长度与长度/类型字段的值不符。该字段仅在帧类
		                    型位(RDES0[5])复位后有效。
			位4 DMA_RX_RWT：接收看门狗超时 (Receive watchdog timeout)
		                    该位置1时，表示接收看门狗计时器在接收当前帧时超时，且当前帧在看门狗超
		                    时后被截断了
			位3 DMA_RX_RE： 接收错误 (Receive error)
                            该位置1时，表示在帧接收期间，当发出RX_DV信号时，会发出RX_ERR信号。
			位1 DMA_RX_CE： CRC 错误(CRC error)
                            该位置1时，表示接收的帧发生循环冗余校验(CRC)错误。只有最后一个描述符
		                    (RDES0[8])置1时，该字段才有效
		*/
		if (Rx_Desc[i].Stat & DMA_RX_ERROR_MASK)
		{
			goto rel;
		}
		
		/*
			#define DMA_RX_SEG_MASK   (DMA_RX_FS | DMA_RX_LS)
		    位9 FS：第一个描述符 (First descriptor)
                    该位置1时，指示此描述符包含帧的第一个缓冲区。如果第一个缓冲区的大小为0，则第二
                    个缓冲区将包含帧的帧头。如果第二个缓冲区的大小为0，则下一个描述符将包含帧的帧头。
		
		    位8 LS：最后一个描述符 (Last descriptor)
                    该位置1时，指示此描述符指向的缓冲区为帧的最后一个缓冲区。
		
		    下面的函数用于判断此帧数据是否只有一个缓冲，初始化接收描述符列表的时候，每个描述符仅设置了
		    一个缓冲。
		*/
		if ((Rx_Desc[i].Stat & DMA_RX_SEG_MASK) != DMA_RX_SEG_MASK) 
		{
			goto rel;
		}
		
		RxLen = ((Rx_Desc[i].Stat >> 16) & 0x3FFF) - 4;
		if (RxLen > ETH_MTU) 
		{
			/* 数据包太大，直接放弃 */
			goto rel;
		}
		
		/* 申请动态内存，RxLen或上0x80000000表示动态内存不足了不会调用函数sys_error() */
		frame = alloc_mem (RxLen | 0x80000000);
		
		/* 如果动态内存申请失败了，放弃此帧数据；成功了，通过函数put_in_queue存入队列中 */
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
		
		/* 设置此接收描述符继续接收新的数据 */
		rel: Rx_Desc[i].Stat = DMA_RX_OWN;

		if (++i == NUM_RX_BUF) i = 0;
	}
	while (!(Rx_Desc[i].Stat & DMA_RX_OWN));
	
	RxBufIndex = i;

	/*
	   DMASR DMA的状态寄存器（DMA status register）
	   位7 RBUS：接收缓冲区不可用状态 (Receive buffer unavailable status)
                 此位指示接收列表中的下一个描述符由CPU所拥有，DMA无法获取。接收过程进入挂起状态。
	             要恢复处理接收描述符，CPU应更改描述符的拥有关系，然后发出接收轮询请求命令。如果
	             未发出接收轮询请求命令，则当接收到下一个识别的传入帧时，接收过程会恢复。仅当上一
	             接收描述符由DMA所拥有时，才能将ETH_DMASR[7]置1。
	
	   DMAIER的接收缓冲区不可用中断RBUIE是bit7，对于的接收缓冲区不可用状态在DMA状态寄存器中也是bit7。
	*/
	if (ETH->DMASR & INT_RBUIE) 
	{
		/* 接收缓冲区不可用，重新恢复DMA传输 */
		ETH->DMASR = ETH_DMASR_RBUS;
		ETH->DMARPDR = 0;
	}
	
	/* 
	   DMASR DMA的状态寄存器（DMA status register）
	   这里实现清除中断挂起标志 
	   位16 ETH_DMASR_NIS：所有正常中断 (Normal interrupt summary)
	   位15 ETH_DMASR_AIS：所有异常中断 (Abnormal interrupt summary)
	   位6  ETH_DMASR_RS ：接收状态 (Receive status)
	                       此位指示帧接收已完成，具体的帧状态信息已经包含在描述符中，接收仍保持运行状态。
	*/
	ETH->DMASR = ETH_DMASR_NIS | ETH_DMASR_AIS | ETH_DMASR_RS;
}

/*
*********************************************************************************************************
*	函 数 名: rx_descr_init
*	功能说明: MAC DMA接收描述符初始化。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void rx_descr_init (void) 
{
	U32 i,next;

	/*
	   1. RDES0：接收描述符字0，对应Rx_Desc[i].Stat
	      位31 OWN：所有关系位 (Own bit)
					该位置1时，指示描述符由MAC子系统的DMA所拥有。
	                该位清零时，指示描述符由主机所拥有，即CPU。
	                DMA在帧接收完成或此描述符的关联缓冲区已满时将该位清零。
	
	   2. RDES1：接收描述符字1，对应Rx_Desc[i].Ctrl
	      位14   RCH： 链接的第二个地址 (Second address chained)
					   该位置1时，表示描述符中的第二个地址是下一个描述符地址，而非第二个缓冲区地址。该
                       位置1时，RBS2(RDES1[28:16])为无关值。RDES1[15]比RDES1[14]优先处理。
	      位12:0 RBS1：接收缓冲区1大小 (Receive buffer 1 size)
                       第一个数据缓冲区的大小以字节为单位。即使RDES2（缓冲区1地址指针）的值未对齐，缓
	                   冲区大小也必须为4、8或16的倍数，具体取决于总线宽度32、64或128。如果缓冲区大小不
	                   是4、8或16的倍数，这种情况的结果是未定义。如果该字段为0，则DMA会忽略该缓冲区并
	                   使用缓冲区2或下一个描述符，具体取决于RCH（位14）的值。
	
	   3. RDES2：接收描述符字2，对应Rx_Desc[i].Addr
	      位31:0 RBAP1/RTSL：接收缓冲区1地址指针/接收帧时间戳低位
		                     Receive buffer 1 address pointer
                             Receive frame time stamp low
	
	   4. RDES3：接收描述符字3，对应Rx_Desc[i].Next
	      位31:0 RBAP2/RTSH：接收缓冲区2地址指针（下一个描述符地址）/ 接收帧时间戳高位
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
	
	/* 接收描述符列表地址寄存器指向接收描述符列表的起始处 */
	ETH->DMARDLAR = (U32)&Rx_Desc[0];
}

/*
*********************************************************************************************************
*	函 数 名: tx_descr_init
*	功能说明: MAC DMA发送描述符初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void tx_descr_init (void) 
{
	U32 i,next;

	/*
	   1. TDES0：发送描述符字0，对应Tx_Desc[i].CtrlStat
	      位29 LS ：末段 (Last segment)
                    该位置1时，指示缓冲区中包含帧的末段。
	      位28 FS ：首段 (First segment)
                    该位置1时，指示缓冲区中包含帧的首段
          位20 TCH：链接的第二个地址 (Second address chained)
                    该位置1时，表示描述符中的第二个地址是下一个描述符地址，而非第二个缓冲区地址。
                    TDES0[20]置1时，TBS2(TDES1[28:16])为无关值。TDES0[21]比TDES0[20]优先处理。
	
	   2. TDES1：发送描述符字1，对应Tx_Desc[i].Size
	
	   3. TDES2：发送描述符字2，对应Tx_Desc[i].Addr
	      位31:0 TBAP1：发送缓冲区1地址指针/发送帧时间戳低位 
	                    Transmit buffer 1 address pointer / Transmitframe time stamp low
	
	   4. TDES3：发送描述符字3，对应Tx_Desc[i].Next
	      位 1:0 TBAP2：发送缓冲区2地址指针（下一个描述符地址）/ 发送帧时间戳高位
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
	
	/* 发送描述符列表地址寄存器指向发送描述符列表的起始处 */
	ETH->DMATDLAR = (U32)&Tx_Desc[0];
}


/*
*********************************************************************************************************
*	函 数 名: write_PHY
*	功能说明: 写数据到PHY寄存器
*	形    参: PhyReg  PHY寄存器地址
*             Value   写入的数据
*	返 回 值: 无
*********************************************************************************************************
*/
static void write_PHY (U32 PhyReg, U16 Value)
{
	U32 tout;

	/* 数据寄存器，存入要写入PHY的16位数据 */
	ETH->MACMIIDR = Value;

	/*
	   MACMIIAR 以太网MAC MII 地址寄存器
       位 15:11 PA：PHY 地址 (PHY address)，对应这里的PHY_DEF_ADDR
					该字段指示正在访问32个可能的PHY器件中的哪一个。	
	   
	   位 10:6  MR：MII寄存器 (MII register)，对应这里的PhyReg
					这些位在所选的PHY器件中选择要访问的MII寄存器。
	   位 1     MW：MII写(MII write)，对应这里的MMAR_MW
	                此位置1是在告知PHY，将要启动一个使用MII数据寄存器的写操作。
	                如果此位未置，则表示会启动一个读操作，将数据放入MII数据寄存器。
	   位 0     MB：MII忙碌 (MII busy)，对应这里的MMAR_MB
	                向ETH_MACMIIAR和ETH_MACMIIDR写入前，此位应读取逻辑0。向ETH_MACMIIAR写入过程中，此
	                位也必须复位为0。在PHY寄存器访问过程中，此位由应用程序置1，指示读或写访问正在进行中。
	                在对PHY进行写操作过程中，ETH_MACMIIDR（MII数据）应始终保持有效，直到MAC将此位清零。
	                在对PHY进行读操作过程中，ETH_MACMIIDR始终无效，直到MAC将此位清零。在此位清零后，才
					可以向ETH_MACMIIAR（MII地址）写入。
	*/
	ETH->MACMIIAR = PHY_DEF_ADDR << 11 | PhyReg << 6 | MMAR_MW | MMAR_MB;

	/* 等待操作完成，即等待MMAR_MB位被清零 */
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
*	函 数 名: read_PHY
*	功能说明: 从PHY芯片中读取寄存器数据
*	形    参: PhyReg  寄存器地址
*	返 回 值: 无
*********************************************************************************************************
*/
static U16 read_PHY (U32 PhyReg) 
{
	U32 tout;

	/*
	   MACMIIAR 以太网MAC MII 地址寄存器
       位 15:11 PA：PHY 地址 (PHY address)，对应这里的PHY_DEF_ADDR
					该字段指示正在访问32个可能的PHY器件中的哪一个。	
	   
	   位 10:6  MR：MII寄存器 (MII register)，对应这里的PhyReg
					这些位在所选的PHY器件中选择要访问的MII寄存器。
	   位 1     MW：MII写(MII write)，对应这里的MMAR_MW
	                此位置1是在告知PHY，将要启动一个使用MII数据寄存器的写操作。
	                如果此位未置，则表示会启动一个读操作，将数据放入MII数据寄存器。
	   位 0     MB：MII忙碌 (MII busy)，对应这里的MMAR_MB
	                向ETH_MACMIIAR和ETH_MACMIIDR写入前，此位应读取逻辑0。向ETH_MACMIIAR写入过程中，此
	                位也必须复位为0。在PHY寄存器访问过程中，此位由应用程序置1，指示读或写访问正在进行中。
	                在对PHY进行写操作过程中，ETH_MACMIIDR（MII数据）应始终保持有效，直到MAC将此位清零。
	                在对PHY进行读操作过程中，ETH_MACMIIDR始终无效，直到MAC将此位清零。在此位清零后，才
					可以向ETH_MACMIIAR（MII地址）写入。
	*/
	ETH->MACMIIAR = PHY_DEF_ADDR << 11 | PhyReg << 6 | MMAR_MB;

	/* 等待操作完成，即等待MMAR_MB位被清零 */
	tout = 0;
	for (tout = 0; tout < MII_RD_TOUT; tout++) 
	{
		if ((ETH->MACMIIAR & MMAR_MB) == 0) 
		{
			break;
		}
	}
	
	/* 从 PHY 中读取16bit的数据值 */
	return (ETH->MACMIIDR & MMDR_MD);
}

/*
*********************************************************************************************************
*	函 数 名: Eth_Link_EXTIConfig
*	功能说明: PHY的中断触发使用STM32的PH6引脚识别，此功能主要用于检测网线的连接状态判别。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void Eth_Link_EXTIConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 安富莱STM32-V6开发板使用PH6作为中断输入口, 下降沿表示中断信号 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* 配置中断引脚是输入 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOH, &GPIO_InitStructure);

	/* 配置外部中断线连接到相应引脚 */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOH, EXTI_PinSource6);

	/* 配置外部中断线 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* 使能中断通道 */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: EXTI9_5_IRQHandler
*	功能说明: PH6引脚的中断处理
*	形    参: 无
*	返 回 值: 无
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
		/* 可以考虑在此处加入延迟，有时连接状态变了，但是寄存器没有及时更新*/
		regv = read_PHY(PHY_REG_INTERRUPT);
		if(regv & (1 << 2))
		{
			/* 重新插入后要多读几次，保证寄存器BMSR被更新 */
			for(tout = 0; tout < 10; tout++) 
			{
				regv = read_PHY (PHY_REG_BMSR);
				if (regv & (1 << 2)) 
				{
					break;
				}
			}

			/* 连接上网线 */
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
						/* 等待发送结束 */
						while ((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);
					}
				#endif 
			}
			/* 网线断开 */
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
						/* 等待发送结束 */
						while ((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);
					}
				#endif
				
			}
			
		}
		/* 清中断挂起位 */
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
