#ifndef NAV_DECODE_H
#define NAV_DECODE_H

#include <stdint.h>
#include "usr_FreeRTOS.h"

extern QueueHandle_t nav_queue;

typedef enum
{
  nav_ok,
  cmd_ok,
  err  
  
}nav_data_status;


#pragma pack(push)
#pragma pack(1)

typedef struct tagSysTime
{
  uint32_t index;
	uint16_t week;
	uint32_t tow_imu;
}SysTime;

typedef struct tagDtcNav
{
  double  	lat;     /* lat in degree */
  double  	lon;     /* lon in degree */
  float   	alt;     /* alt in m */
  float   	ve;      /* ve in m/s */
  float   	vn;      /* vn in m/s */
  float   	vu;      /* vu in m/s */
  float   	pitch;   /* pitch in degree */
  float   	roll;    /* roll in degree */
  float   	heading; /* heading in degree */
	/* add quat  */
	float   	quat[4];
  float   	tilt;    /* tilt angular */
  float   	tilt_quality; /* tilt angular quality  */
  float   	direction_of_tilt;
	float   	direction_quality;
	float   	overall_tilt_quality;
	uint16_t  work_states;
	int     resv[4];
} DtcNav;

typedef struct tagDtcImuOut
{
  uint16_t flag;
  float  gyox;
  float  gyoy;
  float  gyoz;
  int16_t  tmp_gyo;
  float  accx;
  float  accy;
  float  accz;
  int16_t  tmp_acc;
}DtcImuOut;

typedef struct tagDtcGnssOut
{
	uint16_t  week;
  uint32_t  tow;
  uint16_t  fix_type;
  double   	lat;
  double   	lon;
  float    	alt;
  float    	spd;
  float    	heading;
  float    	orient;
  uint16_t  hdop;
} DtcGnssOut;

typedef struct tagDtcDebugInfo
{
  int  resv[4];
} DtcDebugInfo;


typedef struct DtcSendArm3Data
{
  unsigned short  head;
	SysTime         time;
  DtcNav          nav;     /* Nav Data */
  DtcImuOut       imu;    /* VTI Imu Data */
  DtcGnssOut      gnss;    /* Gnss data */
  unsigned short  check;
} DtcArm2Data;

#pragma pack(pop)


void decode_nav(uint8_t* data,uint32_t data_num);
void NavDataQueueInit(void);

#endif
