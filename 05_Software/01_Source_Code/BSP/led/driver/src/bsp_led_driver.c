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
 * @brief: Operate the led
 * @steps:
 *      1. Analyze the parameters of led
 *      2. do the appropriate operation of led
 * 
 * @param[in]  self:   Pointer to a instance of bsp_led_driver_t
 * 
 * @return led_status_t: execute result of this function
 **/
static led_status_t led_driver_twinkle ( bsp_led_driver_t * const self )
{
    uint32_t     period_local;
    uint32_t     count_local;
    led_duty_t   duty_local;
    uint32_t     turn_on_time;
    uint32_t     turn_off_time;
    led_status_t ret = LED_OK;
    /************** 1. Checking the instance **************/
    if ( NULL == self )
    {
        LOG( LOG_LEVEL_ERR, "Input parameter is null" );
        ret = LED_ERRORPARAMETER;
        return ret;
    }
    else if ( LED_NOT_INITED == self->is_initialized )
    {
        LOG( LOG_LEVEL_ERR, "LED not initialized" );
        ret = LED_ERRORSOURCE;
        return ret;
    }

    /****************** 2. Do opreration ******************/
    period_local   = self->period_ms;
    count_local    = self->count;
    duty_local     = self->duty;
    turn_on_time   = ( duty_local * period_local ) / 10;
    turn_off_time  = period_local - turn_on_time;
    
    for ( uint32_t i = 0; i < count_local; ++i )
    {
        /* @TODO: need to consider multi_task*/
        // 2.1 turn on the led
        self->p_led_operation_inst->pf_led_on();
        // 2.2 wait for turn_on_time
        self->p_os_delay->pf_os_delay_ms( turn_on_time );
        // 2.3 turn off the led
        self->p_led_operation_inst->pf_led_off();
        // 2.4 wait for turn_off_time
        self->p_os_delay->pf_os_delay_ms( turn_off_time );
    }

    return ret;
}


 
/**
 * @brief: Control the behavior of led
 * @steps:
 *      1. Set the values of parameter of led
 * 
 * @param[in]  self:   Pointer to a instance of bsp_led_driver_t
 * @param[in]  period: Period of twinkling
 * @param[in]  count:  Count of twinkling
 * @param[in]  duty:   Duty cycle
 * 
 * @return led_status_t: execute result of this function
 **/
led_status_t led_driver_ctrl (
                            bsp_led_driver_t * const self,      /* led inst  */
                            uint32_t                 period,    /* period_ms */
                            uint32_t                 count,     /* count     */
                            led_duty_t               duty       /* duty      */
                                                            )
{
    led_status_t ret = LED_OK;

    /************** 1. Checking the instance **************/
    if ( NULL == self )
    {
        LOG( LOG_LEVEL_ERR, "Input parameter is null" );
        ret = LED_ERRORPARAMETER;
        return ret;
    }
    else if ( LED_NOT_INITED == self->is_initialized )
    {
        LOG( LOG_LEVEL_ERR, "LED not initialized" );
        ret = LED_ERRORSOURCE;
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
        ret = LED_ERRORPARAMETER;
        return ret;
    }

    /************* 3. Modify the param of led *************/
    self->period_ms = period;
    self->count     = count;
    self->duty      = duty;

    /*************** 4. Call fun begin work ***************/
    if ( DUTY_00_PERCENT == duty )
        self->p_led_operation_inst->pf_led_off();
    else if (DUTY_MAX_PERCENT == duty)
        self->p_led_operation_inst->pf_led_on();
    else
        led_driver_twinkle(self);

    return ret;
}

/**
 * @brief: initialize the led
 * @steps:
 *      1. Make the led at the specific status
 * 
 * @param[in]  self: Pointer to a instance of bsp_led_driver_t
 * 
 * @return led_status_t: execute result of this function
 **/
led_status_t led_driver_init ( bsp_led_driver_t * const self )
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
 * @param[in]  led_opt:  Pointer to a instance of led_on_off_t
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
