/*============================================================================*/
/* Copyright (C), 2016, Yagro Co., Ltd.                                       */
/* File name:   gnss.c                                                        */
/* Date:        2016.7.18                                                     */
/* Description: gnss.c                                                        */
/*============================================================================*/

/*============================================================================*/
/*                               Header include                               */
/*============================================================================*/
#include "GNSS.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

/*============================================================================*/
/*                              Global variables                              */
/*============================================================================*/
static stPackage m_stPackage = {0};

/*============================================================================*/
/*                            Function declaration                            */
/*============================================================================*/
static void     GNSS_Decode(stPackage *pstPackage, stGnssIn *pstGNSSIn);
static uint8_t  Decode_Gpgga(stPackage *pstPackage, stGnssIn *pstGNSSIn);
static uint8_t  Decode_Gprmc(stPackage *pstPackage, stGnssIn *pstGNSSIn);
static uint8_t  Decode_Gphdt(stPackage *pstPackage, stGnssIn *pstGNSSIn);
static uint8_t  Decode_BestVela(stPackage *pstPackage, stGnssIn *pstGNSSIn);
static uint8_t  Decode_Headinga(stPackage *pstPackage, stGnssIn *pstGNSSIn);
static uint8_t  Test_P_V_Type(const char *DataBuffer, int Length);
static uint8_t  Test_SolStat(const char *DataBuffer, int Length);
static uint8_t  Checmsum_Novtel(uint8_t *ucBuffer, uint32_t ulCount);
static uint8_t  CheckSum_NMEA(const uint8_t *u8Buffer, uint16_t u16Count);
static uint32_t Asc_Int2Int(uint8_t *Buffer);
static uint32_t CalculateBlockCRC32(uint32_t ulCount, uint8_t *ucBuffer);
static uint32_t CRC32Value(int32_t i);
static uint8_t  ASCII2u8(uint8_t u8Data);
static double   Ddmm2dd(double ddmm);
static uint32_t hhmmss2ss(double fddmm);
static int Date2Week(int date, float time);
static int ReturnWeekDay(int date);

/*============================================================================*/
/*                            Function definition                             */
/*============================================================================*/

void Package_Rec(uint8_t *pBuffer, uint32_t u16Num, stGnssIn *m_stGnssIn)
{
  for(uint32_t i = 0; i < u16Num; i++)
  {
    uint8_t u8Byte = pBuffer[i];
    
    switch(u8Byte)
    {
      case '$':
      case '#':
      {
        m_stPackage.u8Start_Package                         = 1;
        m_stPackage.u8Vec_Package[m_stPackage.u16Pointer++] = u8Byte;
      }
      break;
      
      case 0x0A:
      {
        if(m_stPackage.u8Start_Package == 1)
        {
          m_stPackage.u8Start_Package                       = 0;
          m_stPackage.u8Vec_Package[m_stPackage.u16Pointer] = u8Byte;
          m_stPackage.u16Num_Package                        = m_stPackage.u16Pointer + 1;
          
          GNSS_Decode(&m_stPackage, m_stGnssIn);
          memset(&m_stPackage, 0, sizeof(m_stPackage));
        }
      }
      break;
      
      default:
      {
        if(m_stPackage.u8Start_Package == 1)
        {
          m_stPackage.u8Vec_Package[m_stPackage.u16Pointer++] = u8Byte;
        }
      }
    }
  }
}

static void GNSS_Decode(stPackage *pstPackage, stGnssIn *pstGNSSIn)
{
  if(!memcmp("$GPGGA,", pstPackage->u8Vec_Package, 7))
  {
    if(_GPGGA == Decode_Gpgga(pstPackage, pstGNSSIn))
    {
      pstGNSSIn->u16Flag |= GPS_GPGGA_IN;
    }
  }
  else if(!memcmp("$GPRMC,", pstPackage->u8Vec_Package, 7))
  {
    if(_GPRMC == Decode_Gprmc(pstPackage, pstGNSSIn))
    {
      pstGNSSIn->u16Flag |= GPS_GPRMC_IN;
    }
  }
  else if(!memcmp("$GPHDT,", pstPackage->u8Vec_Package, 7))
  {
    if(_GPHDT == Decode_Gphdt(pstPackage, pstGNSSIn))
    {
      pstGNSSIn->u16Flag |= GPS_GPHDT_IN;
    }
  }
  else if(!memcmp("#BESTVELA,", pstPackage->u8Vec_Package, 10))
  {
    if(_BESTVELA == Decode_BestVela(pstPackage, pstGNSSIn))
    {
      pstGNSSIn->u16Flag |= GPS_BESTVELA_IN;
    }
  }
  else if(!memcmp("#HEADINGA,", pstPackage->u8Vec_Package, 10))
  {
    if(_HEADINGA == Decode_Headinga(pstPackage, pstGNSSIn))
    {
      pstGNSSIn->u16Flag |= GPS_HEADINGA_IN;
    }
  }
}

static uint8_t Decode_Gpgga(stPackage *pstPackage, stGnssIn *pstGNSSIn)
{
  uint8_t  u8DataSum      =  0;
  char     TempStr[50]    = {0};
  uint16_t u16TempStrLeng =  0;

  if(!CheckSum_NMEA(pstPackage->u8Vec_Package, pstPackage->u16Num_Package))
  {
    return _NONE;
  }
  
  for(int i = 7; i < (pstPackage->u16Num_Package - 2); i++)
  {
    if(!((pstPackage->u8Vec_Package[i] == 44) || (pstPackage->u8Vec_Package[i] == 42)))
    {
      TempStr[u16TempStrLeng] = pstPackage->u8Vec_Package[i];
      u16TempStrLeng++;
    }
    else
    {
      u8DataSum++;
      
      switch(u8DataSum)
      {
        case 1:
        {
          pstGNSSIn->m_stNMEA.m_stGpgga.dUtcTime = strtod(TempStr, 0);
          
          break;
        }
        
        case 2:
        {
          pstGNSSIn->m_stNMEA.m_stGpgga.dLatitude = Ddmm2dd(strtod(TempStr, 0));
          
          break;
        }
        
        case 3:
        {
          if(TempStr[0] == 'S')
          {
            pstGNSSIn->m_stNMEA.m_stGpgga.dLatitude = pstGNSSIn->m_stNMEA.m_stGpgga.dLatitude * (-1.0);
          }
          
          break;
        }
        
        case 4:
        {
          pstGNSSIn->m_stNMEA.m_stGpgga.dLongitude = Ddmm2dd(strtod(TempStr, 0));
          
          break;
        }
        
        case 5:
        {
          if(TempStr[0] == 'W')
          {
            pstGNSSIn->m_stNMEA.m_stGpgga.dLongitude = pstGNSSIn->m_stNMEA.m_stGpgga.dLongitude * (-1.0);
          }
          
          break;
        }
        
        case 6:
        {
          pstGNSSIn->m_stNMEA.m_stGpgga.u8GPS_State  = atoi(TempStr);
          
          if(atoi(TempStr) > 0)
          {
             pstGNSSIn->m_stNMEA.m_stGpgga.u8GpggaDataFlag = 1;
          }
          
          if(atoi(TempStr) == 6)
          {
             pstGNSSIn->m_stNMEA.m_stGpgga.u8GpggaDataFlag = 0;
          }
          
          if((atoi(TempStr) == 4) || (atoi(TempStr) == 5))
          {
            pstGNSSIn->m_stNMEA.m_stGpgga.u8RtkFlag = 1;
          }
          else
          {
            pstGNSSIn->m_stNMEA.m_stGpgga.u8RtkFlag = 0;
          }
          
          break;
        }
        
        case 7:
        {
          pstGNSSIn->m_stNMEA.m_stGpgga.Num_Satellites = atoi(TempStr);
          
          if(pstGNSSIn->m_stNMEA.m_stGpgga.Num_Satellites < 4)
          {
            pstGNSSIn->m_stNMEA.m_stGpgga.u8GpggaDataFlag = 0;
            pstGNSSIn->m_stNMEA.m_stGpgga.u8RtkFlag       = 0;
          }
          
          break;
        }
        
        case 8:
        {
          pstGNSSIn->m_stNMEA.m_stGpgga.fHdop = atof(TempStr);
          
          break;
        }
        
        case 9:
        {
          pstGNSSIn->m_stNMEA.m_stGpgga.fAltitude = atof(TempStr);
          
          break;
        }
        
        case 10:
        {
          break;
        }
        
        case 11:
        {
          /* Undulation - the relationship between the geoid and the WGS84 ellipsoid */
          pstGNSSIn->m_stNMEA.m_stGpgga.fAltref    = atof(TempStr);
          pstGNSSIn->m_stNMEA.m_stGpgga.fAltitude += pstGNSSIn->m_stNMEA.m_stGpgga.fAltref;
          
          break;
        }
        
        case 12:
        {
          break;
        }
        
        case 13:
        {
          /* age Age of Differential GPS data (in seconds) */
          pstGNSSIn->m_stNMEA.m_stGpgga.u32DiffTime = atoi(TempStr);
          
          break;
        }
        
        case 14:
        {
          /* stn ID Differential base station ID, 0000-1023 */
          pstGNSSIn->m_stNMEA.m_stGpgga.u16DiffId = atoi(TempStr);
          
          break;
        }
      }
      
      memset(TempStr, 0, sizeof(TempStr));
      u16TempStrLeng = 0;
    }
  }
  
  return _GPGGA;
}

static uint8_t Decode_Gprmc(stPackage *pstPackage, stGnssIn *pstGNSSIn)
{
  uint8_t  datenum     =  0;
  char     TempStr[50] = {0};
  uint16_t TempStrLeng =  0;
  
  if(!CheckSum_NMEA(pstPackage->u8Vec_Package,pstPackage->u16Num_Package))
  {
    return _NONE;
  }
  
  for(int i = 7; i < (pstPackage->u16Num_Package - 2); i++)
  {
    if(!((pstPackage->u8Vec_Package[i] == 44) || (pstPackage->u8Vec_Package[i] == 42)))
    {
      TempStr[TempStrLeng] = pstPackage->u8Vec_Package[i];
      TempStrLeng++;
    }
    else
    {
      datenum++;
      
      switch(datenum)
      {
        case 1:
        {
          pstGNSSIn->m_stNMEA.m_stGprmc.dUtcTime = strtod(TempStr, 0);
          
          break;
        }
        
        case 2:
        {
          if(TempStr[0] == 'A')
          {
            pstGNSSIn->m_stNMEA.m_stGprmc.u8Data_valid = 1;
          }
          else if(TempStr[0] == 'V')
          {
            pstGNSSIn->m_stNMEA.m_stGprmc.u8Data_valid = 0;
          }
          
          break;
        }
        
        case 3:
        {
          pstGNSSIn->m_stNMEA.m_stGprmc.dLatitude = Ddmm2dd(strtod(TempStr, 0));
          
          break;
        }
        
        case 4:
        {
          if(TempStr[0] == 'S')
          {
            pstGNSSIn->m_stNMEA.m_stGprmc.dLatitude = pstGNSSIn->m_stNMEA.m_stGprmc.dLatitude * (-1.0);
          }
          
          break;
        }
        
        case 5:
        {
          pstGNSSIn->m_stNMEA.m_stGprmc.dLongitude = Ddmm2dd(strtod(TempStr, 0));
          
          break;
        }
        
        case 6:
        {
          if(TempStr[0] == 'W')
          {
            pstGNSSIn->m_stNMEA.m_stGprmc.dLongitude = pstGNSSIn->m_stNMEA.m_stGprmc.dLongitude * (-1.0);
          }
          
          break;
        }
        
        case 7:
        {
          pstGNSSIn->m_stNMEA.m_stGprmc.fVgoroud = atof(TempStr) * MILE2KM;
          
          break;
        }
        
        case 8:
        {
          pstGNSSIn->m_stNMEA.m_stGprmc.fHeading = atof(TempStr);
          
          break;
        }
        
        case 9:
        {
          pstGNSSIn->m_stNMEA.m_stGprmc.u32Date = atof(TempStr);
          
          break;
        }
        
        case 10:
        {
          break;
        }
        
        case 11:
        {
          break;
        }
        
        case 12:
        {
          break;
        }
        
        case 13:
        {
          break;
        }
      }
      
      memset(TempStr, 0, sizeof(TempStr));
      TempStrLeng = 0;
    }
  }
  
  return _GPRMC;
}

static uint8_t Decode_Gphdt(stPackage *pstPackage,stGnssIn *pstGNSSIn)
{
  uint8_t  datenum     =  0;
  char     TempStr[50] = {0};
  uint16_t TempStrLeng =  0;
  
  if(!CheckSum_NMEA(pstPackage->u8Vec_Package,pstPackage->u16Num_Package))
  {
    return _NONE;
  }
  
  for(int i = 7; i < (pstPackage->u16Num_Package - 2); i++)
  {
    if(!((pstPackage->u8Vec_Package[i] == 44) || (pstPackage->u8Vec_Package[i] == 42)))
    {
      TempStr[TempStrLeng] = pstPackage->u8Vec_Package[i];
      TempStrLeng++;
    }
    else
    {
      datenum++;
      
      switch(datenum)
      {
        case 1:
        {
          pstGNSSIn->m_stNMEA.m_stGphdt.fAzi = atof(TempStr);
          
          break;
        }
        
        case 2:
        {
          pstGNSSIn->m_stNMEA.m_stGphdt.uTrue = TempStr[0];
          
          break;
        }
        
        case 3:
        {
          break;
        }
      }
      
      memset(TempStr, 0, sizeof(TempStr));
      TempStrLeng = 0;
    }
  }
  
  return _GPHDT;
}

static uint8_t Decode_BestVela(stPackage *pstPackage, stGnssIn *pstGnssIn)
{
  uint8_t  datenum     =  0;
  char     TempStr[50] = {0};
  uint16_t TempStrLeng =  0;
  
  if(!Checmsum_Novtel(pstPackage->u8Vec_Package,pstPackage->u16Num_Package))
  {
    return _NONE;
  }
  
  for(int i = 10; i < (pstPackage->u16Num_Package - 2); i++)
  {
    if(!((pstPackage->u8Vec_Package[i] == 0x2A) || (pstPackage->u8Vec_Package[i] == 0x2C) || (pstPackage->u8Vec_Package[i] == 0x3B)))
    {
      TempStr[TempStrLeng] = pstPackage->u8Vec_Package[i];
      TempStrLeng++;
    }
    else
    {
      datenum++;
      
      switch(datenum)
      {
        case 1:
        {
          memcpy(&(pstGnssIn->m_stBestVela.m_stLogHeader.u8Port), TempStr, TempStrLeng);
          
          break;
        }
        
        case 2:
        {
          memcpy(&(pstGnssIn->m_stBestVela.m_stLogHeader.u8Sequence), TempStr, TempStrLeng);
          
          break;
        }
        
        case 3:
        {
          memcpy(&(pstGnssIn->m_stBestVela.m_stLogHeader.u8IdleTime), TempStr, TempStrLeng);
          
          break;
        }
        
        case 4:
        {
          memcpy(&(pstGnssIn->m_stBestVela.m_stLogHeader.u8TimeStatus), TempStr, TempStrLeng);
          
          break;
        }
        
        case 5:
        {
          pstGnssIn->m_stBestVela.m_stLogHeader.u16Week = atof(TempStr);
          
          break;
        }
        
        case 6:
        {
          pstGnssIn->m_stBestVela.m_stLogHeader.dGpsSec = strtod(TempStr, 0);
          
          break;
        }
        
        case 7:
        {
          memcpy(&(pstGnssIn->m_stBestVela.m_stLogHeader.u8ReceiverStatus), TempStr, TempStrLeng);
          
          break;
        }
        
        case 8:
        {
          memcpy(&(pstGnssIn->m_stBestVela.m_stLogHeader.u8Reserved), TempStr, TempStrLeng);
          
          break;
        }
        
        case 9:
        {
          memcpy(&(pstGnssIn->m_stBestVela.m_stLogHeader.u8Version), TempStr, TempStrLeng);
          
          break;
        }
        
        case 10:
        {
          memcpy(&(pstGnssIn->m_stBestVela.u8SolStatus), TempStr, TempStrLeng);
          
          break;
        }
        
        case 11:
        {
          if(!memcmp("NONE", TempStr, 4))
          {
            pstGnssIn->m_stBestVela.u8P_VType = 0;
          }
          else if(!memcmp("PROPAGATED", TempStr, 10))
          {
            pstGnssIn->m_stBestVela.u8P_VType = 0;
          }
          else
          {
            pstGnssIn->m_stBestVela.u8P_VType   = 1;
            pstGnssIn->m_stBestVela.u8VDataFlag = 1;
          }
          
          memcpy(&(pstGnssIn->m_stBestVela.u8VelType), TempStr, TempStrLeng);
          
          break;
        }
        
        case 12:
        {
          memcpy(&(pstGnssIn->m_stBestVela.u8Latency), TempStr, TempStrLeng);
          
          break;
        }
        
        case 13:
        {
          memcpy(&(pstGnssIn->m_stBestVela.u8Age), TempStr, TempStrLeng);
          
          break;
        }
        
        case 14:
        {
          pstGnssIn->m_stBestVela.fVGround = (float)atof(TempStr);
          
          break;
        }
        
        case 15:
        {
          pstGnssIn->m_stBestVela.fTrack = (float)atof(TempStr);
          pstGnssIn->m_stBestVela.fVe    = (float)sin(pstGnssIn->m_stBestVela.fTrack * _D2R) * pstGnssIn->m_stBestVela.fVGround;
          pstGnssIn->m_stBestVela.fVn    = (float)cos(pstGnssIn->m_stBestVela.fTrack * _D2R) * pstGnssIn->m_stBestVela.fVGround;
          
          break;
        }
        
        case 16:
        {
          pstGnssIn->m_stBestVela.fVu = (float)atof(TempStr);
          
          break;
        }
        
        case 17:
        {
          memcpy(&(pstGnssIn->m_stBestVela.m_stLogHeader.u8Reserved), TempStr, TempStrLeng);
          
          break;
        }
      }
      
      memset(TempStr, 0, sizeof(TempStr));
      TempStrLeng = 0;
    }
  }
  
  if(pstGnssIn->m_stBestVela.u8P_VType == 1)
  {
    pstGnssIn->m_stBestVela.u8VDataFlag = 1;
  }
  
  return _BESTVELA;
}

static uint8_t Decode_Headinga(stPackage *pstPackage, stGnssIn *pstGNSSIn)
{
  uint8_t  datenum     =  0;
  char     TempStr[50] = {0};
  uint16_t TempStrLeng =  0;
  
  if(!Checmsum_Novtel(pstPackage->u8Vec_Package,pstPackage->u16Num_Package))
  {
    return _NONE;
  }
  
  for(int i = 10; i < (pstPackage->u16Num_Package - 2); i++)
  {
    if(!((pstPackage->u8Vec_Package[i] == 0x2A) || (pstPackage->u8Vec_Package[i] == 0x2C) || (pstPackage->u8Vec_Package[i] == 0x3B)))
    {
      TempStr[TempStrLeng] = pstPackage->u8Vec_Package[i];
      TempStrLeng++;
    }
    else
    {
      datenum++;
      
      switch(datenum)
      {
        case 1:
        {
          memcpy(&(pstGNSSIn->m_stHeadinga.m_stLogHeader.u8Port), TempStr, TempStrLeng);
          
          break;
        }
        
        case 2:
        {
          memcpy(&(pstGNSSIn->m_stHeadinga.m_stLogHeader.u8Sequence), TempStr, TempStrLeng);
          
          break;
        }
        
        case 3:
        {
          memcpy(&(pstGNSSIn->m_stHeadinga.m_stLogHeader.u8IdleTime), TempStr, TempStrLeng);
          
          break;
        }
        
        case 4:
        {
          memcpy(&(pstGNSSIn->m_stHeadinga.m_stLogHeader.u8TimeStatus), TempStr, TempStrLeng);
          
          break;
        }
        
        case 5:
        {
          pstGNSSIn->m_stHeadinga.m_stLogHeader.u16Week = atof(TempStr);
          
          break;
        }
        
        case 6:
        {
          pstGNSSIn->m_stHeadinga.m_stLogHeader.dGpsSec = atof(TempStr);
          
          break;
        }
        
        case 7:
        {
          memcpy(&(pstGNSSIn->m_stHeadinga.m_stLogHeader.u8ReceiverStatus), TempStr, TempStrLeng);
          
          break;
        }
        
        case 8:
        {
          memcpy(&(pstGNSSIn->m_stHeadinga.m_stLogHeader.u8Reserved), TempStr, TempStrLeng);
          
          break;
        }
        
        case 9:
        {
          memcpy(&(pstGNSSIn->m_stHeadinga.m_stLogHeader.u8Version), TempStr, TempStrLeng);
          
          break;
        }
        
        case 10:
        {
          pstGNSSIn->m_stHeadinga.u32SolStat = (uint32_t)Test_SolStat(TempStr, TempStrLeng);
          memcpy(&(pstGNSSIn->m_stHeadinga.strSolStat), TempStr, TempStrLeng);
          
          break;
        }
        
        case 11:
        {
          pstGNSSIn->m_stHeadinga.uP_VType = Test_P_V_Type(TempStr, TempStrLeng);
          memcpy(&(pstGNSSIn->m_stHeadinga.strPosType), TempStr, TempStrLeng);
          
          break;
        }
        
        case 12:
        {
          pstGNSSIn->m_stHeadinga.fBaseline = atof(TempStr);
          memcpy(&(pstGNSSIn->m_stHeadinga.strBaseLine), TempStr, TempStrLeng);
          
          break;
        }
        
        case 13:
        {
          pstGNSSIn->m_stHeadinga.fHeading = atof(TempStr);
          
          break;
        }
        
        case 14:
        {
          pstGNSSIn->m_stHeadinga.fPitch = atof(TempStr);
          
          break;
        }
        
        case 15:
        {
          break;
        }
        
        case 16:
        {
          pstGNSSIn->m_stHeadinga.fHdg_std = atof(TempStr);
          
          break;
        }
        
        case 17:
        {
          pstGNSSIn->m_stHeadinga.fPtch_std = atof(TempStr);
          
          break;
        }
        
        case 18:
        {
          memcpy(&(pstGNSSIn->m_stHeadinga.cStnID), TempStr, TempStrLeng);
          
          break;
        }
        
        case 19:
        {
          pstGNSSIn->m_stHeadinga.uSVs = atof(TempStr);
          
          break;
        }
        
        case 20:
        {
          pstGNSSIn->m_stHeadinga.uSolnSVs = atof(TempStr);
          
          break;
        }
        
        case 21:
        {
          pstGNSSIn->m_stHeadinga.uObs = atof(TempStr);
          
          break;
        }
        
        case 22:
        {
          pstGNSSIn->m_stHeadinga.uMulti = atof(TempStr);
          
          break;
        }
        
        case 23:
        {
          pstGNSSIn->m_stHeadinga.u8SolSource = atof(TempStr);
          
          break;
        }
        
        case 24:
        {
          pstGNSSIn->m_stHeadinga.ucExtSolStat = (uint8_t)atoi(TempStr);
          
          break;
        }
        
        case 25:
        {
          pstGNSSIn->m_stHeadinga.u8SigMask_1 = (uint8_t)atoi(TempStr);
          
          break;
        }
        
        case 26:
        {
          pstGNSSIn->m_stHeadinga.u8SigMask_2 = (uint8_t)atoi(TempStr);
          
          break;
        }
      }
      
      memset(TempStr, 0, sizeof(TempStr));
      TempStrLeng = 0;
    }
  }
  
  return _HEADINGA;
}

static uint8_t CheckSum_NMEA(const uint8_t *u8Buffer, uint16_t u16Count)
{
  uint8_t u8Sum = 0;
  
  for(int i = 1; i < (u16Count - 5); i++)
  {
    u8Sum ^= u8Buffer[i];
  }
  
  if(u8Sum == (ASCII2u8(u8Buffer[u16Count - 4]) * 16 + ASCII2u8(u8Buffer[u16Count - 3])))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

static uint8_t ASCII2u8(uint8_t u8Data)
{
  uint8_t u8Temp = 0;
  
  if((u8Data > 0x2f) && (u8Data < 0x3a))
  {
    u8Temp = u8Data - 0x30;
  }
  
  if((u8Data > 0x40) && (u8Data < 0x47))
  {
    u8Temp = u8Data - 55;
  }
  
  if((u8Data > 0x60) && (u8Data < 0x67))
  {
    u8Temp = u8Data - 87;
  }
  
  return u8Temp;
}

static double Ddmm2dd(double ddmm)
{
  int    tt     = 0;
  double d_in   = 0.0;
  double dd_out = 0.0;
  
  tt     = (int)(ddmm / 100.0);
  d_in   = ((ddmm / 100 - tt) * 100) / 60;
  dd_out = tt + d_in;
  
  return dd_out;
}

static uint32_t hhmmss2ss(double fddmm)
{
  uint32_t ddmm = 0;
  int      hh   = 0;
  int      mm   = 0;
  uint32_t ss   = 0;
  
  ddmm = (uint32_t)((fddmm + 0.05) * 10);
  ddmm = ddmm * 100;
  
  hh = (int)(ddmm / 10000000);
  mm = (ddmm - hh * 10000000) / 100000;
  ss = ddmm - hh * 10000000.0 - mm * 100000.0;
  ss = hh * 3600000 + mm * 60000 + ss;
  
  return ss;
}

static int Date2Week(int date, float time)
{
  int day   = date / 10000;
  int month = (date - day * 10000) / 100;
  int year  = date - day * 10000 - month * 100 + 2000;
  
  int hour   = (int)time / 10000;
  int minute = ((int)time - hour * 10000) / 100;
  int second = (int)time - hour * 10000 - minute * 100;
  
  int jdn = (1461 * (year + 4800 + (month - 14) / 12)) / 4
          + (367 * (month - 2 - 12 * ((month - 14) / 12))) / 12
          - (3 * ((year + 4900 + (month - 14) / 12) / 100)) / 4
          + day - 32075;
  
  double jd = jdn + ((hour - 12) * 3600.0 + minute * 60.0 + second) / 86400.0;
  
  return (int)(jd - 2444244.5) / 7;
}

static int ReturnWeekDay(int date)
{
  int y = 0, c = 0, m = 0, d = 0;
  
  int day   = date / 10000;
  int month = (date - day * 10000) / 100;
  int year  = date - day * 10000 - month * 100 + 2000;
  
  if(month < 3)
  {
    c = (year - 1) / 100;
    y = (year - 1) % 100;
    m = month + 12;
    d = day;
  }
  else
  {
    c = year / 100;
    y = year % 100;
    m = month;
    d = day;
  }
  
  return (y + y / 4 + c / 4 - 2 * c + 26 * (m + 1) / 10 + d - 1) % 7;
}

static uint8_t Checmsum_Novtel(uint8_t *ucBuffer, uint32_t ulCount)
{
  uint8_t Check_sum_char[8] = {0};
  memcpy(Check_sum_char, ucBuffer + ulCount - 10, 8);
  uint32_t sum_u32 = Asc_Int2Int(Check_sum_char);
  
  if(sum_u32 == CalculateBlockCRC32(ulCount - 12, ucBuffer + 1))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

static uint32_t Asc_Int2Int(uint8_t *Buffer)
{
  uint32_t asc_0 = 0;
  
  asc_0  += ((uint32_t)ASCII2u8(Buffer[0])) * 16 + ASCII2u8(Buffer[1]);
  asc_0 <<= 8;
  asc_0  += ((uint32_t)ASCII2u8(Buffer[2])) * 16 + ASCII2u8(Buffer[3]);
  asc_0 <<= 8;
  asc_0  += ((uint32_t)ASCII2u8(Buffer[4])) * 16 + ASCII2u8(Buffer[5]);
  asc_0 <<= 8;
  asc_0  += ((uint32_t)ASCII2u8(Buffer[6])) * 16 + ASCII2u8(Buffer[7]);
  
  return asc_0;
}

static uint32_t CalculateBlockCRC32(uint32_t ulCount, uint8_t *ucBuffer)
{
  uint32_t ulTemp1 = 0;
  uint32_t ulTemp2 = 0;
  uint32_t ulCRC   = 0;
  
  while(ulCount-- != 0)
  {
    ulTemp1 = (ulCRC >> 8) & 0x00FFFFFFL;
    ulTemp2 = CRC32Value(((int)ulCRC ^ *ucBuffer++) & 0xFF);
    ulCRC   = ulTemp1 ^ ulTemp2;
  }
  
  return(ulCRC);
}

static uint32_t CRC32Value(int32_t i)
{
  uint32_t ulCRC = i;
  
  for(int j = 8; j > 0; j--)
  {
    if(ulCRC & 1)
    {
      ulCRC = (ulCRC >> 1) ^ CRC32_POLYNOMIAL;
    }
    else
    {
      ulCRC >>= 1;
    }
  }
  
  return ulCRC;
}

static uint8_t Test_P_V_Type(const char *DataBuffer, int Length)
{
  if(!memcmp("NONE", DataBuffer, 4))
  {
    return 0; /* No solution */
  }
  else if(!memcmp("FIXEDPOS", DataBuffer, 8))
  {
    return 1; /* Position has been fixed by the FIX POSITION command */
  }
  else if(!memcmp("FIXEDHEIGHT", DataBuffer, 11))
  {
    return 2; /* Position has been fixed by the FIX HEIGHT/AUTO command */
  }
  else if(!memcmp("DOPPLER_VELOCITY", DataBuffer, 16))
  {
    return 8; /* Velocity computed using instantaneous Doppler */
  }
  else if(!memcmp("SINGLE", DataBuffer, 6))
  {
    return 16; /* Single point position */
  }
  else if(!memcmp("PSRDIFF", DataBuffer, 7))
  {
    return 17; /* Pseudorange differential solution */
  }
  else if(!memcmp("WAAS", DataBuffer, 4))
  {
    return 18; /* Solution calculated using corrections from an SBAS */
  }
  else if(!memcmp("PROPAGATED", DataBuffer, 10))
  {
    return 19; /* Propagated by a Kalman filter without new observations */
  }
  else if(!memcmp("OMNISTAR", DataBuffer, 8))
  {
    return 20; /* OmniSTAR VBS position (L1 sub-meter) */
  }
  else if(!memcmp("L1_FLOAT", DataBuffer, 8))
  {
    return 32; /* Floating L1 ambiguity solution */
  }
  else if(!memcmp("IONOFREE_FLOAT", DataBuffer, 14))
  {
    return 33; /* Floating ionospheric-free ambiguity solution */
  }
  else if(!memcmp("NARROW_FLOAT", DataBuffer, 12))
  {
    return 34; /* Floating narrow-lane ambiguity solution */
  }
  else if(!memcmp("L1_INT", DataBuffer, 6))
  {
    return 48; /* Integer L1 ambiguity solution */
  }
  else if(!memcmp("WIDE_INT", DataBuffer, 7))
  {
    return 49; /* Integer wide-lane ambiguity solution */
  }
  else if(!memcmp("NARROW_INT", DataBuffer, 10))
  {
    return 50; /* Integer narrow-lane ambiguity solution */
  }
  else if(!memcmp("CDGPS", DataBuffer, 5))
  {
    return 66; /* Position solution using CDGPS correction */
  }
  else
  {
    return 0;
  }
}

static uint8_t Test_SolStat(const char* DataBuffer, int Length)
{
  if(!memcmp("SOL_COMPUTED", DataBuffer, 12))
  {
    return 0;
  }
  else if(!memcmp("INSUFFICIENT_OBS", DataBuffer, 16))
  {
    return 1;
  }
  else if(!memcmp("NO_CONVERGENCE", DataBuffer, 14))
  {
    return 2;
  }
  else if(!memcmp("SINGULARITY", DataBuffer, 11))
  {
    return 3;
  }
  else if(!memcmp("COV_TRACE", DataBuffer, 9))
  {
    return 4;
  }
  else if(!memcmp("TEST_DIST", DataBuffer, 9))
  {
    return 5;
  }
  else if(!memcmp("COLD_START", DataBuffer, 10))
  {
    return 6;
  }
  else if(!memcmp("V_H_LIMIT", DataBuffer, 9))
  {
    return 7;
  }
  else if(!memcmp("VARIANCE", DataBuffer, 8))
  {
    return 8;
  }
  else if(!memcmp("RESIDUALS", DataBuffer, 9))
  {
    return 9;
  }
  else if(!memcmp("DELTA_POS", DataBuffer, 9))
  {
    return 10;
  }
  else if(!memcmp("NEGATIVE_VAR", DataBuffer, 12))
  {
    return 11;
  }
  else if(!memcmp("INTEGRITY_WARNING", DataBuffer, 17))
  {
    return 13;
  }
  else if(!memcmp("PENDING", DataBuffer, 7))
  {
    return 18;
  }
  else if(!memcmp("INVALID_FIX", DataBuffer, 11))
  {
    return 19;
  }
  else if(!memcmp("UNAUTHORIZED", DataBuffer, 12))
  {
    return 20;
  }
  else if(!memcmp("ANTENNA_WARNING", DataBuffer, 15))
  {
    return 21;
  }
  else
  {
    return 0;
  }
}

void Combain_GNSS(stGnssIn *pstGNSSIn, stGnssOut *pstGNSSOut)
{
#if (USER_NMEA == USER_NMEA_0)
  if((pstGNSSIn->u16Flag & GPS_GPGGA_IN) && (pstGNSSIn->u16Flag & GPS_GPRMC_IN) && (pstGNSSIn->m_stNMEA.m_stGpgga.u8GPS_State > 0))
  {
    uint8_t u8Day = ReturnWeekDay(pstGNSSIn->m_stNMEA.m_stGprmc.u32Date);
    
    pstGNSSOut->u16Week    = Date2Week(pstGNSSIn->m_stNMEA.m_stGprmc.u32Date, pstGNSSIn->m_stNMEA.m_stGprmc.dUtcTime);
    pstGNSSOut->u32UTC     = hhmmss2ss(pstGNSSIn->m_stNMEA.m_stGpgga.dUtcTime) + u8Day * 24 * 3600 * 1000 + _UTC2GPS;
    pstGNSSOut->sv_num     = pstGNSSIn->m_stNMEA.m_stGpgga.Num_Satellites;
    pstGNSSOut->dLatitude  = pstGNSSIn->m_stNMEA.m_stGpgga.dLatitude;
    pstGNSSOut->dLongitude = pstGNSSIn->m_stNMEA.m_stGpgga.dLongitude;
    pstGNSSOut->fAltitude  = pstGNSSIn->m_stNMEA.m_stGpgga.fAltitude;
    pstGNSSOut->fHeading   = pstGNSSIn->m_stNMEA.m_stGprmc.fHeading;
    pstGNSSOut->fVgroud    = pstGNSSIn->m_stNMEA.m_stGprmc.fVgoroud;
    pstGNSSOut->rtcm_age   = pstGNSSIn->m_stNMEA.m_stGpgga.u32DiffTime;
    pstGNSSOut->hdop       = (uint16_t)(pstGNSSIn->m_stNMEA.m_stGpgga.fHdop * 100.0f);
    
    if((pstGNSSIn->m_stNMEA.m_stGpgga.u8GPS_State > 0) && (pstGNSSIn->m_stNMEA.m_stGprmc.u8Data_valid == 1)) /* 位置和速度有效 */
    {
      pstGNSSOut->u16Flag |= GNSS_POSITIONING;
    }
    
    if(pstGNSSIn->m_stNMEA.m_stGpgga.u8GPS_State == 1)
    {
      pstGNSSOut->u16Flag |= GNSS_SINGLE_POINT;
    }
    
    if(pstGNSSIn->m_stNMEA.m_stGpgga.u8GPS_State == 2)
    {
      pstGNSSOut->u16Flag |= GNSS_DGPS;
    }
    
    if(pstGNSSIn->m_stNMEA.m_stGpgga.u8GPS_State == 5)
    {
      pstGNSSOut->u16Flag |= GNSS_FLOAT;
    }
    
    if(pstGNSSIn->m_stNMEA.m_stGpgga.u8GPS_State == 4)
    {
      pstGNSSOut->u16Flag |= GNSS_FIXED;
    }
    
    pstGNSSOut->u16Flag |= GNSS_DATA;
  }
#elif (USER_NMEA == USER_NMEA_1)
  if((pstGNSSIn->u16Flag & GPS_GPGGA_IN) && (pstGNSSIn->u16Flag & GPS_GPRMC_IN) && (pstGNSSIn->u16Flag & GPS_HEADINGA_IN) && (pstGNSSIn->m_stNMEA.m_stGpgga.u8GPS_State > 0))
  {
    uint8_t u8Day = ReturnWeekDay(pstGNSSIn->m_stNMEA.m_stGprmc.u32Date);
    
    pstGNSSOut->u16Week    = Date2Week(pstGNSSIn->m_stNMEA.m_stGprmc.u32Date, pstGNSSIn->m_stNMEA.m_stGprmc.dUtcTime);
    pstGNSSOut->u32UTC     = hhmmss2ss(pstGNSSIn->m_stNMEA.m_stGpgga.dUtcTime) + u8Day * 24 * 3600 * 1000 + _UTC2GPS;
    pstGNSSOut->sv_num     = pstGNSSIn->m_stNMEA.m_stGpgga.Num_Satellites;
    pstGNSSOut->dLatitude  = pstGNSSIn->m_stNMEA.m_stGpgga.dLatitude;
    pstGNSSOut->dLongitude = pstGNSSIn->m_stNMEA.m_stGpgga.dLongitude;
    pstGNSSOut->fAltitude  = pstGNSSIn->m_stNMEA.m_stGpgga.fAltitude;
    pstGNSSOut->fHeading   = pstGNSSIn->m_stNMEA.m_stGprmc.fHeading;
    pstGNSSOut->fAzi       = pstGNSSIn->m_stHeadinga.fHeading;
    pstGNSSOut->fVgroud    = pstGNSSIn->m_stNMEA.m_stGprmc.fVgoroud;
    pstGNSSOut->rtcm_age   = pstGNSSIn->m_stNMEA.m_stGpgga.u32DiffTime;
    pstGNSSOut->hdop       = (uint16_t)(pstGNSSIn->m_stNMEA.m_stGpgga.fHdop * 100.0f);
    
    if((pstGNSSIn->m_stNMEA.m_stGpgga.u8GPS_State > 0) && (pstGNSSIn->m_stNMEA.m_stGprmc.u8Data_valid == 1)) /* 位置和速度有效 */
    {
      pstGNSSOut->u16Flag |= GNSS_POSITIONING;
    }
    
    if((pstGNSSIn->m_stHeadinga.uP_VType == 48) || (pstGNSSIn->m_stHeadinga.uP_VType == 50))
    {
      pstGNSSOut->u16Flag |= GNSS_DUAL_ANTENNA;
    }
    
    if(pstGNSSIn->m_stNMEA.m_stGpgga.u8GPS_State == 1)
    {
      pstGNSSOut->u16Flag |= GNSS_SINGLE_POINT;
    }
    
    if(pstGNSSIn->m_stNMEA.m_stGpgga.u8GPS_State == 2)
    {
      pstGNSSOut->u16Flag |= GNSS_DGPS;
    }
    
    if(pstGNSSIn->m_stNMEA.m_stGpgga.u8GPS_State == 5)
    {
      pstGNSSOut->u16Flag |= GNSS_FLOAT;
    }
    
    if(pstGNSSIn->m_stNMEA.m_stGpgga.u8GPS_State == 4)
    {
      pstGNSSOut->u16Flag |= GNSS_FIXED;
    }
    
    pstGNSSOut->u16Flag |= GNSS_DATA;
  }
#elif (USER_NMEA == USER_NMEA_2)
  if((pstGNSSIn->u16Flag & GPS_GPGGA_IN) && (pstGNSSIn->u16Flag & GPS_BESTVELA_IN) && (pstGNSSIn->u16Flag & GPS_HEADINGA_IN))
  {
    uint32_t u32Time = (uint32_t)((pstGNSSIn->m_stBestVela.m_stLogHeader.dGpsSec + 0.05) * 10);
    
    pstGNSSOut->u16Week    = pstGNSSIn->m_stBestVela.m_stLogHeader.u16Week;
    pstGNSSOut->u32UTC     = u32Time * 100;
    pstGNSSOut->sv_num     = pstGNSSIn->m_stNMEA.m_stGpgga.Num_Satellites;
    pstGNSSOut->dLatitude  = pstGNSSIn->m_stNMEA.m_stGpgga.dLatitude;
    pstGNSSOut->dLongitude = pstGNSSIn->m_stNMEA.m_stGpgga.dLongitude;
    pstGNSSOut->fAltitude  = pstGNSSIn->m_stNMEA.m_stGpgga.fAltitude;
    pstGNSSOut->fVgroud    = pstGNSSIn->m_stBestVela.fVGround;
    pstGNSSOut->fHeading   = pstGNSSIn->m_stBestVela.fTrack;
    pstGNSSOut->fAzi       = pstGNSSIn->m_stHeadinga.fHeading;
    pstGNSSOut->rtcm_age   = pstGNSSIn->m_stNMEA.m_stGpgga.u32DiffTime;
    pstGNSSOut->hdop       = (uint16_t)(pstGNSSIn->m_stNMEA.m_stGpgga.fHdop * 100.0f);
    
    if((pstGNSSIn->m_stNMEA.m_stGpgga.u8GPS_State > 0) && (pstGNSSIn->m_stBestVela.u8VDataFlag == 1))
    {
      pstGNSSOut->u16Flag |= GNSS_POSITIONING;
    }
    
    if((pstGNSSIn->m_stHeadinga.uP_VType == 48) || (pstGNSSIn->m_stHeadinga.uP_VType == 50))
    {
      pstGNSSOut->u16Flag |= GNSS_DUAL_ANTENNA;
    }
    
    /* single point */
    if(pstGNSSIn->m_stNMEA.m_stGpgga.u8GPS_State == 1)
    {
      pstGNSSOut->u16Flag |= GNSS_SINGLE_POINT;
    }
    
    /*  DGPS  */
    if(pstGNSSIn->m_stNMEA.m_stGpgga.u8GPS_State == 2)
    {
      pstGNSSOut->u16Flag |= GNSS_DGPS;
    }
    
    /*  float */
    if(pstGNSSIn->m_stNMEA.m_stGpgga.u8GPS_State == 5)
    {
      pstGNSSOut->u16Flag |= GNSS_FLOAT;
    }
    
    /* fixed */
    if(pstGNSSIn->m_stNMEA.m_stGpgga.u8GPS_State == 4)
    {
      pstGNSSOut->u16Flag |= GNSS_FIXED;
    }
    
    pstGNSSOut->u16Flag |= GNSS_DATA;
  }
#endif
}
