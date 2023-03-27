/*
 * timers.h
 *
 * Created: 28/07/2022 11:57:59 
 *  Author: Mahmoud Abdelmoniem
 */ 


#ifndef TIMERS_H_
#define TIMERS_H_

#include "avr/io.h"
#include "avr/interrupt.h"
#include "../std_types.h"
#include "../Bit_Config.h"

#define TIMER0_DISABLE               0X00
#define TIMER0_1_PRESCALER           0X01
#define TIMER0_8_PRESCALER           0X02
#define TIMER0_64_PRESCALER          0X03
#define TIMER0_256_PRESCALER         0X04
#define TIMER0_1024_PRESCALER        0X05
#define TIMER0_EXITRNAL_FALLING      0X06
#define TIMER0_EXITRNAL_RISING       0X07

#define TIMER0_INTERRUPT_ENABLE()           (TIMSK = TIMSK | 0x01)
#define TIMER0_INTERRUPT_DISABLE()          (TIMSK = TIMSK | 0x00)

#define TIMER0_OC_INTERRUPT_ENABLE()        (TIMSK = TIMSK | 0x02)
#define TIMER0_OC_INTERRUPT_DISABLE()       (TIMSK = TIMSK | 0x00) 


#define SET_PERSCALLEER(_PRESCALER)        (TCCR0 = TCCR0 | _PRESCALER)
#define SET_OCO_MODE(_MODE)                (TCCR0 = TCCR0 | (_MODE<<4))

typedef enum{
	TIMER0_NOTMAL_MODE,
	TIMER0_PHASSCORRECT_MODE,
	TIMER0_CTC_MODE,
	TIMER0_FASTPWM_MODE,
}timer0_modes_t; 

typedef enum {
	OCO_DISCONNECTED=0,
	OCO_TOGGLE,
	OCO_NON_INVERTING,
	OCO_INVERTING
}OCO_mode_t;

typedef struct {
	timer0_modes_t mode;
	uint8 prescales;
	OCO_mode_t oc_mode;
	void (*Tmr0_OV_interruptHandler) (void);
	void (*Tmr0_OC_interruptHandler) (void);
}timer0_t;
	
	
Std_ReturnType TMR0_init(const timer0_t *_tmer0);	
Std_ReturnType TMr0_Write_Value(uint8 timer0);
Std_ReturnType TMr0_Read_Value(uint8 *timer0);
Std_ReturnType TMR0_Iterrupt_Enable(const timer0_t *_tmer0);
Std_ReturnType TMR0_Iterrupt_Disnable(const timer0_t *_tmer0);
Std_ReturnType TMR0_OC_Iterrupt_Enable(const timer0_t *_tmer0);
Std_ReturnType TMR0_OC_Iterrupt_Disnable(const timer0_t *_tmer0);
#endif /* TIMERS_H_ */