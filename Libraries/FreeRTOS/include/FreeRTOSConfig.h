/**
 *  @file     FreeRTOSConfig.h                                                           
 *  @brief    FreeRTOS 配置文件                                                                                                                     
 *  @note     编译环境:keil v5.23   
 *  @note     开发平台 stm32f401CCUx 
 *  @author   gao                                                              
 *  @version  1.0                                                  
 *  @date     2017.8.28                                            		    
 *  @warning  测试FreeRTOS版本					
 */ 
 
#include "globals.h"

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H
#include <stdint.h>
/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

/* Ensure stdint is only used by the compiler, and not the assembler. */
#ifdef __ICCARM__
	#include <stdint.h>
	extern uint32_t SystemCoreClock;
#endif

#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()    (ulHighFrequencyTimerTicks = 0ul)
#define portGET_RUN_TIME_COUNTER_VALUE()            ulHighFrequencyTimerTicks
#define configUSE_STATS_FORMATTING_FUNCTIONS        1

#define configUSE_PREEMPTION			1   /**< 使能抢占调度器 */
#define configUSE_IDLE_HOOK				0   /**< 使能空闲钩子函数 */
#define configUSE_TICK_HOOK				0   /**< 使能滴答时钟中断里的钩子函数 */
#define configCPU_CLOCK_HZ				( (uint32_t)168000000 )        /**< 定义系统主频84M */
#define configTICK_RATE_HZ				( ( TickType_t ) 1000)       /**< 定义系统时钟节拍  1MHZ */
#define configMAX_PRIORITIES			( 5 )                         /**< 定义系统最大优先级数，定义为5 ，则可使用优先级0，1，2，3，4，不包含5 */
#define configMINIMAL_STACK_SIZE	( ( uint32_t ) 130 )    /**< 定义系统空闲任务堆大小 */
#define configTOTAL_HEAP_SIZE			( ( size_t ) ( 40 * 1024 ) )  /**< 定义系统任务堆大小 */
#define configMAX_TASK_NAME_LEN		( 16 )                        /**< 定义系统任务名最大字节数,结束符也包含在内*/
#define configUSE_TRACE_FACILITY		1 /**< 要配置显示栈的使用情况 。置1，使能 vTaskList() 和 vTaskGetRunTimeStats() */ 
#define configUSE_16_BIT_TICKS			0 /**< 定义TickType_t字节数，0即24位(定义32位变量，只用低24位)（32位单片机使用），1即16位(定义16位变量，只用低8位)*/
#define configIDLE_SHOULD_YIELD			1 
#define configUSE_MUTEXES				    1 /**< 使能互斥信号量 */
#define configQUEUE_REGISTRY_SIZE		8 /**< 定义可以注册的信号量和消息队列设置 */
#define configCHECK_FOR_STACK_OVERFLOW	0 /**< 栈溢出，检测方法。2：方法2 ; 1：方法1 ; 0:禁止栈溢出检测 */
#define configUSE_RECURSIVE_MUTEXES		  1
#define configUSE_MALLOC_FAILED_HOOK	  0 /**< 使能malloc失败的钩子函数 */
#define configUSE_APPLICATION_TASK_TAG	  0
#define configUSE_COUNTING_SEMAPHORES	  1 /**< 使能计数信号量 */
#define configGENERATE_RUN_TIME_STATS	  1 /**< 使能任务运行状态参数统计 */

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 		0  /**< 使能合作式调度函数 */
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

/* Software timer definitions. */
#define configUSE_TIMERS				1   /**< 使能软件定时器 */
#define configTIMER_TASK_PRIORITY		( 2 ) /**< 软件定时器任务优先级 */
#define configTIMER_QUEUE_LENGTH		10 /**< 配置软件定时器命令队列长度 */
#define configTIMER_TASK_STACK_DEPTH	( configMINIMAL_STACK_SIZE * 2 )/**< 配置软件定时器任务 栈空间大小 */

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet		  1
#define INCLUDE_uxTaskPriorityGet		  1
#define INCLUDE_vTaskDelete				  1
#define INCLUDE_vTaskCleanUpResources	  0
#define INCLUDE_vTaskSuspend			  1
#define INCLUDE_vTaskDelayUntil			  1
#define INCLUDE_vTaskDelay				  1
#define INCLUDE_xTimerPendFunctionCall  1
/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
	/* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		4        /* 15 priority levels */
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			0x0f

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	0x01

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
	
/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }	
	
/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
#define vPortSVCHandler     SVC_Handler
#define xPortPendSVHandler  PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#endif /* FREERTOS_CONFIG_H */

