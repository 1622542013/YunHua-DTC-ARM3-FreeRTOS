#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__

#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>

#include "FreeRTOS.h"

#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "event_groups.h"
#include "semphr.h"
#include <Net_Config.h>

/*�����¼���־�飬���RLЧ�ʣ���ʱ����main_TcpNet();*/
#define MAIN_TCP   (1<<0)
extern  EventGroupHandle_t Main_tcpnet_group;

#endif
