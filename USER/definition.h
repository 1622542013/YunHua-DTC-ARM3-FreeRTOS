/******************************************************************************/
/*                                                                            */
/*                                definition.h                                */
/*                                                                            */
/******************************************************************************/
#ifndef  __UMI_DEFINITION_H
#define  __UMI_DEFINITION_H

/*============================================================================*/
/*                               Header include                               */
/*============================================================================*/


/*============================================================================*/
/*                              Macro definition                              */
/*============================================================================*/
#ifndef TRUE
#define TRUE                    (1)
#endif /* TRUE */
#ifndef FALSE
#define FALSE                   (0)
#endif /* FALSE */

#define INLIB_OK                (0)
#define INLIB_ERROR             (-1)
#define PI                      (3.1415926535897932384626433832795)
#define R2D                     (180.0/PI)
#define D2R                     (PI/180.0)

#ifdef WIN32
  #define sinf(a) (float)sin((double)a)
  #define asinf(a) (float)asin((double)a)
  #define cosf(a) (float)cos((double)a)
  #define tanf(a) (float)tan((double)a)
  #define atan2f(a,b) (float)atan2((double)a,(double)b)
  #define sqrtf(a) (float)sqrt((double)a)
  #define fabsf(a) (float)fabs((double)a)
  #define floorf(a) (float)floor((double)a)
  #define powf(a,b) (float)pow((double)a,b)
#endif

typedef void               TpVoid;
typedef signed char        TpBool;
typedef char               TpChar;
typedef unsigned char      TpUchar;
typedef short              TpInt16;
typedef unsigned short     TpUint16;
typedef int                TpInt32;
typedef unsigned int       TpUint32;
typedef float              TpFloat;
typedef double             TpDouble;
typedef long long          TpInt64;
typedef unsigned long long TpUint64;


#endif
