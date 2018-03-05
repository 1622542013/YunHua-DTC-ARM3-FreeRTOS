#include "usart4.h"

uint8_t UART4_Tx_Buffer[UART4_Tx_BufferSize];
uint8_t UART4_Rx_Buffer[UART4_Rx_BufferSize];

/*-------------------------------------  USART Init  ------------------------------------------*/
void UART4_Gpio_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
/* enable gpio clock */ 
  RCC_AHB1PeriphClockCmd(UART4_Tx_Clock, ENABLE);  /* Enable UART4_Tx GPIO clock */
  RCC_AHB1PeriphClockCmd(UART4_Rx_Clock, ENABLE);  /* Enable UART4_Rx GPIO clock */

/* selects the pin to used as Alternate function */   
  GPIO_PinAFConfig(UART4_Tx_IO_Group, UART4_Tx_PinSource, GPIO_AF_UART4); /* Connect GPIOxn to UART4_Tx */
  GPIO_PinAFConfig(UART4_Rx_IO_Group, UART4_Rx_PinSource, GPIO_AF_UART4); /* Connect GPIOxn to UART4_Rx */

  GPIO_InitStructure.GPIO_Pin = UART4_Tx_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(UART4_Tx_IO_Group, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = UART4_Rx_Pin;
  GPIO_Init(UART4_Rx_IO_Group, &GPIO_InitStructure);
}

void UART4_Config(uint32_t BaudRate)
{
  USART_InitTypeDef USART_InitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);     /* Enable UART4 clock */

  USART_InitStructure.USART_BaudRate = BaudRate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
  
  USART_Init(UART4, &USART_InitStructure);
  USART_Cmd(UART4, ENABLE);
}

void UART4_Irq_Config(uint16_t USART_irq_mode)
{
  if(USART_irq_mode == NO_USART_IRQ)
  {
    USART_ITConfig(UART4, USART_irq_mode, DISABLE);
    return;
  }
  
  if( (USART_irq_mode & USART_IT_PE) == USART_IT_PE )
  {
    USART_ITConfig(UART4, USART_IT_PE, ENABLE); /* Enabling interrupts specified UART4 */
  }
  if( (USART_irq_mode & USART_IT_TXE) == USART_IT_TXE )
  {
    USART_ITConfig(UART4, USART_IT_TXE, ENABLE); /* Enabling interrupts specified UART4 */
  }
  if( (USART_irq_mode & USART_IT_TC) == USART_IT_TC )
  {
    USART_ITConfig(UART4, USART_IT_TC, ENABLE); /* Enabling interrupts specified UART4 */
  }
  if( (USART_irq_mode & USART_IT_RXNE) == USART_IT_RXNE )
  {
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE); /* Enabling interrupts specified UART4 */
  }
  if( (USART_irq_mode & USART_IT_ORE_RX) == USART_IT_ORE_RX )
  {
    USART_ITConfig(UART4, USART_IT_ORE_RX, ENABLE); /* Enabling interrupts specified UART4 */
  }
  if( (USART_irq_mode & USART_IT_IDLE) == USART_IT_IDLE )
  {
    USART_ITConfig(UART4, USART_IT_IDLE, ENABLE); /* Enabling interrupts specified UART4 */
  }
  if( (USART_irq_mode & USART_IT_LBD) == USART_IT_LBD )
  {
    USART_ITConfig(UART4, USART_IT_LBD, ENABLE); /* Enabling interrupts specified UART4 */
  }
  if( (USART_irq_mode & USART_IT_CTS) == USART_IT_CTS )
  {
    USART_ITConfig(UART4, USART_IT_CTS, ENABLE); /* Enabling interrupts specified UART4 */
  }
  if( (USART_irq_mode & USART_IT_ERR) == USART_IT_ERR )
  {
    USART_ITConfig(UART4, USART_IT_ERR, ENABLE); /* Enabling interrupts specified UART4 */
  }
  if( (USART_irq_mode & USART_IT_ORE_ER) == USART_IT_ORE_ER )
  {
    USART_ITConfig(UART4, USART_IT_ORE_ER, ENABLE); /* Enabling interrupts specified UART4 */
  }
  if( (USART_irq_mode & USART_IT_NE) == USART_IT_NE )
  {
    USART_ITConfig(UART4, USART_IT_NE, ENABLE); /* Enabling interrupts specified UART4 */
  }
  if( (USART_irq_mode & USART_IT_FE) == USART_IT_FE )
  {
    USART_ITConfig(UART4, USART_IT_FE, ENABLE); /* Enabling interrupts specified UART4 */
  }
}

void UART4_DMA_Config(FunctionalState NewState)
{
  DMA_InitTypeDef DMA_InitStructure;
  
  if(NewState == DISABLE)
  {
     USART_DMACmd(UART4, USART_DMAReq_Tx, DISABLE);  
     USART_DMACmd(UART4, USART_DMAReq_Rx, DISABLE); 

     return;
  }
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

/*----------------------  DMA TX  -----------------------------*/
  
  USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE);  
  DMA_DeInit(UART4_TX_DMA_Source);
  while(DMA_GetCmdStatus(UART4_TX_DMA_Source) != DISABLE);

  DMA_InitStructure.DMA_Channel = UART4_TX_DMA_CH; /* Channel selection */
/* Set the DMA source: peripheral data register address */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART4->DR; 

  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)UART4_Tx_Buffer;     /* Memory address */
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                 /* Data Memory to Peripheral */
  DMA_InitStructure.DMA_BufferSize = UART4_Tx_BufferSize;                      /* Specifies the DMA channel DMA buffer size */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        /* Peripheral address does not increase */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 /* Memory address increment */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; /* Peripheral data width of 8 bit */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         /* Memory data width of 8 bit */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           /* Use Normal mode */
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                   /* Medium priority */
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;             /* A single transmission burst memory */
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;     /* Peripherals single transmission burst */

  DMA_Init(UART4_TX_DMA_Source, &DMA_InitStructure);
  DMA_Cmd(UART4_TX_DMA_Source, ENABLE);

/* Starting a byte of data to make a DMA transmission completion flag */
  DMA_ClearFlag(UART4_TX_DMA_Source, UART4_TX_DMA_flag);  /* Clear Pending DMA channel x flag (DMA2 Stream7 transfer completion flag) */
  DMA_Cmd(UART4_TX_DMA_Source, DISABLE);
  while(DMA_GetCmdStatus(UART4_TX_DMA_Source) != DISABLE);
  DMA_SetCurrDataCounter(UART4_TX_DMA_Source, 1);
  DMA_Cmd(UART4_TX_DMA_Source, ENABLE);
   
/*----------------------  DMA RX  -----------------------------*/ 
  USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);

  DMA_DeInit(UART4_RX_DMA_Source);
  while(DMA_GetCmdStatus(UART4_RX_DMA_Source) != DISABLE);

  DMA_InitStructure.DMA_Channel = UART4_RX_DMA_CH;  /* Channel selection */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART4->DR; 
  
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)UART4_Rx_Buffer;            /* Memory address */
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                 /* Data Peripheral to Memory */
  DMA_InitStructure.DMA_BufferSize = UART4_Rx_BufferSize;                      /* Specifies the DMA channel DMA buffer size */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        /* Peripheral address does not increase */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 /* Memory address increment */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; /* Peripheral data width of 8 bit */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         /* Memory data width of 8 bit */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                         /* Use Circular mode */
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                   /* Medium priority */
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;             /* A single transmission burst memory */
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;     /* Peripherals single transmission burst */

  DMA_Init(UART4_RX_DMA_Source, &DMA_InitStructure);
  DMA_Cmd(UART4_RX_DMA_Source, ENABLE);
}

void UART4_Init(uint32_t BaudRate,uint16_t USART_irq_mode,FunctionalState Dma_swtich)
{
  UART4_Gpio_Init();
    
  UART4_Config(BaudRate);
  
  UART4_Irq_Config(USART_irq_mode);
  
  UART4_DMA_Config(Dma_swtich);
}

/*-------------------------------------  USART Send  ------------------------------------------*/

void UART4_DMA_Send(uint8_t* send_buff,uint16_t send_size)
{
  while(DMA_GetFlagStatus(UART4_TX_DMA_Source, UART4_TX_DMA_flag)==0);

  memcpy(UART4_Tx_Buffer, send_buff, send_size); /* Copy memory */
  
  DMA_ClearFlag(UART4_TX_DMA_Source, UART4_TX_DMA_flag);  /* Clear Pending DMA channel x flag (DMA2 Stream7 transfer completion flag) */
  DMA_Cmd(UART4_TX_DMA_Source, DISABLE);
  while(DMA_GetCmdStatus(UART4_TX_DMA_Source) != DISABLE);
  DMA_SetCurrDataCounter(UART4_TX_DMA_Source, send_size);
  DMA_Cmd(UART4_TX_DMA_Source, ENABLE);
}

/*-------------------------------------  USART Receive  ------------------------------------------*/

uint16_t UART4_GetReceiveDataNumber(void)
{
  uint16_t number = 0;
  
  number = UART4_Rx_BufferSize - DMA_GetCurrDataCounter(UART4_RX_DMA_Source);
  
  return number;
}

void UART4_Reset_RX(void)
{
  memset(UART4_Rx_Buffer, 0, UART4_Rx_BufferSize);  /* Empty memory */
  
  DMA_ClearFlag(UART4_RX_DMA_Source, UART4_RX_DMA_flag);  /* Clear Pending DMA channel x flag (DMA2 DMA2_Stream2 transfer completion flag) */
  DMA_Cmd(UART4_RX_DMA_Source, DISABLE);
  while(DMA_GetCmdStatus(UART4_RX_DMA_Source) != DISABLE);
  DMA_SetCurrDataCounter(UART4_RX_DMA_Source, UART4_Rx_BufferSize);
  DMA_Cmd(UART4_RX_DMA_Source, ENABLE);
}

/*-------------------------------------  USART app  ------------------------------------------*/


void UART4_Send_data(uint8_t* out_buff,uint16_t out_size)
{  
   UART4_DMA_Send(out_buff,out_size);  
}
