#include "nav_decode.h"
#include "globals.h"
#include <string.h>

#define NAV_HEAD_0      (0xCD)
#define NAV_HEAD_1      (0x34)

QueueHandle_t nav_queue = NULL;
void NavDataQueueInit(void)
{
  nav_queue = xQueueCreate(1,sizeof(DtcArm2Data));
}

static DtcArm2Data nav_data;
void decode_nav(uint8_t* data,uint32_t data_num)
{
  uint8_t  result = err; 
  
  uint32_t i = 0; 
  uint16_t  check = 0;
  uint16_t  rev_check = 0;
  
  uint32_t  nav_data_length = sizeof(DtcArm2Data);
  
  for(;i<data_num;i++)
  {
    if((data[i] == NAV_HEAD_0)&&(data[i+1] == NAV_HEAD_1)&&((data_num - i) >= nav_data_length))
    {
       check = CheckSum(&data[i], nav_data_length);
       rev_check = (data[i+nav_data_length - 1] << 8) | data[i+ nav_data_length - 2];
         
       if(check == rev_check)/*nav data is right*/
       {
          memcpy(&nav_data,&data[i],sizeof(DtcArm2Data));
          
          xQueueSend(nav_queue,&nav_data,SYSTEM_CYCLE);
       }
    }
  }
}



