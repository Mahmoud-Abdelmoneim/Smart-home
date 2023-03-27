/*
 * GPIO.h
 *
 * Created: 25/07/2022 02:54:13 
 *  Author:Mahmoud Abdelmoniem
 */ 


#ifndef GPIO_H_
#define GPIO_H_
#define F_CPU 8000000UL
#include "avr/io.h"
#include "../std_types.h"
#include "../Bit_Config.h"
#include "util/delay.h"

/*.............section: macro declaration.....*/

#define MASK_PIN    1
/*...................section: macro function declaration.......*/
/*
#define SET_BIT(_REG,PIN)      (_REG=_REG | (MASK_PIN <<PIN))
#define CLEAR_BIT(_REG,PIN)    (_REG=_REG & (~(MASK_PIN<<PIN)))
#define TOGGLE_BIT(_REG,PIN)   (_REG=_REG ^ (MASK_PIN<<PIN))
#define READ_BIT(_REG,PIN)     ((_REG>>PIN)  & MASK_PIN)
*/
/*..........section: data type declaration..............*/

typedef enum{
	GPIO_LOW=0,
	GPIO_HIGH
}logic_t;

typedef enum{
	PORTA_INDEX=0,
	PORTB_INDEX,
	PORTC_INDEX,
	PORTD_INDEX
	
}port_index_t;

typedef enum{
	GPIO_PIN0=0,
	GPIO_PIN1,
	GPIO_PIN2,
	GPIO_PIN3,
	GPIO_PIN4,
	GPIO_PIN5,
	GPIO_PIN6,
	GPIO_PIN7
	
}pin_number;

typedef enum{
	GPIO_DIRECTION_INPUT=0,
	GPIO_DIRECTION_OUTPUT=1
}pin_direction_t;

typedef struct{
	port_index_t port;
	pin_number pin;
	pin_direction_t pin_direction;
	logic_t pin_logic;
	
}pin_cofig_t;
/************************************************************************/
/*...............section:function declaration ................          */
/************************************************************************/
/*...............section:function declaration  */
Std_ReturnType gpio_pin_dirction_init(const pin_cofig_t* _pin_obj);
Std_ReturnType gpio_get_pin_direction(const pin_cofig_t*_pin_obj,pin_direction_t* dirction);
Std_ReturnType gpio_write_logic(const pin_cofig_t *pin_cofid_t,logic_t _logic);
Std_ReturnType gpio_read_logic(const pin_cofig_t *pin_cofid_t,logic_t* _logic);
Std_ReturnType gpio_pin_toggle(const pin_cofig_t *pin_cofid_t);
Std_ReturnType gpio_pin_init(const pin_cofig_t *pin_cofid_t);



#endif /* GPIO_H_ */