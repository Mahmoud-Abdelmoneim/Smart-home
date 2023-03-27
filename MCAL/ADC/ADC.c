/*
 * ADC.c
 *
 * Created: 29/07/2022 10:12:36 
 *  Author: Mahmoud Abdelmoniem
 */ 

/************************************************************************/
/*                    Function Implementation                           */
/************************************************************************/



/************************************************************************/
/*                       Section:Includes                               */
/************************************************************************/
#include "ADC.h"

/**************Pointer to function callback for interrupt****************/
static void (*(ADC_InterruptCallBack))(void);
/************************************************************************/
/* Implementation Function Initialize Analog To Digital Conversion      */
/************************************************************************/
Std_ReturnType ADC_Init(const ADC_t* _adc){
	Std_ReturnType ret=E_OK;
	if(NULL == _adc)
	{
		ret = E_NOT_OK;
	}
	else{
		DDRA=0;
		ADC_SET_VOLTAGE_REV(_adc->revrance);
		ADC_SET_PRESCALER(_adc->prescaler);
		ADC_SET_READ_ADJUST(_adc->read_adjust);
		if(_adc->start_conversion_type == ADC_AUTO_START_ENABLE)
		{
			SET_BIT(ADCSRA,ADATE);
			ADC_SELECT_AUTO_TRIGGER(_adc->auto_trigger);
			
		}
		else if(_adc->start_conversion_type == ADC_AUTO_START_DISABLE)
		{
			CLEAR_BIT(ADCSRA,ADATE);
		}
		CLEAR_BIT(ADCSRA,ADIE);
		SET_BIT(ADCSRA,ADEN);
		SET_BIT(ADCSRA,ADSC);
			
	}
	return ret;
}
/************************************************************************/
/*     Implementation Function Read Analog To Digital Conversion        */
/************************************************************************/
Std_ReturnType ADC_Read(const ADC_t* _adc,uint16 *value_conversion){
	Std_ReturnType ret=E_OK;
	chanlle_slect_t chanle=_adc->channle;
	chanle &= 0x07;
	ADMUX &= 0xE0;
	ADMUX |=(uint8)chanle;
	SET_BIT(ADCSRA,ADSC);	
	while( READ_BIT(ADCSRA,ADSC));

	if(_adc->read_adjust == ADC_READ_RIGHT_ADJUST)
	{
		
		*value_conversion = ADCL;
		*value_conversion += (ADCH << 8); 
	}
	else if(_adc->read_adjust == ADC_READ_LEFT_ADJUST)
	{
		*value_conversion = (ADCL>>6);
		*value_conversion += (ADCH<<2);
	}
	else
	{
				*value_conversion = ADCL;
				*value_conversion += (ADCH << 8);
	}
	*value_conversion = ADC;
	return ret;
}
/***************************************************************************/
/*Implementation Function To Enable Interrupt Analog To Digital Conversion */
/***************************************************************************/
Std_ReturnType ADC_Interript_Enable(const ADC_t* _adc)
{
	Std_ReturnType ret=E_OK;
	if(NULL == _adc)
	{
		ret = E_NOT_OK;
	}
	else
	{
		ADC_INTERRUPT_ENABLE();
		ADC_InterruptCallBack = _adc->ADC_InterruptCallBack;
	}
	return ret;
}

/****************************************************************************/
/*Implementation Function To Disable Interrupt Analog To Digital Conversion */
/****************************************************************************/
Std_ReturnType ADC_Interript_Disable()
{

	Std_ReturnType ret=E_OK;
	ADC_INTERRUPT_DISABLE();
	//ADC_InterruptCallBack==NULL;
	return ret;
}


ISR(ADC_vect)
{
	if(ADC_InterruptCallBack)
		ADC_InterruptCallBack();
}

