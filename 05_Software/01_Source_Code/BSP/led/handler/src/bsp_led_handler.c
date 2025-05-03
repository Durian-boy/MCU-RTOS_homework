/******************************************************************************
 * Copyright (C) 2025 Damian.
 * 
 * All Rights Reserved.
 * 
 * @file bsp_led_handler.h
 * 
 * @par dependencies
 * - bsp_led_handler.h
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

//******************************** Includes *********************************//

#include "bsp_led_handler.h"
#include "stdio.h"

//******************************** Includes *********************************//

//******************************** Defines **********************************//

/**
 * @brief: Operate the led
 * @steps:
 *      1. Analyze the parameters of led
 *      2. do the appropriate operation of led
 * 
 * @param[in]  self:   Pointer to a instance of bsp_led_driver_t
 * 
 * @return led_status_t: execute result of this function
 **/
static led_handler_status_t led_twinkle ( 
                                  bsp_led_handler_t * const led_handler,
                                  bsp_led_driver_t  * const led_inst
                                                                        )
{
    uint32_t             period_local;
    uint32_t             count_local;
    led_duty_t           duty_local;
    uint32_t             turn_on_time;
    uint32_t             turn_off_time;
    led_handler_status_t ret           = LED_HNDLR_OK;
    /************** 1. Checking the instance **************/
    // No need to check it again, already checked in led_ctrl

    /****************** 2. Do opreration ******************/
    period_local   = led_inst->period_ms;
    count_local    = led_inst->count;
    duty_local     = led_inst->duty;
    turn_on_time   = ( duty_local * period_local ) / 10;
    turn_off_time  = period_local - turn_on_time;
    
    for ( uint32_t i = 0; i < count_local; ++i )
    {
        /* @TODO: need to consider multi_task*/
        // 2.1 turn on the led
        led_inst->p_led_operation_inst->pf_led_on();
        // 2.2 wait for turn_on_time
        led_handler->p_os_delay->pf_os_delay_ms( turn_on_time );
        // 2.3 turn off the led
        led_inst->p_led_operation_inst->pf_led_off();
        // 2.4 wait for turn_off_time
        led_handler->p_os_delay->pf_os_delay_ms( turn_off_time );
    }

    return ret;
}
 
/**
 * @brief: Control the behavior of led
 * @steps:
 *      1. Set the values of parameter of led
 *      2. control the led
 * 
 * @param[in]  led_handler:   Pointer to a instance of bsp_led_handler_t
 * @param[in]  led_inst:      Pointer to a instance of bsp_led_driver_t
 * @param[in]  period:        Period of twinkling
 * @param[in]  count:         Count of twinkling
 * @param[in]  duty:          Duty cycle
 * 
 * @return led_status_t: execute result of this function
 **/
static led_handler_status_t led_ctrl (
                       bsp_led_handler_t * const led_handler, /* led handler */
                       bsp_led_driver_t  * const led_inst,    /* led inst    */
                       uint32_t                  period,      /* period_ms   */
                       uint32_t                  count,       /* count       */
                       led_duty_t                duty         /* duty        */
                                                              )
{
    led_handler_status_t ret = LED_HNDLR_OK;

    /************** 1. Checking the instance **************/
    if ( NULL == led_inst )
    {
        LOG( LOG_LEVEL_ERR, "Input parameter is null" );
        ret = LED_HNDLR_ERRORPARAMETER;
        return ret;
    }
    else if ( LED_HANDLER_NOT_INITED ==  led_handler->is_initialized)
    {
        LOG( LOG_LEVEL_ERR, "LED handler not initialized" );
        ret = LED_HNDLR_ERRORSOURCE;
        return ret;
    }
    else if ( LED_INST_NOT_INITED == led_inst->is_initialized )
    {
        LOG( LOG_LEVEL_ERR, "LED inst not initialized" );
        ret = LED_HNDLR_ERRORSOURCE;
        return ret;
    }
    
    /********** 2. Checking the input parameters **********/
    if ( period > 10000            ||
         count  > 1000             ||
         duty   > DUTY_MAX_PERCENT
                                     )
    {
        LOG( LOG_LEVEL_ERR, "Parameter err, period:%u, count:%u, duty:%d",
                            period, count, duty);
        ret = LED_HNDLR_ERRORPARAMETER;
        return ret;
    }

    /************* 3. Modify the param of led *************/
    led_inst->period_ms = period;
    led_inst->count     = count;
    led_inst->duty      = duty;

    /*************** 4. Call fun begin work ***************/
    if ( DUTY_00_PERCENT == duty )
        led_inst->p_led_operation_inst->pf_led_off();
    else if ( DUTY_MAX_PERCENT == duty )
        led_inst->p_led_operation_inst->pf_led_on();
    else
        led_twinkle( led_handler, led_inst );

    return ret;
}

/**
 * @brief: Register a led instance into a led handler
 * @steps:
 *      1. Set the values of parameter of led
 * 
 * @param[in]  led_handler:   Pointer to a instance of bsp_led_handler_t
 * @param[in]  led_inst:      Pointer to a instance of bsp_led_driver_t
 * 
 * @return led_handler_status_t: execute result of this function
 **/
static led_handler_status_t led_register (
                      bsp_led_handler_t * const led_handler,  /* led handler */
                      bsp_led_driver_t  * const led_inst      /* led inst    */
                                                            )
{
    led_handler_status_t ret = LED_HNDLR_OK;
    return ret;
}

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
 * 
 * @return led_handler_status_t: execute result of this function
 **/
led_handler_status_t led_handler_inst (
                                        bsp_led_handler_t * const led_handler,
                                        time_operation_t  * const time_ops,
#ifdef OS_SUPPORTING   
                                        os_delay_t        * const os_delay
#endif /* OS_SUPPORTING */  
                                                                             )
{
    led_handler_status_t ret = LED_HNDLR_OK;
    return ret;
}

//******************************** Defines **********************************//
