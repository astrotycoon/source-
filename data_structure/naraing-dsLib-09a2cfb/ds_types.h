/* ds_types.h - sine memory management headers */

#ifndef __DS_TYPES_H
#define __DS_TYPES_H

#define TRUE 1
#define FALSE 0

typedef unsigned short MODULE_ID_T;
typedef void(*FUNCPTR_T)(void);
typedef float(*FUNCPTR1_T)(float, float);
typedef int (*FUNCPTR2_T)(int arg1, int arg2, int arg3);

typedef enum {DS_NO=1, DS_YES=2} TRUTH_E;
typedef enum {DS_FALSE=0, DS_TRUE=1} BOOL_E;
typedef enum {DS_LESS=1, DS_EQUAL, DS_MORE} DS_COMP_E;

typedef union {
   long idata;
   unsigned long uidata;
   char cdata;
   unsigned char ucdata;
   void* vpdata;
   FUNCPTR2_T fpdata;
   struct {unsigned int key; long idata;} assoc_idata;
   struct {unsigned int key; long uidata;} assoc_uidata;
   struct {unsigned int key; char cdata;} assoc_cdata;
   struct {unsigned int key; unsigned char ucdata;} assoc_ucdata;
   struct {unsigned int key; void* vpdata;} assoc_vpdata;
   struct {unsigned int key; FUNCPTR2_T fpdata;} assoc_fpdata;
} NODE_DATA_T;

typedef enum {
    IDATA=0,
    UIDATA,
    CDATA,
    UCDATA,
    VPDATA,
    FPDATA,
    LDATA,
    LLDATA,
    ULDATA,
    ULLDATA
} NODE_DATA_TYPE_E;

#define NANOSECONDS     1000000000
#define MICROSECONDS    1000000
#define MILLISECODS     1000
#define SECONDS         1

typedef enum {INT, CHAR, ULONG, LONG, ULONGLONG, LONGLONG, VOIDP } ELE_T;
typedef enum {
   DS_ERR_ERROR_HIGH = -512, /* fencepost */
   DS_ERR_MALLOC_FAIL,
   DS_ERR_ERR = -1, /* generic operational error */
   DS_ERR_OK = 0 /* routine returned without any errors */
} DS_ERR_E;

#if __STDC_VERSION__ < 199901L
#define false 0
#define true 1
#endif /* __STDC_VERSION__ */

#endif
