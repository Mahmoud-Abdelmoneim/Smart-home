/*
 * ADC.h
 *
 * Created: 29/07/2022 10:12:52 
 *  Author: Mahmoud Abdelmoniem
 */ 


#ifndef ADC_H_
#define ADC_H_
#define F_CPU 8000000UL
/************************************************************************/
/*                       Section:Includes                               */
/************************************************************************/
#include "avr/io.h"
#include "avr/interrupt.h"
#include "../std_types.h"
#include "../Bit_Config.h"
#include "util/delay.h"
/************************************************************************/
/*                      Section:Macro Declaration                       */
/************************************************************************/

#define ADC_INTERNAL_VOLTAGE_REV_OFF     0X00
#define ADC_VCC_VOLTAGE_REV              0X01
#define ADC_25_INTERNAL_VOLTAGE_REV      0X03

#define ADC_FREE_RUNNING_MODE            0X00
#define ADC_ANALOG_COMPARTOR             0X01
#define ADC_EXTERNAL_INTERRUPT_REQ       0X02
#define ADC_TMR0_COUNTER_COMPARE_MATCH   0X03
#define ADC_TMR0_COUNTER_OVERFLOW        0X04
#define ADC_TMR_COUNTER_COMPARE_MATCH    0X05
#define ADC_TMR1_COUNTER_OVERFLOW        0X06
#define ADC_TMR1_COUNTER_CAPTURE_EVENT   0X07

#define ADC_AUTO_START_DISABLE                 0
#define ADC_AUTO_START_ENABLE                  1

#define ADC_READ_RIGHT_ADJUST            0X00
#define ADC_READ_LEFT_ADJUST             0X01


/************************************************************************/
/*             Section:Function Like Macro Declaration                  */
/************************************************************************/
#define ADC_SET_VOLTAGE_REV(_REV)           (ADMUX = ADMUX | (_REV<<6))

#define ADC_SET_PRESCALER(_PRESCALER)       (ADCSRA = ADCSRA | _PRESCALER)

#define ADC_SET_READ_ADJUST(_ADJUST)        (ADMUX = ADMUX | (_ADJUST<<5))

#define ADC_SELECT_AUTO_TRIGGER(_TRIGGER)   (SFIOR = SFIOR | (_TRIGGER<<5))

#define ADC_INTERRUPT_ENABLE()              (ADCSRA= ADCSRA | (0x09))
#define ADC_INTERRUPT_DISABLE()             (ADCSRA= ADCSRA & (0xF7))

/************************************************************************/
/*                Section:Data Type Declaration                         */
/************************************************************************/
typedef enum {
	
	ADC_CHANNLE_1=0,
	ADC_CHANNLE_2,
	ADC_CHANNLE_3,
	ADC_CHANNLE_4,
	ADC_CHANNLE_5,
	ADC_CHANNLE_6,
	ADC_CHANNLE_7,	
}chanlle_slect_t;
typedef enum {
	ADC_PRESCALER_2=0,
	ADC_PRESCALER_2_,
	ADC_PRESCALER_4,
	ADC_PRESCALER_8,
	ADC_PRESCALER_16,
	ADC_PRESCALER_32,
	ADC_PRESCALER_64,
	ADC_PRESCALER_128	
}prescaler_t;

typedef struct {
	prescaler_t prescaler;
	uint8 revrance;
	uint8 start_conversion_type;
	uint8 read_adjust;
	chanlle_slect_t channle;
	uint8 auto_trigger;
    void (*(ADC_InterruptCallBack))(void);
	
}ADC_t;

/************************************************************************/
/*                  Section:Function Declaration                        */
/************************************************************************/
Std_ReturnType ADC_Init(const ADC_t* _adc);
Std_ReturnType ADC_Read(const ADC_t* _adc,uint16 *value_conversion);
Std_ReturnType ADC_Interript_Enable(const ADC_t* _adc);
Std_ReturnType ADC_Interript_Disable();
	
#endif /* ADC_H_ */