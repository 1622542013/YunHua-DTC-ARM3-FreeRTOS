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


#define SYSTEM_CYCLE     10  /*10ms data cycle*/


#define SD_TIME_OK_BIT   (1<<1)

extern EventGroupHandle_t Event_group;

#endif
