/*============================================================================*/
/* Copyright (C), 2016, Yagro Co., Ltd.                                       */
/* File name:   GNSS.h                                                        */
/* Date:        2016.7.18                                                     */
/* Description:                                                               */
/*============================================================================*/

#ifndef __UMI_GNSS_H__
#define __UMI_GNSS_H__

/*============================================================================*/
/*                               Header include                               */
/*============================================================================*/
#include <stdint.h>

/*============================================================================*/
/*                                   Macros                                   */
/*============================================================================*/
#define MILE2KM           (1852.0f / 3600.0f)
#define _D2R              (0.01745329251994f)
#define CRC32_POLYNOMIAL  (0xEDB88320L)
#define _UTC2GPS          (0) //(17000)

#define _NONE             (0)
#define _GPGGA            (1)
#define _GPRMC            (2)
#define _GPHDT            (3)
#define _BESTVELA         (4)
#define _HEADINGA         (5)

#define GPS_GPSDATA_IN    (0x8000)
#define GPS_GPGGA_IN      (0x0001)
#define GPS_GPRMC_IN      (0x0002)
#define GPS_GPHDT_IN      (0x0004)
#define GPS_BESTVELA_IN   (0x0008)
#define GPS_HEADINGA_IN   (0x0010)

#define GNSS_POSITIONING  (1 << 0)
#define GNSS_DUAL_ANTENNA (1 << 1)
#define GNSS_SINGLE_POINT (1 << 10)
#define GNSS_DGPS         (1 << 11)
#define GNSS_FLOAT        (1 << 12)
#define GNSS_FIXED        (1 << 13)
#define GNSS_DATA         (1 << 15)

#define USER_NMEA_0       (0) /* GPGGAºÍGPRMC */
#define USER_NMEA_1       (1) /* GPGGA¡¢GPRMCºÍHEADINGA */
#define USER_NMEA_2       (2) /* GPGGA¡¢BESTVELAºÍHEADINGA */
#define USER_NMEA         USER_NMEA_0

/*============================================================================*/
/*                             struct define                                  */
/*============================================================================*/
#pragma pack(push)
#pragma pack(1)

typedef struct
{
  uint16_t u16Flag;
  uint16_t u16Week;
  uint32_t u32UTC;
  uint16_t sv_num;
  double   dLatitude;
  double   dLongitude;
  float    fAltitude;
  float    fVgroud;
  float    fHeading;
  float    fAzi;
  uint16_t rtcm_age;
  uint16_t hdop;
}stGnssOut;

typedef struct
{
  uint8_t  u8Sync;
  uint8_t  u8Message[10];
  uint8_t  u8Port[5];
  uint8_t  u8Sequence[20];
  uint8_t  u8IdleTime[20];
  uint8_t  u8TimeStatus[20];
  uint16_t u16Week;
  double   dGpsSec;
  uint8_t  u8ReceiverStatus[10];
  uint8_t  u8Reserved[4];
  uint8_t  u8Version[4];
}stLogHeader;

typedef struct
{
  double   dUtcTime;
  double   dLatitude;
  double   dLongitude;
  float    fAltitude;
  uint8_t  Num_Satellites;
  uint8_t  uFS;
  float    fHdop;
  uint8_t  u8NoSV;
  float    fAltref;
  uint32_t u32DiffTime;
  uint16_t u16DiffId;
  uint8_t  u8GpggaDataFlag;
  uint8_t  u8RtkFlag;
  uint8_t  u8Gnss_Mode;
  uint8_t  u8GPS_State;
}stGpgga;

typedef struct
{
  double   dUtcTime;
  uint8_t  u8Data_valid;
  double   dLatitude;
  double   dLongitude;
  float    fVgoroud;
  float    fHeading;
  uint32_t u32Date;
  float    fMagn_Azi;
  uint8_t  uSystme_mode;
}stGprmc;

typedef struct
{
  float   fAzi;
  uint8_t uTrue;
}stGphdt;

typedef struct
{
  stLogHeader m_stLogHeader;
  uint8_t     u8Age[10];
  uint8_t     u8SolStatus[20];
  uint8_t     u8VelType[20];
  uint8_t     u8Latency[10];
  uint8_t     u8Reserved[4];
  float       fTrack;
  float       fVGround;
  float       fVe;
  float       fVn;
  float       fVu;
  uint8_t     u8SolutionStatus;
  uint8_t     u8P_VType;
  uint8_t     u8VDataFlag;
  uint8_t     u8TimeType;
}stBestVela;

typedef struct
{
  stLogHeader m_stLogHeader;
  uint8_t     strSolStat[30];
  uint8_t     strPosType[20];
  uint8_t     strBaseLine[20];
  uint32_t    u32SolStat;
  float       fBaseline;
  float       fPitch;
  float       fHeading;
  float       fHdg_std;
  float       fPtch_std;
  int8_t      cStnID[4];
  uint8_t     uSVs;
  uint16_t    uSolnSVs;
  uint16_t    uObs;
  uint16_t    uMulti;
  uint8_t     u8SolSource;
  uint8_t     u8SigMask_1;
  uint8_t     u8SigMask_2;
  uint8_t     ucReserved1;
  uint8_t     ucExtSolStat;
  uint8_t     ucReserved2;
  uint8_t     ucSigMask;
  uint16_t    uP_VType;
  uint16_t    uHeadingaFlag;
}stHeadinga;

typedef struct
{
  stGpgga  m_stGpgga;
  stGprmc  m_stGprmc;
  stGphdt  m_stGphdt;
  uint16_t u16Flag;
}stNMEA;

typedef struct
{
  stNMEA     m_stNMEA;
  stBestVela m_stBestVela;
  stHeadinga m_stHeadinga;
  uint16_t   u16Flag;
}stGnssIn;

typedef struct
{
  uint8_t  u8Vec_Package[256];
  uint16_t u16Num_Package;
  uint16_t u16Pointer;
  uint8_t  u8Start_Package;
}stPackage;

typedef struct
{
  uint8_t  u8Hour;
  uint8_t  u8Minute;
  uint8_t  u8Second;
  uint16_t u16MilSecond;
}stUtcData;

typedef struct
{
	uint16_t  head;
	uint16_t  week_num;
	uint32_t  week_second;
  
	double    lat;
	double    lon;
  
	float     alt;
  
	uint16_t  flag;
	float     spd;
	float     heading;
	uint16_t  hdop;
	float     orient;
  
	uint8_t   check;
}GnssOutPut;

#pragma pack(pop)

/*============================================================================*/
/*                               declaration                                  */
/*============================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void Package_Rec(uint8_t *pBuffer, uint32_t u16Num, stGnssIn *m_stGnssIn);
void Combain_GNSS(stGnssIn *pstGNSSIn, stGnssOut *pstGNSSOut);
	
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
