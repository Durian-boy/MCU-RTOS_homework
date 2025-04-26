/******************************************************************************
 * Copyright (C) 2025 Damian.
 * 
 * All Rights Reserved.
 * 
 * @file bsp_led_driver.h
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
 * @version V1.0 2025-04-26
 *
 * @note 1 tab == 4 spaces!
 * 
 *****************************************************************************/

#ifndef __BSP_LED_DRIVER_H__
#define __BSP_LED_DRIVER_H__

//******************************** Includes *********************************//

#include <stdint.h>
#include <stdio.h>

//******************************** Includes *********************************//


//******************************** Defines **********************************//

#define LED_INITED        1                 /* LED already initialized       */
#define LED_NOT_INITED    0                 /* LED not initialized           */
#define OS_SUPPORTING                       /* OS is available               */
#define CURRENT_LOG_LEVEL LOG_LEVEL_WARN    /* Define the level of log       */
#define LOG(level, fmt, ...) \
        do { \
            if (level >= CURRENT_LOG_LEVEL) { \
                const char *level_str[] = {"DBG ", "INFO", "WARN", "ERR "}; \
                printf("[%s]%s:%d " fmt "\r\n", level_str[level], \
                                              __FILE__, \
                                              __LINE__, \
                                              ##__VA_ARGS__); \
            } \
        } while (0)

typedef enum {
    LOG_LEVEL_DBG       = 0,        /* Print DBG & INFO & WARN & ERR         */
    LOG_LEVEL_INFO      = 1,        /* Print INFO & WARN & ERR               */
    LOG_LEVEL_WARN      = 2,        /* Print WARN & ERR                      */
    LOG_LEVEL_ERR       = 3,        /* Print ERR                             */
    LOG_LEVEL_OFF       = 4,        /* Print nothing                         */
} log_level_t;

typedef enum
{
    LED_OK              = 0,        /* LED operate successfully              */
    LED_ERROR           = 1,        /* LED error without case matched        */
    LED_ERRORTIMEOUT    = 2,        /* LED operate failed with timeout       */
    LED_ERRORSOURCE     = 3,        /* LED resource not available            */
    LED_ERRORPARAMETER  = 4,        /* LED parameter error                   */
    LED_ERRORNOMEMORY   = 5,        /* LED out of memory                     */
    LED_ERRORISR        = 6,        /* LED not allowed in ISR context        */
    LED_RESERVED        = 0xFF,     /* LED reserved                          */
} led_status_t;

typedef struct
{
    led_status_t ( *pf_led_on )  ( void );
    led_status_t ( *pf_led_off ) ( void );
    led_status_t ( *pf_led_ctl ) (
                                   uint32_t,    /* period_ms */
                                   uint32_t,    /* count */
                                   uint32_t     /* duty */
                                            );  
} led_operation_t;

typedef struct
{
    led_status_t ( *pf_get_time_ms )  ( uint32_t * const );
} time_operation_t;

typedef struct
{
    led_status_t ( *pf_os_delay_ms )  ( const uint32_t );
} os_delay_t;

typedef enum
{
    DUTY_00_PERCENT  = 0,
    DUTY_10_PERECNT  = 1,
    DUTY_20_PERCENT  = 2,
    DUTY_30_PERCENT  = 3,
    DUTY_40_PERCENT  = 4,
    DUTY_50_PERCENT  = 5,
    DUTY_60_PERCENT  = 6,
    DUTY_70_PERCENT  = 7,
    DUTY_80_PERCENT  = 8,
    DUTY_90_PERCENT  = 9,
    DUTY_MAX_PERCENT = 0xFF,
} led_duty_t;

typedef struct bsp_led_driver
{
    //************************** Internal status ****************************//
    uint8_t             is_initialized;               /* record init status  */

    //****************************** Property *******************************//
    uint32_t            period_ms;                    /* Period of twinkling */
    uint32_t            count;                        /* Count of twinkling  */
    led_duty_t          duty;                         /* Duty cycle          */

    //************************ Interface from core **************************//
    led_operation_t     * p_led_operation_inst;       /* led ops interface   */
    time_operation_t    * p_time_operation_inst;      /* time ops interface  */

    //************************ Interface from RTOS **************************//
#ifdef OS_SUPPORTING
    os_delay_t          * p_os_delay;                 /* os ops interface    */
#endif /* OS_SUPPORTING */ 
} bsp_led_driver_t;

//******************************** Defines **********************************//

//******************************* Declaring *********************************//

/**
 * @brief: initialize the led
 * @steps:
 *      1. Make the led at the specific status
 * 
 * @param[in]  self:     Pointer to a instance of bsp_led_driver_t
 * 
 * @return led_status_t: execute result of this function
 **/
led_status_t led_driver_init ( bsp_led_driver_t    * const self );

/**
 * @brief: Instantiate a bsp_led_driver_t
 * @steps:
 *      1. Adding the Core interfaces into the instance of bsp_led_driver_t
 *      2. Adding the OS interfaces into the instance of bsp_led_driver_t
 *      3. Adding the time opt interfaces into the instance of bsp_led_driver_t
 * 
 * @param[in]  self:     Pointer to a instance of bsp_led_driver_t
 * @param[in]  led_opt:  Pointer to a instance of led_operation_t
 * @param[in]  time_opt: Pointer to a instance of time_base_t
 * @param[in]  os_delay: Pointer to a instance of os_delay_t
 * 
 * @return led_status_t: execute result of this function
 **/
led_status_t led_driver_inst (
                               bsp_led_driver_t    * const self,
                               led_operation_t     * const led_opt,
                               time_operation_t    * const time_opt,
#ifdef OS_SUPPORTING   
                               os_delay_t          * const os_delay
#endif /* OS_SUPPORTING */  
                                                                            );
//******************************* Declaring *********************************//
#endif // __BSP_LED_DRIVER_H__
