/*
 * EX_interrupt.c
 *
 * Created: 26/07/2022 02:13:39 
 *  Author: Mahmoud Abdelmoniem
 */ 
#include "EX_interrupt.h"
static void (*INT0_fptr) (void) = NULL;
static void (*INT1_fptr) (void) = NULL;
static void (*INT2_fptr) (void) = NULL;

static void EXI_SET_SOURCE(const EXI_interruot_t *_interrupt);
static void EXI_SET_EDGE(const EXI_interruot_t *_interrupt);
Std_ReturnType EXI_interrupt_init(const EXI_interruot_t *_interrupt){
	Std_ReturnType ret=E_OK;
	if(NULL == _interrupt)
	{
		ret = E_NOT_OK;
	}
	else{
			EXI_SET_SOURCE(_interrupt);
			EXI_SET_EDGE(_interrupt);
		}
	
	return ret;
}

Std_ReturnType EXI_CallBack(const EXI_interruot_t *_interrupt){
	Std_ReturnType ret=E_OK;
	if(NULL == _interrupt)
	{
		ret = E_NOT_OK;
	}
	else{
		switch(_interrupt->source){
			case EXI_INT0:
				INT0_fptr = (_interrupt->INT0_fpt);
				break;
			case EXI_INT1:
				INT1_fptr = (_interrupt->INT1_fpt);
				break;
			case EXI_INT2:
				INT2_fptr = (_interrupt->INT2_fpt);
				break;
		}		
		
	}	
	return ret;
}


static void EXI_SET_SOURCE(const EXI_interruot_t *_interrupt)
{
		switch(_interrupt->source){
			case EXI_INT0:
			SET_BIT(GICR,INT0);
			break;
			case EXI_INT1:
			SET_BIT(GICR,INT1);
			break;
			case EXI_INT2:
			SET_BIT(GICR,INT2);
			break;	
		}
}
static void EXI_SET_EDGE(const EXI_interruot_t *_interrupt){
		switch(_interrupt->source){
			case EXI_INT0:
				switch(_interrupt->edge)
				{
					case EXI_INT0_GENERATE_INTERRUPT_AT_LOW_LEVEL:
						EXI_INT0_SET_EDGE(EXI_INT0_GENERATE_INTERRUPT_AT_LOW_LEVEL);
						break;
					
					case EXI_INT0_GENERATE_INTERRUPT_AT_HIGH_LEVEL:
						EXI_INT0_SET_EDGE(EXI_INT0_GENERATE_INTERRUPT_AT_HIGH_LEVEL);
						break;
					case EXI_INT0_GENERATE_INTERRUPT_AT_RISING_EDGE:
						EXI_INT0_SET_EDGE(EXI_INT0_GENERATE_INTERRUPT_AT_RISING_EDGE);
						break;
					case EXI_INT0_GENERATE_INTERRUPT_AT_FALLING_EDGE:
						EXI_INT0_SET_EDGE(EXI_INT0_GENERATE_INTERRUPT_AT_FALLING_EDGE);
						break;																
				}
				break;
			case EXI_INT1:
				switch(_interrupt->edge)
				{
					case EXI_INT1_GENERATE_INTERRUPT_AT_LOW_LEVEL:
						EXI_INT0_SET_EDGE(EXI_INT1_GENERATE_INTERRUPT_AT_LOW_LEVEL);
						break;
					
					case EXI_INT1_GENERATE_INTERRUPT_AT_HIGH_LEVEL:
						EXI_INT0_SET_EDGE(EXI_INT1_GENERATE_INTERRUPT_AT_HIGH_LEVEL);
						break;
					case EXI_INT0_GENERATE_INTERRUPT_AT_RISING_EDGE:
						EXI_INT0_SET_EDGE(EXI_INT1_GENERATE_INTERRUPT_AT_RISING_EDGE);
						break;
					case EXI_INT0_GENERATE_INTERRUPT_AT_FALLING_EDGE:
						EXI_INT0_SET_EDGE(EXI_INT1_GENERATE_INTERRUPT_AT_FALLING_EDGE);
						break;
				}			
				break;
			case EXI_INT2:
				switch(_interrupt->edge)
				{
					case EXI_INT2_GENERATE_INTERRUPT_AT_RISING_EDGE:
						EXI_INT2_SET_EDGE(EXI_INT2_GENERATE_INTERRUPT_AT_RISING_EDGE);
						break;
					case EXI_INT2_GENERATE_INTERRUPT_AT_FALLING_EDGE:
						EXI_INT2_SET_EDGE(EXI_INT2_GENERATE_INTERRUPT_AT_FALLING_EDGE);
						break;
					default	
				}
				break;
		}
}

/************************************************************************/
/*                           ISR                                        */
/************************************************************************/
ISR(INT0_vect)
{
	if(INT0_fptr != NULL)
	{
		INT0_fptr();
	}
}
ISR(INT1_vect)
{
	if(INT1_fptr != NULL)
	{
		INT1_fptr();
	}
}

ISR(INT2_vect)
{
	if(INT2_fptr != NULL)
	{
		INT2_fptr();
	}
}