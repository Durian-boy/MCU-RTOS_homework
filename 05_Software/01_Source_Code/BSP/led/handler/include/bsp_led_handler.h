/******************************************************************************
 * Copyright (C) 2025 Damian.
 * 
 * All Rights Reserved.
 * 
 * @file bsp_led_handler.h
 * 
 * @par dependencies 
 * - stdio.h
 * - stdint.h
 * 
 * @author Damian
 * 
 * @brief Provide the HAL APIs for LEDs and corresponding operations.
 * 
 * Processing flow:
 * 
 * Call directly.
 * 
 * @version V1.0 2025-05-03
 *
 * @note 1 tab == 4 spaces!
 * 
 *****************************************************************************/

#ifndef __BSP_LED_HANDLER_H__
#define __BSP_LED_HANDLER_H__

//******************************** Includes *********************************//

#include "bsp_led_driver.h"
#include <stdint.h>
#include <stdio.h>

//******************************** Includes *********************************//

typedef struct bsp_led_handler bsp_led_handler_t;

//******************************** Defines **********************************//

#define OS_SUPPORTING                       /* OS is available               */
#define INIT_PATTERN      0xA6A6A6A6U       /* Init pattern for led driver   */
#define MAX_LED_INST_NUM  10                /* Max number of led inst        */

typedef struct bsp_led_driver bsp_led_driver_t;

typedef enum
{
    LED_HANDLER_INITED     = 0,     /* LED handler initialized               */
    LED_HANDLER_NOT_INITED = 1,     /* LED handler not initialized           */
} led_handler_init_t;

typedef enum
{
    LED_HNDLR_OK              = 0,        /* HNDLR operate successfully      */
    LED_HNDLR_ERROR           = 1,        /* HNDLR error                     */
    LED_HNDLR_ERRORTIMEOUT    = 2,        /* HNDLR operate timeout           */
    LED_HNDLR_ERRORSOURCE     = 3,        /* HNDLR resource not available    */
    LED_HNDLR_ERRORPARAMETER  = 4,        /* HNDLR parameter error           */
    LED_HNDLR_ERRORNOMEMORY   = 5,        /* HNDLR out of memory             */
    LED_HNDLR_ERRORISR        = 6,        /* HNDLR not allowed in ISR        */
    LED_HNDLR_RESERVED        = 0xFF,     /* HNDLR reserved                  */
} led_handler_status_t;

typedef led_handler_status_t ( *pf_led_ctrl_t ) (
                        bsp_led_handler_t * const led_handler, /* led ins    */
                        bsp_led_driver_t  * const led_inst,    /* led inst   */
                        uint32_t                 period,       /* period_ms  */
                        uint32_t                 count,        /* count      */
                        led_duty_t               duty          /* duty       */
                                                                );  

typedef led_handler_status_t ( *pf_led_register_t ) ( 
                                    bsp_led_handler_t * const led_handler,
                                    bsp_led_driver_t  * const led_driver
                                                                            );  

typedef struct
{
    led_handler_status_t ( *pf_get_time_ms )  ( uint32_t * const );
} time_operation_t;

#ifdef OS_SUPPORTING
typedef struct
{
    led_handler_status_t ( *pf_os_delay_ms )  ( const uint32_t );
} os_delay_t;

typedef struct
{
    led_handler_status_t ( *pf_os_critical_enter )  ( void );
    led_handler_status_t ( *pf_os_critical_exit )   ( void );
} os_critical_t;

typedef struct
{
    /* OS queue create */
    led_handler_status_t ( *pf_os_queue_create ) (
                                                uint32_t const num,
                                                uint32_t const size,
                                                void **  const queue_handler );
    
    /* OS queue put */
    led_handler_status_t ( *pf_os_queue_put ) (
                                                void *   const queue_handler,
                                                void *   const item,
                                                uint32_t       timeout       );

    /* OS queue get */
    led_handler_status_t ( *pf_os_queue_get ) (
                                                void *   const queue_handler,
                                                void *   const msg,
                                                uint32_t       timeout       );

    /* OS queue delete */
    led_handler_status_t ( *pf_os_queue_delete ) (
                                                void *   const queue_handler
                                                                             );

} os_queue_t;

#endif  // OS_SUPPORTING

typedef struct
{
    uint32_t           led_inst_num;                      /* num of led inst */
    bsp_led_driver_t * led_inst_array[MAX_LED_INST_NUM];  /* led inst array  */
} led_inst_group_t;

typedef struct bsp_led_handler
{
    //************************* Internal property ***************************//
    led_handler_init_t    is_initialized;             /* record init status  */
    led_inst_group_t      * led_inst_group;           /* led inst group      */
    // TBD: add the mutex or semaphore for thread safe           

    //****************************** Property *******************************//
    
    //************************ Interface from core **************************//
    time_operation_t      * p_time_operation_inst;    /* time ops interface  */

    //************************ Interface from RTOS **************************//
#ifdef OS_SUPPORTING
    os_delay_t            * p_os_delay;             /* os delay interface    */
    os_queue_t            * p_os_queue;             /* os queue interface    */
    os_critical_t         * p_os_critical;          /* os critical interface */

#endif /* OS_SUPPORTING */ 

    //************************* Interface for APP ***************************//
    pf_led_ctrl_t         pf_led_ctrl;                /* control the led     */

    //******************** Interface for iternal driver *********************//
    pf_led_register_t     pf_led_register;            /* register led inst   */

} bsp_led_handler_t;

//******************************** Defines **********************************//

//******************************* Declaring *********************************//

/**
 * @brief: Instantiate a bsp_led_handler_t
 * @steps:
 *      1. Adding the Core interfaces into the instance of bsp_led_handler_t
 *      2. Adding the OS interfaces into the instance of bsp_led_handler_t
 *      3. Adding the time interfaces into the instance of bsp_led_handler_t
 * 
 * @param[in]  led_handler: Pointer to a instance of bsp_led_handler_t
 * @param[in]  time_ops:    Pointer to a instance of time_base_t
 * @param[in]  os_delay:    Pointer to a instance of os_delay_t
 * @param[in]  os_queue:    Pointer to a instance of os_queue_t
 * 
 * @return led_handler_status_t: execute result of this function
 **/
led_handler_status_t led_handler_inst (
                                        bsp_led_handler_t * const led_handler,
#ifdef OS_SUPPORTING   
                                        os_delay_t        * const os_delay,
                                        os_queue_t        * const os_queue,
                                        os_critical_t     * const os_critical,
#endif /* OS_SUPPORTING */  
                                        time_operation_t  * const time_ops
                                                                            );

//******************************* Declaring *********************************//
#endif // __BSP_LED_HANDLER_H__
