/******************************************************************************
 * Copyright (C) 2025 Damian.
 * 
 * All Rights Reserved.
 * 
 * @file bsp_led_driver.c
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
 * @param[in]  led_inst: Pointer to a instance of bsp_led_driver_t
 * 
 * @return led_inst_status_t: execute result of this function
 **/
static led_inst_status_t led_inst_init ( bsp_led_driver_t * const led_inst )
{
    if ( NULL == led_inst )
    {
        LOG( LOG_LEVEL_ERR, "Input parameter is null" );
        return LED_INST_ERRORPARAMETER;
    }
    led_inst->p_led_operation_inst->pf_led_off();
    return LED_INST_OK;
}

/**
 * @brief: Instantiate a bsp_led_driver_t
 * @steps:
 *      1. Adding the Core interfaces into the instance of bsp_led_driver_t
 * 
 * @param[in]  led_inst: Pointer to a instance of bsp_led_driver_t
 * @param[in]  led_opt:  Pointer to a instance of led_on_off_t
 * 
 * @return led_inst_status_t: execute result of this function
 **/
led_inst_status_t led_instantiate (
                               bsp_led_driver_t    * const led_inst,
                               led_operation_t     * const led_ops
                                                                    )
{
    led_inst_status_t ret = LED_INST_OK;

    /********** 1. Checking the input parameters **********/
    if ( 
         NULL == led_inst ||
         NULL == led_ops
                            ) 
    {
        LOG(LOG_LEVEL_ERR, "Input parameter is null");
        return LED_INST_ERRORPARAMETER;
    }

    /************** 2. Checking the resource **************/
    if ( LED_INST_INITED == led_inst->is_initialized )
    {
        LOG( LOG_LEVEL_WARN, "LED inst already initialized" );
        return LED_INST_ERRORSOURCE;
    }

    /************** 3. Adding the interfaces **************/
    led_inst->p_led_operation_inst      = led_ops;

    /************* 4. Initialize the instance *************/
    led_inst->period_ms     = 1000;
    led_inst->count         = 0;
    led_inst->duty          = DUTY_00_PERCENT;

    ret = led_inst_init( led_inst );
    if (LED_INST_OK != ret)
    {
        LOG( LOG_LEVEL_ERR, "LED inst initializ failed" );
        led_inst->p_led_operation_inst = NULL;
        return ret;
    }

    led_inst->is_initialized = LED_INST_INITED;
    return ret;
}
//******************************** Defines **********************************//
