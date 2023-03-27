/*
 * timer1.c
 *
 * Created: 30/07/2022 11:00:38 
 *  Author: Mahmoud Abdelmoniem
 */ 
#include "timer1.h"

/************************************************************************/
/*              Declaration Static Global Variable                      */
/************************************************************************/
static volatile uint16 Global_u16Time1;
static volatile uint16 Global_u16Time2;
static volatile uint16 Global_u16Time3;
static volatile uint16 Global_u16Flag = 0;
uint16 Local_u16TimeON, Local_u16TimeOFF;
/************************************************************************/
/*                  Declaration Static Function					        */
/************************************************************************/

static void Timer1ICU_InterruptAction(void);


/************************************************************************/
/*           Declaration Pointer To Function For Call Back              */
/************************************************************************/


static void (*(TMR1_InterriptOveFlow)) (void);
static void (*(TMR1_InterriptComA)) (void);
static void (*(TMR1_InterriptComB)) (void);
static void (*(TMR1_InterriptCAPT))(void);


/************************************************************************/
/*         Implementation Function To Initialize Timer1                 */
/************************************************************************/


Std_ReturnType TMR1_Init(const timer1_t *_timer)
{
		Std_ReturnType ret=E_OK;
		if(NULL == _timer)
		{
			ret = E_NOT_OK;
		}
		else{
			switch(_timer->mode)
			{
				case TMR1_NORMAL_MODE:
					CLEAR_BIT(TCCR1A,WGM10);
					CLEAR_BIT(TCCR1A,WGM11);
					CLEAR_BIT(TCCR1B,WGM12);
					CLEAR_BIT(TCCR1B,WGM13);
					break;
				case TMR1_PWM_PASS_CORRECT_8BIT_MODE:
					SET_BIT(TCCR1A,WGM10);
					CLEAR_BIT(TCCR1A,WGM11);
					CLEAR_BIT(TCCR1B,WGM12);
					CLEAR_BIT(TCCR1B,WGM13);
					break;
				case TMR1_PWM_PASS_CORRECT_9BIT_MODE:
					CLEAR_BIT(TCCR1A,WGM10);
					SET_BIT(TCCR1A,WGM11);
					CLEAR_BIT(TCCR1B,WGM12);
					CLEAR_BIT(TCCR1B,WGM13);
					break;
				case TMR1_PWM_PASS_CORRECT_10BIT_MODE:
					SET_BIT(TCCR1A,WGM10);
					SET_BIT(TCCR1A,WGM11);
					CLEAR_BIT(TCCR1B,WGM12);
					CLEAR_BIT(TCCR1B,WGM13);
					break;
				case TMR1_CTC_OCR1A_MODE:
					CLEAR_BIT(TCCR1A,WGM10);
					CLEAR_BIT(TCCR1A,WGM11);
					SET_BIT(TCCR1B,WGM12);
					CLEAR_BIT(TCCR1B,WGM13);
					break;
				case TMR1_PWM_FAST_MODE_8BIT:
					SET_BIT(TCCR1A,WGM10);
					CLEAR_BIT(TCCR1A,WGM11);
					SET_BIT(TCCR1B,WGM12);
					CLEAR_BIT(TCCR1B,WGM13);
					break;
				case TMR1_PWM_FAST_MODE_9BIT:
					CLEAR_BIT(TCCR1A,WGM10);
					SET_BIT(TCCR1A,WGM11);
					SET_BIT(TCCR1B,WGM12);
					CLEAR_BIT(TCCR1B,WGM13);
					break;
				case TMR1_PWM_FAST_MODE_10BIT:
					SET_BIT(TCCR1A,WGM10);
					SET_BIT(TCCR1A,WGM11);
					SET_BIT(TCCR1B,WGM12);
					CLEAR_BIT(TCCR1B,WGM13);
					break;

				case TMR1_PWM_PASS_FRQ_ICR1_MODE:
					CLEAR_BIT(TCCR1A,WGM10);
					CLEAR_BIT(TCCR1A,WGM11);
					CLEAR_BIT(TCCR1B,WGM12);
					SET_BIT(TCCR1B,WGM13);
					break;
				case TMR1_PWM_PASS_FRQ_OCR1A_MODE:
					SET_BIT(TCCR1A,WGM10);
					CLEAR_BIT(TCCR1A,WGM11);
					CLEAR_BIT(TCCR1B,WGM12);
					SET_BIT(TCCR1B,WGM13);
					break;
				case TMR1_PWM_PASS_CORRECT_ICR1_MODE:
					CLEAR_BIT(TCCR1A,WGM10);
					SET_BIT(TCCR1A,WGM11);
					CLEAR_BIT(TCCR1B,WGM12);
					SET_BIT(TCCR1B,WGM13);
					break;
				case TMR1_PWM_PASS_CORRECT_OCR1A_MODE:
					SET_BIT(TCCR1A,WGM10);
					SET_BIT(TCCR1A,WGM11);
					CLEAR_BIT(TCCR1B,WGM12);
					SET_BIT(TCCR1B,WGM13);
					break;
				case TMR1_CTC_ICR1_MODE:
					CLEAR_BIT(TCCR1A,WGM10);
					CLEAR_BIT(TCCR1A,WGM11);
					SET_BIT(TCCR1B,WGM12);
					SET_BIT(TCCR1B,WGM13);
					break;
				case REVERSED:
					SET_BIT(TCCR1A,WGM10);
					CLEAR_BIT(TCCR1A,WGM11);
					SET_BIT(TCCR1B,WGM12);
					SET_BIT(TCCR1B,WGM13);
					break;
				case TMR1_PWM_FAST_MODE_ICR1:
					CLEAR_BIT(TCCR1A,WGM10);
					SET_BIT(TCCR1A,WGM11);
					SET_BIT(TCCR1B,WGM12);
					SET_BIT(TCCR1B,WGM13);
					break;
				case TMR1_PWM_FAST_MODE_OCR1A:
					SET_BIT(TCCR1A,WGM10);
					SET_BIT(TCCR1A,WGM11);
					SET_BIT(TCCR1B,WGM12);
					SET_BIT(TCCR1B,WGM13);
					break;		
			}

			TMR1_SET_PRESCALER(_timer->prescaler);
			TMR1_SELECT_CHANNLE(_timer->channle);


			switch(_timer->fast_pwm)
			{
				case OC1_DISCONNECTED:
					
					switch(_timer->channle)
					{
						case TIMER1_OCR1A:
							CLEAR_BIT(TCCR1A,COM1A0);
							CLEAR_BIT(TCCR1A,COM1A1);

							break;

						case TIMER1_OCR1b:
							CLEAR_BIT(TCCR1A,COM1B0);
							CLEAR_BIT(TCCR1A,COM1B1);				
							break;
					}
					break;
				case OC1_TOGGLE:

					switch(_timer->channle)
					{
						case TIMER1_OCR1A:
							SET_BIT(TCCR1A,COM1A0);
							CLEAR_BIT(TCCR1A,COM1A1);
							DDRD =   0B00100000;
							break;

						case TIMER1_OCR1b:
							SET_BIT(TCCR1A,COM1B0);
							CLEAR_BIT(TCCR1A,COM1B1);
							DDRD =   0B00010000;
							break;
					}						
					break;
				case OC1_NON_INVERTING:
					switch(_timer->channle)
					{
						case TIMER1_OCR1A:
							CLEAR_BIT(TCCR1A,COM1A0);
							SET_BIT(TCCR1A,COM1A1);
							DDRD =   0B00110000;
							break;

						case TIMER1_OCR1b:
							CLEAR_BIT(TCCR1A,COM1B0);
							SET_BIT(TCCR1A,COM1B1);
							DDRD =   0B00110000;
							break;
					}
					break;
				case OC1_INVERTING:
					switch(_timer->channle)
					{
						case TIMER1_OCR1A:
							SET_BIT(TCCR1A,COM1A0);
							SET_BIT(TCCR1A,COM1A1);
							DDRD =   0B00110000;
							break;

						case TIMER1_OCR1b:
							SET_BIT(TCCR1A,COM1B0);
							SET_BIT(TCCR1A,COM1B1);
							DDRD =   0B00110000;
							break;
					}				
					break;
			}



		}
	return ret;
}



/************************************************************************/
/*       Implementation Function To Enable OverFlow Interrupt           */
/************************************************************************/



Std_ReturnType TMR1_Interrupt_Over_Flow_enable(const timer1_t *_timer)
{
		Std_ReturnType ret=E_OK;
		if(NULL == _timer)
		{
			ret = E_NOT_OK;
		}
		else{
				TMR1_INTERRUPT_OVER_FLOW_ENABLE();
				TMR1_InterriptOveFlow=_timer->TMR1_InterriptOveFlow;
		}
	return ret;
}


/************************************************************************/
/*       Implementation Function To Disable OverFlow Interrupt          */
/************************************************************************/


Std_ReturnType TMR1_Interrupt_Over_Flow_Disable()
{
	Std_ReturnType ret=E_OK;
	TMR1_INTERRUPT_OVER_FLOW_DISABLE();
	return ret;
}


/************************************************************************/
/*       Implementation Function To Enable OCR1A Interrupt              */
/************************************************************************/


Std_ReturnType TMR1_Interrupt_COMPA_enable(const timer1_t *_timer)
{
	Std_ReturnType ret=E_OK;
	if(NULL == _timer)
	{
		ret = E_NOT_OK;
	}
	else{
		TMR1_INTERRUPT_COMPA_ENABLE();
		TMR1_InterriptComA=_timer->TMR1_InterriptComA;
	}
	return ret;
}


/************************************************************************/
/*       Implementation Function To Disable OCR1A Interrupt          */
/************************************************************************/


Std_ReturnType TMR1_Interrupt_COMPA_Disable()
{
	Std_ReturnType ret=E_OK;
	TMR1_INTERRUPT_COMPA_DISABLE();
	return ret;
}



/************************************************************************/
/*       Implementation Function To Enable CAPT Interrupt               */
/************************************************************************/



Std_ReturnType TMR1_Interrupt_CAPT_enable(void (*fun)(void))
{
	Std_ReturnType ret=E_OK;
	

		TMR1_INTERRUPT_CAPT_ENABLE();
		TMR1_InterriptCAPT=fun;
	
	return ret;
}


/************************************************************************/
/*       Implementation Function To Disable CAPT Interrupt              */
/************************************************************************/



Std_ReturnType TMR1_Interrupt_CAPT_Disable()
{
	Std_ReturnType ret=E_OK;
	TMR1_INTERRUPT_CAPT_DISABLE();
	return ret;
}



/************************************************************************/
/*       Implementation Function To Enable OCR1B Interrupt              */
/************************************************************************/



Std_ReturnType TMR1_Interrupt_COMPB_enable(const timer1_t *_timer)
{
	Std_ReturnType ret=E_OK;
	if(NULL == _timer)
	{
		ret = E_NOT_OK;
	}
	else{
		TMR1_INTERRUPT_COMPA_ENABLE();
		TMR1_InterriptComB=_timer->TMR1_InterriptComB;
	}
	return ret;
}



/************************************************************************/
/*       Implementation Function To Disable OCR1B Interrupt             */
/************************************************************************/



Std_ReturnType TMR1_Interrupt_COMPB_Disable()
{
	Std_ReturnType ret=E_OK;
	TMR1_INTERRUPT_COMPB_DISABLE();
	return ret;
}



/************************************************************************/
/*         Implementation Function To Set Value To OCR1A REG            */
/************************************************************************/



Std_ReturnType TIMER1_voidSetOCR1A(uint16 Copy_u16Value){
	Std_ReturnType ret=E_OK;
	OCR1A = Copy_u16Value;
	return ret;
}



/************************************************************************/
/*         Implementation Function To Set Value To OCR1B REG            */
/************************************************************************/


Std_ReturnType TIMER1_voidSetOCR1B(uint16 Copy_u16Value){
	Std_ReturnType ret=E_OK;
	OCR1B = Copy_u16Value;
	return ret;
}


/************************************************************************/
/*         Implementation Function To Set Value To ICR1 REG             */
/************************************************************************/


Std_ReturnType TIMER1_voidSetICR1(uint16 Copy_u16Value){
	Std_ReturnType ret=E_OK;
	ICR1 = Copy_u16Value;
	return ret;
}


/************************************************************************/
/*         Implementation Function To Set Value To TCNT1 REG            */
/************************************************************************/


Std_ReturnType TIMER1_voidSetTCNT1(uint16 Copy_u16Value){
	Std_ReturnType ret=E_OK;
	TCNT1 = Copy_u16Value;
	return ret;
}



/************************************************************************/
/*         Implementation Function To Set Frequency KHz Of PWM          */
/************************************************************************/


Std_ReturnType PWM_FreQ_KHz(uint16 _ferq)
{

	
	Std_ReturnType ret = E_OK;
	if(_ferq == 0 )
	{
		ret = E_NOT_OK;
	}
	else
	{
		uint16 ttotal = (uint32)1000/_ferq;
		if(ttotal >= 1)
		{
			ICR1 = ttotal-1;
		}
		else{ICR1 = 0 ;}
	}
	return ret;
}

/************************************************************************/
/*         Implementation Function To Set Frequency Hz Of PWM           */
/************************************************************************/


Std_ReturnType PWM_FreQ_Hz(uint16 _ferq)
{

	
	Std_ReturnType ret = E_OK;
	if(_ferq == 0 )
	{
		ret = E_NOT_OK;
	}
	else
	{
		uint16 ttotal =(uint32) 1000000/_ferq;
		if(ttotal >= 1)
		{
			ICR1 = ttotal-1;
		}
		else{ICR1 = 0 ;}
	}
	return ret;
}

/************************************************************************/
/*         Implementation Function To Set Duty Cycle                    */
/************************************************************************/


Std_ReturnType PWM_SetDuty_Cycle(uint8 _duty)
{
	
	
	Std_ReturnType ret = E_OK;
	if(_duty >= 100 )
	{
		ret = E_NOT_OK;
	}
	else
	{
		uint16 ton =((uint32)_duty*(ICR1+1))/100;
		if(ton >= 1)
		{
			OCR1A = ton - 1;
		}
		else {OCR1A = 0 ;}
	}
	return ret;
}



/************************************************************************/
/*               Function To Set Edge For ICU                           */
/************************************************************************/



Std_ReturnType TMR1_Set_CaptureEdge(uint8 _edge)
{
	Std_ReturnType ret =E_OK;


			switch(_edge)
			{
				case ICU_RISING_EDGE:
					SET_BIT(TCCR1B,ICES1);
					break;
				case ICU_FALLING_EDGE:
					CLEAR_BIT(TCCR1B,ICES1);
					break;
			}
		
	

	return ret;
}




/******************************************************************************
* Description 	: TIMER1 PWM1 Read Function.		                          *
******************************************************************************/



Std_ReturnType PWM1_voidRead(uint32 *ReturnFreq, uint8 *ReturnDuty){
	Std_ReturnType ret =E_OK;
	

	TCNT1 = 0;


	TMR1_Set_CaptureEdge(ICU_RISING_EDGE);
	TMR1_Interrupt_CAPT_enable(Timer1ICU_InterruptAction);

	
    Global_u16Flag = 0;
	while(Global_u16Flag < 3);
	
	Local_u16TimeON = Global_u16Time2 - Global_u16Time1;

	Local_u16TimeOFF = Global_u16Time3 - Global_u16Time2;

	*ReturnDuty = ((uint32) Local_u16TimeON * 100) / ((uint32)Local_u16TimeON + Local_u16TimeOFF);

	*ReturnFreq = ((uint32)1000000) / ((uint32)Local_u16TimeON + Local_u16TimeOFF);
	
	return ret;
}



/******************************************************************************
* Description 	: TIMER1 ICU Interrupt Action Private Function.				  *
******************************************************************************/



static void Timer1ICU_InterruptAction(void){

	if(Global_u16Flag == 0){

		Global_u16Time1 = ICR1;
		TMR1_Set_CaptureEdge(ICU_FALLING_EDGE);
		Global_u16Flag = 1;

	}
	else if(Global_u16Flag == 1){

		Global_u16Time2 = ICR1;
		TMR1_Set_CaptureEdge(ICU_RISING_EDGE);
		Global_u16Flag = 2;

	}
	else if(Global_u16Flag == 2){

		Global_u16Time3 = ICR1;
		TMR1_Interrupt_CAPT_Disable();
		Global_u16Flag = 3;

	}
	
}



/************************************************************************/
/*                          ISR Functions                               */
/************************************************************************/

/***********************ISR For TMR1 Over Flow***************************/
ISR(TIMER1_OVF_vect)
{
	if(TMR1_InterriptOveFlow)
		TMR1_InterriptOveFlow();
}
/***********************ISR For TMR1 For COMPA***************************/

ISR(TIMER1_COMPA_vect)
{
	if(TMR1_InterriptComA)
		TMR1_InterriptComA();
}
/***********************ISR For TMR1 For COMPB***************************/

ISR(TIMER1_COMPB_vect)
{
	if(TMR1_InterriptComB)
		TMR1_InterriptComB();
}
/***********************ISR For TMR1 For ICU*****************************/

ISR(TIMER1_CAPT_vect)
{
	if(TMR1_InterriptCAPT)
		TMR1_InterriptCAPT();
}

