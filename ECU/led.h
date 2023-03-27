/*
 * led.h
 *
 * Created: 25/07/2022 04:58:16 
 *  Author: Mahmoud Abdelmoniem
 */ 


#ifndef LED_H_
#define LED_H_
#include "../MCAL/GPIO/GPIO.h"
#include "stdio.h"
/*........section: macro declarations......... */



/*........section: macro function declarations......... */


/*........section: data type declarations......... */
typedef enum {
	LED_OFF,
	LED_ON
}led_status_t;

typedef struct {
	uint8 port_name:4;
	uint8 pin:3;
	uint8 led_status:1;
}led_t;


/*........section: function declarations......... */
Std_ReturnType led_intialize(const led_t *led );
Std_ReturnType led_turn_on(const led_t *led);
Std_ReturnType led_turn_off(const led_t *led);
Std_ReturnType led_turn_toggle(const led_t *led);




#endif /* LED_H_ */