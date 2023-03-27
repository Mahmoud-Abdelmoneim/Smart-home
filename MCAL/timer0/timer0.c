/*
 * timers.c
 *
 * Created: 28/07/2022 11:57:45 
 *  Author:Mahmoud Abdelmoniem
 */ 
#include "timer0.h"
/************************************************************************/
/*                    Timer0 Implementation                             */
/************************************************************************/
static void (*tmr0_interruptHandler) (void);
static void (*Tmr0_OC_interruptHandler)(void);
//static void timer0_select_select_mode(const timer0_t *tmer0);
Std_ReturnType TMR0_init(const timer0_t *_tmer0){
		Std_ReturnType ret=E_OK;
		if(NULL == _tmer0)
		{
			ret = E_NOT_OK;
		}
		else{
			switch(_tmer0->mode)
			{
				case TIMER0_NOTMAL_MODE:
				CLEAR_BIT(TCCR0,WGM00);
				CLEAR_BIT(TCCR0,WGM01);
				break;
				case TIMER0_PHASSCORRECT_MODE:
				SET_BIT(TCCR0,WGM00);
				CLEAR_BIT(TCCR0,WGM01);
				break;
				case TIMER0_CTC_MODE:
				CLEAR_BIT(TCCR0,WGM00);
				SET_BIT(TCCR0,WGM01);
				break;
				case TIMER0_FASTPWM_MODE:
				SET_BIT(TCCR0,WGM00);
				SET_BIT(TCCR0,WGM01);
				break;
			}
			SET_PERSCALLEER(_tmer0->prescales);
			SET_OCO_MODE(_tmer0->oc_mode);
			if(_tmer0->oc_mode != OCO_DISCONNECTED)
			{
				SET_BIT(DDRB,3);	
			}
			
		}
		return ret;
}

Std_ReturnType TMr0_Write_Value(uint8 timer0){
	Std_ReturnType ret=E_OK;
	TCNT0=timer0;
	return ret;
}
Std_ReturnType TMr0_Read_Value(uint8 *timer0)
{	
	Std_ReturnType ret=E_OK;
	*timer0=TCNT0;
	return ret;
}
Std_ReturnType TMR0_Iterrupt_Enable(const timer0_t *_tmer0){
	Std_ReturnType ret=E_OK;
	if(NULL == _tmer0)
	{
		ret = E_NOT_OK;
	}
	else
	{
		tmr0_interruptHandler=_tmer0->Tmr0_OV_interruptHandler;
		TIMER0_INTERRUPT_ENABLE() ;
		
	
	}
	return ret;
}
Std_ReturnType TMR0_Iterrupt_Disnable(const timer0_t *_tmer0){
	Std_ReturnType ret=E_OK;
	if(NULL == _tmer0)
	{
		ret = E_NOT_OK;
	}
	else
	{
		tmr0_interruptHandler=NULL;
		TIMER0_INTERRUPT_DISABLE() ;
		
		
	}
	return ret;
}

Std_ReturnType TMR0_OC_Iterrupt_Enable(const timer0_t *_tmer0)
{
	Std_ReturnType ret=E_OK;
	if(NULL == _tmer0)
	{
		ret = E_NOT_OK;
	}
	else
	{
		TIMER0_OC_INTERRUPT_ENABLE();
		Tmr0_OC_interruptHandler =  _tmer0->Tmr0_OC_interruptHandler;

	}
	return ret;
	
}
Std_ReturnType TMR0_OC_Iterrupt_Disnable(const timer0_t *_tmer0)
{
	Std_ReturnType ret=E_OK;
	if(NULL == _tmer0)
	{
		ret = E_NOT_OK;
	}
	else
	{
		TIMER0_OC_INTERRUPT_DISABLE();
	}
	return ret;	
}
/*
ISR(TIMER0_OVF_vect){
	if(tmr0_interruptHandler != NULL)
		tmr0_interruptHandler();
}

ISR(TIMER0_COMP_vect){
	if(Tmr0_OC_interruptHandler)
		Tmr0_OC_interruptHandler();
}

*/