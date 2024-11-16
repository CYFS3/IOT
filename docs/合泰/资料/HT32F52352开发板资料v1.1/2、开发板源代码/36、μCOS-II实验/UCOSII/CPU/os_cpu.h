/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name		: os_cpu_c.c 
* Author		: Librae
* Date			: 06/10/2010
* Description	: ��COS-II��STM32�ϵ���ֲ����C���Բ��֣�
*				  ���������ջ��ʼ������͹��Ӻ�����

******************************************************************************/

#ifndef  OS_CPU_H
#define  OS_CPU_H
#ifdef   OS_CPU_GLOBALS
#define  OS_CPU_EXT
#else
#define  OS_CPU_EXT  extern
#endif
#ifndef  OS_CPU_EXCEPT_STK_SIZE
#define  OS_CPU_EXCEPT_STK_SIZE    1           /* Default exception stack size is 128 OS_STK entries */
#endif
typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned short INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   short INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned int   INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   int   INT32S;                   /* Signed   32 bit quantity                           */
typedef float          FP32;                     /* Single precision floating point                    */
typedef double      FP64;                     /* Double precision floating point                    */
typedef unsigned int   OS_STK;                   /* Each stack entry is 32-bit wide                    */
typedef unsigned int   OS_CPU_SR;            /* Define size of CPU status register (PSR = 32 bits) */
#define  OS_CRITICAL_METHOD   3
#if OS_CRITICAL_METHOD == 3
#define  OS_ENTER_CRITICAL()  {cpu_sr = OS_CPU_SR_Save();}
#define  OS_EXIT_CRITICAL()     {OS_CPU_SR_Restore(cpu_sr);}
#endif
#define  OS_STK_GROWTH        1                   /* Stack grows from HIGH to LOW memory on ARM        */
#define  OS_TASK_SW()         OSCtxSw()
#if OS_CRITICAL_METHOD == 3                       
OS_CPU_SR  OS_CPU_SR_Save        (void);
void       OS_CPU_SR_Restore     (OS_CPU_SR cpu_sr);
#endif
void       OSCtxSw               (void);           
void       OSStartHighRdy        (void);

#define OSIntCtxSw()         OSCtxSw();
#endif

/************************ (C) COPYLEFT 2010 Leafgrass ************************/
