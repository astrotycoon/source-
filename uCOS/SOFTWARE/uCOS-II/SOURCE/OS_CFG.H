/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                           (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
*                                       uC/OS-II Configuration File
*
* File : OS_CFG.H
* By   : Jean J. Labrosse
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         uC/OS-II CONFIGURATION
*********************************************************************************************************
*/

                                       /* ---------------------- MISCELLANEOUS ----------------------- */
#define OS_ARG_CHK_EN             1    /* Enable (1) or Disable (0) argument checking                  *//* 参数检查 					*/

#define OS_CPU_HOOKS_EN           1    /* uC/OS-II hooks are found in the processor port files         *//* 设定是否在OS_CPU_C.C中声明对外接口函数 	*/

#define OS_LOWEST_PRIO           63    /* Defines the lowest priority that can be assigned ...         *//* 最低任务优先级				*/
                                       /* ... MUST NEVER be higher than 63!                            */

#define OS_MAX_EVENTS            10    /* Max. number of event control blocks in your application ...  *//* 可分配的事件控制块的最大数目 		*/
                                       /* ... MUST be > 0                                              */
#define OS_MAX_FLAGS              5    /* Max. number of Event Flag Groups    in your application ...  *//* 事件标志的最大数目 				*/
                                       /* ... MUST be > 0                                              */
#define OS_MAX_MEM_PART          10    /* Max. number of memory partitions ...                         *//* 最多内存分区数目				*/
                                       /* ... MUST be > 0                                              */
#define OS_MAX_QS                10    /* Max. number of queue control blocks in your application ...  *//* 消息队列的最大数目				*/
                                       /* ... MUST be > 0                                              */
#define OS_MAX_TASKS             63    /* Max. number of tasks in your application ...                 *//* 最多任务数					*/
                                       /* ... MUST be >= 2                                             */

#define OS_SCHED_LOCK_EN          1    /*     Include code for OSSchedLock() and OSSchedUnlock()       *//* 用于设定是否使用左边两个函数		*/

#define OS_TASK_IDLE_STK_SIZE   512    /* Idle task stack size (# of OS_STK wide entries)              *//* 设定空闲任务的堆栈容量 			*/

#define OS_TASK_STAT_EN           1    /* Enable (1) or Disable(0) the statistics task                 *//* 设定是否使用统计任务			*/
#define OS_TASK_STAT_STK_SIZE   512    /* Statistics task stack size (# of OS_STK wide entries)        *//* 设定统计任务的堆栈容量			*/

#define OS_TICKS_PER_SEC        200    /* Set the number of ticks in one second                        *//* 设定调用OSTimeTick()函数的频率		*/


                                       /* ----------------------- EVENT FLAGS ------------------------ */
#define OS_FLAG_EN                1    /* Enable (1) or Disable (0) code generation for EVENT FLAGS    *//* 设定是否使用所有事件标志函数以及相关数据结构*/
#define OS_FLAG_WAIT_CLR_EN       1    /* Include code for Wait on Clear EVENT FLAGS                   *//* 控制是否允许生成用于等待事件标志清0的代码	*/
#define OS_FLAG_ACCEPT_EN         1    /*     Include code for OSFlagAccept()                          *//* 是否使用OSFlagAccept()函数			*/
#define OS_FLAG_DEL_EN            1    /*     Include code for OSFlagDel()                             *//* 是否使用OSFlagDel()函数			*/
#define OS_FLAG_QUERY_EN          1    /*     Include code for OSFlagQuery()                           *//* 是否使用OSFlagQuery()函数			*/

				       /*                         消息邮箱			       */
                                       /* -------------------- MESSAGE MAILBOXES --------------------- */
#define OS_MBOX_EN                1    /* Enable (1) or Disable (0) code generation for MAILBOXES      *//* 设定是否使用消息邮箱及其相关的数据机构	*/
#define OS_MBOX_ACCEPT_EN         1    /*     Include code for OSMboxAccept()                          *//* 是否使用OSMboxAccept()函数			*/
#define OS_MBOX_DEL_EN            1    /*     Include code for OSMboxDel()                             *//* 是否使用OSMboxDel()函数			*/
#define OS_MBOX_POST_EN           1    /*     Include code for OSMboxPost()                            *//* 是否使用OSMboxPost()函数			*/
#define OS_MBOX_POST_OPT_EN       1    /*     Include code for OSMboxPostOpt()                         *//* 是否使用OSMboxPostOpt()函数			*/
#define OS_MBOX_QUERY_EN          1    /*     Include code for OSMboxQuery()                           *//* 是否使用OSMboxQuery()函数			*/


                                       /* --------------------- MEMORY MANAGEMENT -------------------- */
#define OS_MEM_EN                 1    /* Enable (1) or Disable (0) code generation for MEMORY MANAGER *//* 设定是否使用内存管理函数及相关的数据结构	*/
#define OS_MEM_QUERY_EN           1    /*     Include code for OSMemQuery()                            *//* 是否使用OSMemQuery()函数 			*/
				
				       /*                       互斥型信号量			       */
                                       /* ---------------- MUTUAL EXCLUSION SEMAPHORES --------------- */
#define OS_MUTEX_EN               1    /* Enable (1) or Disable (0) code generation for MUTEX          *//* 设定是否使用互斥型信号量函数及其相关数据结构*/
#define OS_MUTEX_ACCEPT_EN        1    /*     Include code for OSMutexAccept()                         *//* 是否使用OSMutexAccept()函数			*/
#define OS_MUTEX_DEL_EN           1    /*     Include code for OSMutexDel()                            *//* 是否使用OSMutexDel()函数			*/
#define OS_MUTEX_QUERY_EN         1    /*     Include code for OSMutexQuery()                          *//* 是否使用OSMutexQuery()函数			*/

				       /*                        消息队列			       */
                                       /* ---------------------- MESSAGE QUEUES ---------------------- */
#define OS_Q_EN                   1    /* Enable (1) or Disable (0) code generation for QUEUES         *//* 设定是否使用消息队列函数及其相关的数据结构	*/
#define OS_Q_ACCEPT_EN            1    /*     Include code for OSQAccept()                             *//* 是否使用OSQAccept()函数			*/
#define OS_Q_DEL_EN               1    /*     Include code for OSQDel()                                *//* 是否使用OSQDel()函数			*/
#define OS_Q_FLUSH_EN             1    /*     Include code for OSQFlush()                              *//* 是否使用OSQFlush()函数			*/
#define OS_Q_POST_EN              1    /*     Include code for OSQPost()                               *//* 是否使用OSQPost()函数			*/
#define OS_Q_POST_FRONT_EN        1    /*     Include code for OSQPostFront()                          *//* 是否使用OSQPostFront()函数			*/
#define OS_Q_POST_OPT_EN          1    /*     Include code for OSQPostOpt()                            *//* 是否使用OSQPostOptA()函数			*/
#define OS_Q_QUERY_EN             1    /*     Include code for OSQQuery()                              *//* 是否使用OSQQuery()函数			*/

				       /*                            信号量			       */
                                       /* ------------------------ SEMAPHORES ------------------------ */
#define OS_SEM_EN                 1    /* Enable (1) or Disable (0) code generation for SEMAPHORES     *//* 设定是否使用信号量函数及其相关的数据结构	*/
#define OS_SEM_ACCEPT_EN          1    /*    Include code for OSSemAccept()                            *//* 是否使用OSSemAccept()函数			*/
#define OS_SEM_DEL_EN             1    /*    Include code for OSSemDel()                               *//* 是否使用OSSemDel()函数			*/
#define OS_SEM_QUERY_EN           1    /*    Include code for OSSemQuery()                             *//* 是否使用OSSemQuery()函数			*/

				       /*                         任务管理			       */
                                       /* --------------------- TASK MANAGEMENT ---------------------- */
#define OS_TASK_CHANGE_PRIO_EN    1    /*     Include code for OSTaskChangePrio()                      *//* 是否使用OSTaskChangePrio()函数		*/
#define OS_TASK_CREATE_EN         1    /*     Include code for OSTaskCreate()                          *//* 是否使用OSTaskCreate()函数			*/
#define OS_TASK_CREATE_EXT_EN     1    /*     Include code for OSTaskCreateExt()                       *//* 是否使用OSTaskCteateExt()函数		*/
#define OS_TASK_DEL_EN            1    /*     Include code for OSTaskDel()                             *//* 是否使用OSTaskDel()函数			*/
#define OS_TASK_SUSPEND_EN        1    /*     Include code for OSTaskSuspend() and OSTaskResume()      *//* 是否使用OSTaskSuspend()和OSTaskResume()函数	*/
#define OS_TASK_QUERY_EN          1    /*     Include code for OSTaskQuery()                           *//* 是否使用OSTaskQuery()函数			*/

				       /*                         时间管理			       */
                                       /* --------------------- TIME MANAGEMENT ---------------------- */
#define OS_TIME_DLY_HMSM_EN       1    /*     Include code for OSTimeDlyHMSM()                         *//* 是否使用OSTimeDlyHMSM()函数			*/
#define OS_TIME_DLY_RESUME_EN     1    /*     Include code for OSTimeDlyResume()                       *//* 是否使用OSTimeDlyResume()函数		*/
#define OS_TIME_GET_SET_EN        1    /*     Include code for OSTimeGet() and OSTimeSet()             *//* 是否使用OSTimeGet()和OSTimeSet()函数	*/


typedef INT16U             OS_FLAGS;   /* Date type for event flag bits (8, 16 or 32 bits)             *//* 用来存放信号量集所有信号的状态，每一个信号占据一个二进制位 */
