/*
 * safe.c
 *
 * Created: 25/07/2022 02:44:33 
 * Author : Mahmoud Abdelmoniem
 */ 



#include "MCAL/GPIO/GPIO.h"
#include "ECU/led.h"
#include "ECU/button1.h"
#include "ECU/ecu_lcd_char.h"
#include "ECU/ecu_keypad.h"
#include "MCAL/interrupt/EX_interrupt.h"
#include "MCAL/timer0/timer0.h"
#include "MCAL/timer1/timer1.h"
#include "ECU/ultrasonic.h"
#include "MCAL/UART/UART.h"
#include "services/UART_Services.h"
#include "MCAL/ADC/ADC.h"

Std_ReturnType ret;

void uart_interrupt(void);

led_t rom1_led1={.port_name=PORTB_INDEX,.pin=GPIO_PIN0,.led_status=LED_OFF};
led_t rom1_led2={.port_name=PORTB_INDEX,.pin=GPIO_PIN1,.led_status=LED_OFF};
led_t rom2_led1={.port_name=PORTB_INDEX,.pin=GPIO_PIN5,.led_status=LED_OFF};
led_t rom2_led2={.port_name=PORTB_INDEX,.pin=GPIO_PIN6,.led_status=LED_OFF};
led_t reception_led1={.port_name=PORTB_INDEX,.pin=GPIO_PIN2,.led_status=LED_OFF};
led_t reception_led2={.port_name=PORTB_INDEX,.pin=GPIO_PIN3,.led_status=LED_OFF};
led_t reception_led3={.port_name=PORTB_INDEX,.pin=GPIO_PIN4,.led_status=LED_OFF};	
	
void ADC_Intrrupt();
	
ADC_t adc_obj={
	.prescaler=ADC_PRESCALER_16,
	.revrance=ADC_VCC_VOLTAGE_REV,
	.start_conversion_type=ADC_AUTO_START_DISABLE,
	.read_adjust=ADC_READ_RIGHT_ADJUST,
	.channle=ADC_CHANNLE_8,
	//.ADC_InterruptCallBack=ADC_Intrrupt
};	
/******************Variable Declaration***************************/

volatile uint8 read_uart=0,C=0; 
uint16 adc_value=0;
uint8 temp=0;
uint8 send=0;
int main(void)
{
	sei();
	ret = led_intialize(&rom1_led1); 
	ret = led_intialize(&rom1_led2);
	ret = led_intialize(&rom2_led1);
	ret = led_intialize(&rom2_led2);
	ret = led_intialize(&reception_led1);
	ret = led_intialize(&reception_led2);
	ret = led_intialize(&reception_led3);
	
	ret = ADC_Init(&adc_obj);
	UART_Init();
	//ret = ADC_Interript_Enable(&adc_obj);
		
	UART_Init();
	UART_RX_InterruptEnable();
	
	UART_RX_SetCallBack(uart_interrupt);
	while(1)
	{
	   ret = ADC_Read(&adc_obj,&adc_value);
	   temp = 	((float)adc_value*.0048828)/.01;
	   if (read_uart == 's'){
		for(uint8 counter=0;counter<2;counter++)
			{
				send=temp%10;
				send =send + '0';
				UART_SendByte(send);
				temp/=10;
				_delay_ms(1000);
			}
		}


	    else if(read_uart == '1')
		{
			ret = led_turn_on(&rom1_led1);
		}
		else if(read_uart == '2')
		{
			ret = led_turn_off(&rom1_led1);
		}
		
		
		else if(read_uart == '7')
		{
			ret = led_turn_on(&rom1_led2);
		}
		else if(read_uart == '8')
		{
			ret = led_turn_off(&rom1_led2);
		}		
		
		
		else if(read_uart == '3' )
		{
			ret = led_turn_on(&rom2_led1);
		}
		else if(read_uart == '4' )
		{
			ret = led_turn_off(&rom2_led1);
	    }
		
		
		
		else if(read_uart == '9' )
		{
			ret = led_turn_on(&rom2_led2);
		}
		else if(read_uart == 'a' )
		{
			ret = led_turn_off(&rom2_led2);
		}		
		
		else if(read_uart == '5' )
		{
			ret = led_turn_on(&reception_led1);
		}
		else if(read_uart == '6' )
		{
			ret = led_turn_off(&reception_led1);
		}	
		else if(read_uart == 'b' )
		{
			ret = led_turn_on(&reception_led2);
		}
		else if(read_uart == 'c' )
		{
			ret = led_turn_off(&reception_led2);
		}
		else if(read_uart == 'd' )
		{
			ret = led_turn_on(&reception_led3);
		}
		else if(read_uart == 'e' )
		{
			ret = led_turn_off(&reception_led3);
		}
			
					
	}
	
}
void uart_interrupt(void)
{
	
	read_uart = UART_ReceiveByteNoBlock();
}
