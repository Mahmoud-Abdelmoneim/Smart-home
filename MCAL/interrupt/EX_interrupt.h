/*
 * EX_interrupt.h
 *
 * Created: 26/07/2022 02:13:55 م
 *  Author: Mahmoud Abdelmoniem
 */ 


#ifndef EX_INTERRUPT_H_
#define EX_INTERRUPT_H_
#include "avr/io.h"
#include "avr/interrupt.h"
#include "../Bit_Config.h"
#include "../std_types.h"




#define EXI_INT0_GENERATE_INTERRUPT_AT_LOW_LEVEL      0x00
#define EXI_INT0_GENERATE_INTERRUPT_AT_HIGH_LEVEL     0x01
#define EXI_INT0_GENERATE_INTERRUPT_AT_FALLING_EDGE    0x02
#define EXI_INT0_GENERATE_INTERRUPT_AT_RISING_EDGE   0x03

#define EXI_INT1_GENERATE_INTERRUPT_AT_LOW_LEVEL      0x00
#define EXI_INT1_GENERATE_INTERRUPT_AT_HIGH_LEVEL     0x01
#define EXI_INT1_GENERATE_INTERRUPT_AT_FALLING_EDGE    0x02
#define EXI_INT1_GENERATE_INTERRUPT_AT_RISING_EDGE   0x03

#define EXI_INT2_GENERATE_INTERRUPT_AT_RISING_EDGE    0x01
#define EXI_INT2_GENERATE_INTERRUPT_AT_FALLING_EDGE   0x00

#define EXI_INT0_SET_EDGE(_EDGE)                   (MCUCR=MCUCR | _EDGE)
#define EXI_INT1_SET_EDGE(_EDGE)                   (MCUCR=(MCUCR | (_EDGE<<2)))
#define EXI_INT2_SET_EDGE(_EDGE)                   (MCUCSR= MCUCSR | (_EDGE<<6))




typedef enum{
	EXI_INT0,
	EXI_INT1,
	EXI_INT2
	}EXI_source;

typedef struct  
{
	EXI_source source;
	uint8 edge;
	void (*INT0_fpt) (void);
	void (*INT1_fpt) (void);
	void (*INT2_fpt) (void);
	
}EXI_interruot_t;

Std_ReturnType EXI_interrupt_init(const EXI_interruot_t *_interrupt);
Std_ReturnType EXI_CallBack(const EXI_interruot_t *_interrupt);

#endif /* EX_INTERRUPT_H_ */