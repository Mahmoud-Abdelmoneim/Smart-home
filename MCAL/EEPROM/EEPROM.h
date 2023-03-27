/*
 * EEPROM.h
 *
 * Created: 14/08/2022 06:23:26 
 *  Author:Mahmoud Abdelmoniem
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_
#include "avr/io.h"
#include "avr/interrupt.h"
#include "../std_types.h"
#include "../Bit_Config.h"


void EEPROM_Read_Byte(uint8 address,uint8 *_value);


void EEPROM_Write_Byte(uint8 address,uint8 _value);

void EEPROM_Interupt_Enable(void);
void EEPROM_Interupt_Disable(void);
void EEPROM_Set_CallBack(void(*Local_ptr)(void));

#endif /* EEPROM_H_ */