/******************************************************************************
 * Copyright (C) 2025 Damian.
 * 
 * All Rights Reserved.
 * 
 * @file bsp_led_driver.h
 * 
 * @par dependencies 
 * - bsp_led_driver.h
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

//******************************** Includes *********************************//

#include "bsp_led_driver.h"
#include "stdio.h"

//******************************** Includes *********************************//

//******************************** Defines **********************************//

/**
 * @brief: initialize the led
 * @steps:
 *      1. Make the led at the specific status
 * 
 * @param[in]  self: Pointer to a instance of bsp_led_driver_t
 * 
 * @return led_status_t: execute result of this function
 **/
led_status_t led_driver_init ( bsp_led_driver_t    * const self )
{
    if ( NULL == self )
    {
        LOG( LOG_LEVEL_ERR, "Input parameter is null" );
        return LED_ERRORPARAMETER;
    }
    self->p_led_operation_inst->pf_led_off();
    return LED_OK;
}

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
                               led_operation_t     * const led_ops,
                               time_operation_t    * const time_ops,
#ifdef OS_SUPPORTING   
                               os_delay_t          * const os_delay
#endif /* OS_SUPPORTING */  
                                                                            )
{
    led_status_t ret = LED_OK;

    /********** 1. Checking the input parameters **********/
    if ( 
         NULL == self     ||
         NULL == led_ops  ||
         NULL == time_ops ||
         NULL == os_delay 
                             ) 
    {
        LOG(LOG_LEVEL_ERR, "Input parameter is null");
        return LED_ERRORPARAMETER;
    }

    /************** 2. Checking the resource **************/
    if ( LED_INITED == self->is_initialized )
    {
        LOG( LOG_LEVEL_WARN, "LED already initialized" );
        return LED_ERRORSOURCE;
    }

    /************** 3. Adding the interfaces **************/
    self->p_led_operation_inst      = led_ops;
    self->p_time_operation_inst     = time_ops;
    self->p_os_delay                = os_delay;

    /************* 4. Initialize the instance *************/
    self->period_ms     = 1000;
    self->count         = 0;
    self->duty          = DUTY_00_PERCENT;

    ret = led_driver_init( self );
    if (LED_OK != ret)
    {
        LOG( LOG_LEVEL_ERR, "LED initializ failed" );
        self->p_led_operation_inst      = NULL;
        self->p_time_operation_inst     = NULL;
        self->p_os_delay                = NULL;
        return ret;
    }

    self->is_initialized = LED_INITED;
    return ret;
}
//******************************** Defines **********************************//
