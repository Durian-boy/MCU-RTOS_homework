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
#include <stddef.h>

//******************************** Includes *********************************//

typedef struct bsp_led_driver bsp_led_driver_t;

//******************************** Defines **********************************//

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
    LED_INST_INITED     = 0,        /* LED handler initialized               */
    LED_INST_NOT_INITED = 1,        /* LED handler not initialized           */
} led_inst_init_t;

typedef enum
{
    LED_INST_OK              = 0,    /* LED operate successfully             */
    LED_INST_ERROR           = 1,    /* LED error without case matched       */
    LED_INST_ERRORTIMEOUT    = 2,    /* LED operate failed with timeout      */
    LED_INST_ERRORSOURCE     = 3,    /* LED resource not available           */
    LED_INST_ERRORPARAMETER  = 4,    /* LED parameter error                  */
    LED_INST_ERRORNOMEMORY   = 5,    /* LED out of memory                    */
    LED_INST_ERRORISR        = 6,    /* LED not allowed in ISR context       */
    LED_INST_RESERVED        = 0xFF, /* LED reserved                         */
} led_inst_status_t;

typedef struct
{
    led_inst_status_t ( *pf_led_on )  ( void );
    led_inst_status_t ( *pf_led_off ) ( void );
} led_operation_t;

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
    DUTY_MAX_PERCENT = 10,
} led_duty_t;

typedef struct bsp_led_driver
{
    //************************** Internal status ****************************//
    led_inst_init_t     is_initialized;               /* record init status  */
    // TBD: add the mutex or semaphore for thread safe

    //****************************** Property *******************************//
    uint32_t            period_ms;                    /* Period of twinkling */
    uint32_t            count;                        /* Count of twinkling  */
    led_duty_t          duty;                         /* Duty cycle          */

    //************************ Interface from core **************************//
    led_operation_t     * p_led_operation_inst;       /* led ops interface   */
} bsp_led_driver_t;

//******************************** Defines **********************************//

//******************************* Declaring *********************************//

/**
 * @brief: Instantiate a bsp_led_driver_t
 * @steps:
 *      1. Adding the Core interfaces into the instance of bsp_led_driver_t
 *      2. Adding the OS interfaces into the instance of bsp_led_driver_t
 *      3. Adding the time opt interfaces into the instance of bsp_led_driver_t
 * 
 * @param[in]  self:     Pointer to a instance of bsp_led_driver_t
 * @param[in]  led_ops:  Pointer to a instance of led_operation_t
 * 
 * @return led_inst_status_t: execute result of this function
 **/
led_inst_status_t led_instantiate (
                               bsp_led_driver_t    * const led_inst,
                               led_operation_t     * const led_ops
                                                                    );
//******************************* Declaring *********************************//
#endif // __BSP_LED_DRIVER_H__
