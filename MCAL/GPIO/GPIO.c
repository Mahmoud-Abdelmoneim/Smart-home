/*
 * GPIO.c
 *
 * Created: 25/07/2022 02:53:48 
 *  Author: Mahmoud Abdelmoniem
 */ 

#include "../GPIO/GPIO.h"


volatile uint8 *ddr_regester[]={&DDRA,&DDRB,&DDRC,&DDRD};
volatile uint8 *port_regster[]={&PORTA,&PORTB,&PORTC,&PORTD}; 
volatile uint8 *pin_regester[]={&PINA,&PINB,&PINC,&PIND};		
	
Std_ReturnType gpio_pin_dirction_init(const pin_cofig_t* _pin_obj){
	Std_ReturnType ret =E_OK;
	if(_pin_obj == NULL){
		ret = E_NOT_OK;
	}
	else{
		switch(_pin_obj->pin_direction){
			case GPIO_DIRECTION_OUTPUT:
			SET_BIT(*(ddr_regester[_pin_obj->port]),_pin_obj->pin);
			
			break;
			case GPIO_DIRECTION_INPUT:
			CLEAR_BIT(*(ddr_regester[_pin_obj->port]),_pin_obj->pin);
			break;
			default: {ret = E_NOT_OK;}
		}
	}
	return ret;
}
Std_ReturnType gpio_get_pin_direction(const pin_cofig_t*_pin_obj,pin_direction_t* dirction){
	Std_ReturnType ret =E_OK;
	if(_pin_obj == NULL || dirction == NULL){
		ret = E_NOT_OK;
	}
	else{
		*dirction = READ_BIT(*ddr_regester[_pin_obj->port],_pin_obj->pin);
	}
	return ret;
}
Std_ReturnType gpio_write_logic(const pin_cofig_t *_pin_obj,logic_t _logic){
	Std_ReturnType ret =E_OK;
	if(_pin_obj == NULL){
		ret = E_NOT_OK;
	}
	else{
		switch(_logic){
			case GPIO_HIGH:
			SET_BIT(*(port_regster[_pin_obj->port]),_pin_obj->pin);
			
			break;
			case GPIO_LOW:
			CLEAR_BIT(*(port_regster[_pin_obj->port]),_pin_obj->pin);
			break;
			default: {ret = E_NOT_OK;}
		}
	}
	return ret;
}
Std_ReturnType gpio_read_logic(const pin_cofig_t *_pin_obj,logic_t* _logic){
	Std_ReturnType ret =E_OK;
	if(_pin_obj == NULL || _logic ==NULL ){
		ret = E_NOT_OK;
	}
	else{
		*_logic = READ_BIT(*pin_regester[_pin_obj->port],_pin_obj->pin);
	}
	return ret;
}
Std_ReturnType gpio_pin_toggle(const pin_cofig_t *_pin_obj){
	Std_ReturnType ret =E_OK;
	if(_pin_obj == NULL){
		ret = E_NOT_OK;
	}
	else{
		TOGGLE_BIT(*port_regster[_pin_obj->port],_pin_obj->pin);
	}
	return ret;
}
Std_ReturnType gpio_pin_init(const pin_cofig_t *pin_cofid_t){
		Std_ReturnType ret =E_OK;
		if(pin_cofid_t == NULL){
			ret = E_NOT_OK;
		}
		else{
			ret = gpio_pin_dirction_init(pin_cofid_t);
			ret = gpio_write_logic(pin_cofid_t,pin_cofid_t->pin_logic);
			
		}
		return ret;
}
