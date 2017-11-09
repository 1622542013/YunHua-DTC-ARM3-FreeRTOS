// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SFLIB_DLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SFLIB_DLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#pragma once

#if (defined MCLIB_DLL_EXPORTS)&&(defined WIN32)
#define MCLIB_DLL_API __declspec(dllexport)
#elif (defined WIN32)
#define MCLIB_DLL_API __declspec(dllimport)
#else
#define MCLIB_DLL_API
#endif

#include <stdint.h>


#ifndef INS_CONST
#define INS_CONST


/******************** magcali data len *******************/
#define   MAG_CALI_DATA_LEN         ((unsigned short int)100)

#define _ESP_F32    1.19e-7f
#define _ESP_F64    2.22e-16

#define _PI 		3.1415926535897932	//PI
#define _D2R 	    0.017453292519943	//Degree to Rad
#define _R2D		57.29577951308232
#define _DPH 	    (4.848136811e-6)	//Degree per Hour

#define _G0 		9.8019967			//??9.8019967 //??9.801616
#define _UG 		(_G0*1e-6)			//ug
#define _MG		    (_G0*1e-3)			//mg
#define _PPM		1e-6

#define _PI_F 		3.1415926535897932f	//PI
#define _D2R_F 	    0.017453292519943f	//Degree to Rad
#define _R2D_F		57.29577951308232f
#define _DPH_F 	    (4.848136811e-6f)	//Degree per Hour

#define _G0_F 		9.8019967f			//??9.8019967 //??9.801616
#define _UG_F 		(_G0_F*1e-6f)			//ug
#define _MG_F		(_G0_F*1e-3f)			//mg
#define _PPM_F		1e-6f

#endif



/**************************************************************************/




/************************************************************************/
#ifdef WIN32
#define __packed
#pragma pack(push ,1)
#endif
#define __packed

/************************* Instance ********************************/

/*** Mag CAli Type ***/
typedef enum 
{
	MC_ELLIPSOID = 0,  /*0: Ellipsoid Fitting*/
  MC_CALIBOTH        /*1: Ellipsoid Fitting and  Axial Align*/
}MagCaliType;

/**** MCLib instance  ***/
typedef __packed struct
{
	unsigned char				index;
	unsigned short int  magcali_num;
  MagCaliType         magcali_type;
}MCLibInstance;

/************************** Structer ****************************************/

/*** mag and acc measurement ***/
typedef __packed struct
{
	float mag[3];
	float acc[3];
}Measure_in;

/*** EllipFitting result ***/
typedef __packed struct
{
	unsigned char is_ellipfitting_ok;
	float center0[3];
	float D0m_inv[9];
}EllipFittingResult;

typedef __packed struct
{
	float mag[3];
	int   count;
	int   state;
	double temp;
}outtempdata;


#ifdef WIN32
#pragma pack(pop)
#endif
/************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif
	
	MCLIB_DLL_API unsigned char CreatMCLib(MCLibInstance *ptr_mclib_instance);
  MCLIB_DLL_API unsigned char MagCalibtation(MCLibInstance *ptr_mclib_instance,Measure_in *mea_in);
	
	MCLIB_DLL_API unsigned char GetEllipFittingState(void);
	MCLIB_DLL_API void GetEllipFittingResult(EllipFittingResult *out);
	
	MCLIB_DLL_API void GetTempData(outtempdata* od);

#ifdef __cplusplus
}
#endif

