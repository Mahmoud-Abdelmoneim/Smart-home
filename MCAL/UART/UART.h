

#ifndef UART_H_
#define UART_H_


#include "avr/io.h"
#include "avr/interrupt.h"
#include "UART_Lcfg.h"
#include "../Bit_Config.h"
#include "../std_types.h"




void UART_Init(void);

void UART_SendByte(uint8 data);
uint8 UART_ReceiveByte(void);
uint8 UART_Receive_NoBlock(uint8*pdata);





void UART_RX_InterruptEnable(void);
void UART_RX_InterruptDisable(void);

void UART_TX_InterruptEnable(void);
void UART_TX_InterruptDisable(void);

void UART_RX_SetCallBack(void (*LocalFptr)(void));
void UART_TX_SetCallBack(void (*LocalFptr)(void));





void UART_SendByteNoBlock(uint8 data);
uint8 UART_ReceiveByteNoBlock(void);




#endif /* UART_H_ */
