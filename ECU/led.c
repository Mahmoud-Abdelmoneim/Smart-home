/*
 * led.c
 *
 * Created: 25/07/2022 04:58:03 
 *  Author: Mahmoud Abdelmoniem
 */ 
#include "../ECU/led.h"

Std_ReturnType led_intialize(const led_t *led )
{
    Std_ReturnType ret= E_OK;
    if(led == NULL)
    {
        ret=E_NOT_OK;
    }
    else
    {
        pin_cofig_t pin_obj={.port=led->port_name,.pin=led->pin,
                              .pin_logic=led->led_status,.pin_direction=GPIO_DIRECTION_OUTPUT}; 
        gpio_pin_init(&pin_obj);  
        
    }   
    return ret;
}
/**
 * 
 * @param led:pointer to led module configuration
 * @return:status of the function 
 *        (E_OK)    : the function done successfully 
 *        (E_NOT_OK): the function has issue to perform this action 
 */
Std_ReturnType led_turn_on(const led_t *led)
{
    Std_ReturnType ret=E_OK;
     if(NULL==led)
    {
        ret=E_NOT_OK;
    }
    else
    {
          pin_cofig_t pin_obj={.port=led->port_name,.pin=led->pin,
                                .pin_logic=led->led_status,.pin_direction=GPIO_DIRECTION_OUTPUT};
          gpio_write_logic(&pin_obj,GPIO_HIGH);
        
    }
     
     
    return ret;

}
/**
 * 
 * @param led:pointer to led module configuration
 * @return :status of the function 
 *        (E_OK)    : the function done successfully 
 *        (E_NOT_OK): the function has issue to perform this action
 */

Std_ReturnType led_turn_off(const led_t *led)
{
    Std_ReturnType ret=E_OK;
    if(NULL==led)
    {
        ret=E_NOT_OK;
    }
    else
    {
        pin_cofig_t pin_obj={.port=led->port_name,.pin=led->pin,
                              .pin_logic=led->led_status,.pin_direction=GPIO_DIRECTION_OUTPUT};
          gpio_write_logic(&pin_obj,GPIO_LOW);
    }
    return ret;
}
/**
 * 
 * @param led:pointer to led module configuration
 * @return:status of the function 
 *        (E_OK)    : the function done successfully 
 *        (E_NOT_OK): the function has issue to perform this action 
 */

Std_ReturnType led_turn_toggle(const led_t *led)
{
    Std_ReturnType ret=E_OK;
     if(NULL==led)
    {
        ret=E_NOT_OK;
    }
    else
    {
          pin_cofig_t pin_obj={.port=led->port_name,.pin=led->pin,
                              .pin_logic=led->led_status,.pin_direction=GPIO_DIRECTION_OUTPUT};
          gpio_pin_toggle(&pin_obj);
        
    }
     
     
    return ret;
}
