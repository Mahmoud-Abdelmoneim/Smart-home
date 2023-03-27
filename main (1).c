/*
 * safe.c
 *
 * Created: 25/07/2022 02:44:33 
 * Author : Mahmoud Abdelmoneim
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
/************************************************************************/
/*                   Macro Declaration                                  */
/************************************************************************/


#define  EEPROM_STATUS_LOCATION 0x06

#define  EEPROM_PASSWORD_LOCATION1 0x00
#define  EEPROM_PASSWORD_LOCATION2 0x01
#define  EEPROM_PASSWORD_LOCATION3 0x02
#define  EEPROM_PASSWORD_LOCATION4 0x03

#define  MAX_TRIES  2
#define  NOTPRESSED 0XFF
Std_ReturnType ret;

/************************************************************************/
/*                       LCD Declaration:                               */
/************************************************************************/
char_4bit_lcd_t lcd1={
	.lcd_rs.port=PORTC_INDEX,
	.lcd_rs.pin=GPIO_PIN0,
	.lcd_rs.pin_direction=GPIO_DIRECTION_OUTPUT,
	.lcd_rs.pin_logic=GPIO_LOW,
	.lcd_en.port=PORTC_INDEX,
	.lcd_en.pin=GPIO_PIN1,
	.lcd_en.pin_direction=GPIO_DIRECTION_OUTPUT,
	.lcd_en.pin_logic=GPIO_LOW,
	.lcd_data[0].port=PORTC_INDEX,
	.lcd_data[0].pin=GPIO_PIN2,
	.lcd_data[0].pin_direction=GPIO_DIRECTION_OUTPUT,
	.lcd_data[0].pin_logic=GPIO_LOW,
	.lcd_data[1].port=PORTC_INDEX,
	.lcd_data[1].pin=GPIO_PIN3,
	.lcd_data[1].pin_direction=GPIO_DIRECTION_OUTPUT,
	.lcd_data[1].pin_logic=GPIO_LOW,
	.lcd_data[2].port=PORTC_INDEX,
	.lcd_data[2].pin=GPIO_PIN4,
	.lcd_data[2].pin_direction=GPIO_DIRECTION_OUTPUT,
	.lcd_data[2].pin_logic=GPIO_LOW,
	.lcd_data[3].port=PORTC_INDEX,
	.lcd_data[3].pin=GPIO_PIN5,
	.lcd_data[3].pin_direction=GPIO_DIRECTION_OUTPUT,
	.lcd_data[3].pin_logic=GPIO_LOW,
	
};

/************************************************************************/
/*                       KeyPad Declaration:                            */
/************************************************************************/

keypad_t keypad={
	.keypad_row_pin[0].port=PORTA_INDEX,
	.keypad_row_pin[0].pin=GPIO_PIN0,
	.keypad_row_pin[0].pin_direction=GPIO_DIRECTION_OUTPUT,
	.keypad_row_pin[0].pin_logic=GPIO_LOW,	
	.keypad_row_pin[1].port=PORTA_INDEX,
	.keypad_row_pin[1].pin=GPIO_PIN1,
	.keypad_row_pin[1].pin_direction=GPIO_DIRECTION_OUTPUT,
	.keypad_row_pin[1].pin_logic=GPIO_LOW,
	.keypad_row_pin[2].port=PORTA_INDEX,
	.keypad_row_pin[2].pin=GPIO_PIN2,
	.keypad_row_pin[2].pin_direction=GPIO_DIRECTION_OUTPUT,
	.keypad_row_pin[2].pin_logic=GPIO_LOW,
	.keypad_row_pin[3].port=PORTA_INDEX,
	.keypad_row_pin[3].pin=GPIO_PIN3,
	.keypad_row_pin[3].pin_direction=GPIO_DIRECTION_OUTPUT,
	.keypad_row_pin[3].pin_logic=GPIO_LOW,
	.keypad_column_pin[0].port=PORTA_INDEX,
	.keypad_column_pin[0].pin=GPIO_PIN4,
	.keypad_column_pin[0].pin_direction=GPIO_DIRECTION_INPUT,
	.keypad_column_pin[0].pin_logic=GPIO_LOW,
	.keypad_column_pin[1].port=PORTA_INDEX,
	.keypad_column_pin[1].pin=GPIO_PIN5,
	.keypad_column_pin[1].pin_direction=GPIO_DIRECTION_INPUT,
	.keypad_column_pin[1].pin_logic=GPIO_LOW,
	.keypad_column_pin[2].port=PORTA_INDEX,
	.keypad_column_pin[2].pin=GPIO_PIN6,
	.keypad_column_pin[2].pin_direction=GPIO_DIRECTION_INPUT,
	.keypad_column_pin[2].pin_logic=GPIO_LOW,
	.keypad_column_pin[3].port=PORTA_INDEX,
	.keypad_column_pin[3].pin=GPIO_PIN7,
	.keypad_column_pin[3].pin_direction=GPIO_DIRECTION_INPUT,
	.keypad_column_pin[3].pin_logic=GPIO_LOW,				
};

void uart_interrupt();

/******************Variable Declaration***************************/

//array to save the password
uint8 arr_check[4]; 
//array to save the PIN number to reset password
uint8 PIN[4];
uint8 keypad_read=0;
uint8 counter=0,x=0;
uint8 keypad_read1=0;
//variables to read the password from EEPROM
uint8 location1=0;
uint8 location2=0;
uint8 location3=0;
uint8 location4=0;
uint8 flag=0,i;

uint8 read_uart=0;
//the number of tries wrong password
uint8 tries=MAX_TRIES;
int main(void)
{

	sei();
	UART_Init();
	UART_RX_InterruptEnable();
		
	UART_RX_SetCallBack(uart_interrupt);
	//initialize keypad
    keypad_intialize(&keypad);
	//initialize LCD
	lcd_4bit_initialize(&lcd1);
	//EEPROM_Write_Byte(EEPROM_STATUS_LOCATION,0xff);
	
	/*read the status value in the EEPROM if the status was 0xff, it is the first time to use the save and the
	  should set a password. and if status was 0x00 the user was set password before */ 
	EEPROM_Read_Byte(EEPROM_STATUS_LOCATION,&x);
	lcd_4bit_send_string(&lcd1,"    welcome to");
	lcd_4bit_send_string_pos(&lcd1,2,5,"smart home");
	_delay_ms(2000);
	lcd_4bit_send_comand(&lcd1,_LCD_CLEAR);
	if(x==NOTPRESSED)
	{
		lcd_4bit_send_string(&lcd1," set password");
		for(i=0;i<=3;i++)
		{
			do 
			{
				keypad_get_value(&keypad,&keypad_read);
			} while (keypad_read == 0);
			lcd_4bit_send_char_data(&lcd1,keypad_read);
			_delay_ms(500);
			lcd_4bit_send_char_data_pos(&lcd1,1,14+i,'*');
			_delay_ms(500);
			EEPROM_Write_Byte(EEPROM_PASSWORD_LOCATION1+i,keypad_read);
			keypad_read=0;						
		}
		EEPROM_Write_Byte(EEPROM_STATUS_LOCATION,0x00);
		
	}
	
    while (flag==0) 
    {
		lcd_4bit_send_comand(&lcd1,_LCD_CLEAR);
		lcd_4bit_send_string(&lcd1," check password");
		for(i=0;i<=3;i++)
		{
			do
			{
				keypad_get_value(&keypad,&keypad_read);
			} while (keypad_read == 0);
			arr_check[i]=keypad_read;
			lcd_4bit_send_char_data(&lcd1,keypad_read);
			_delay_ms(500);
			lcd_4bit_send_char_data_pos(&lcd1,1,16+i,'*');
			_delay_ms(500);
			
			keypad_read=0;
		}
		EEPROM_Read_Byte(EEPROM_PASSWORD_LOCATION1,&location1);
		EEPROM_Read_Byte(EEPROM_PASSWORD_LOCATION2,&location2);
		EEPROM_Read_Byte(EEPROM_PASSWORD_LOCATION3,&location3);
		EEPROM_Read_Byte(EEPROM_PASSWORD_LOCATION4,&location4);		
		if(location1 == arr_check[0] && location2 == arr_check[1]
		&& location3 == arr_check[2] && location4 == arr_check[3])
		{
			lcd_4bit_send_comand(&lcd1,_LCD_CLEAR);
			lcd_4bit_send_string(&lcd1,"  correct password");
			_delay_ms(1000);
			while(1){
			lcd_4bit_send_comand(&lcd1,_LCD_CLEAR);
			lcd_4bit_send_string_pos(&lcd1,1,2," 1-Rom1");
			lcd_4bit_send_string_pos(&lcd1,1,11,"2-Rom2");
			lcd_4bit_send_string_pos(&lcd1,2,2,"3-Reception");
			lcd_4bit_send_string_pos(&lcd1,3,2,"4-Temperature");
			keypad_read=0;
			do
			{
				keypad_get_value(&keypad,&keypad_read);
			} while (keypad_read == 0);
			//rom1
			if(keypad_read == '1')
			{
				lcd_4bit_send_comand(&lcd1,_LCD_CLEAR);
				lcd_4bit_send_string_pos(&lcd1,1,2," 1-Lamp1");
				lcd_4bit_send_string_pos(&lcd1,1,10," 2-Lamp2");
				_delay_ms(1000);
				keypad_read=0;
				do
				{
					keypad_get_value(&keypad,&keypad_read);
				} while (keypad_read == 0);
				if(keypad_read == '1')
				{
					
					lcd_4bit_send_comand(&lcd1,_LCD_CLEAR);
				    lcd_4bit_send_string_pos(&lcd1,1,2," 1-Turn ON The L1");
				    lcd_4bit_send_string_pos(&lcd1,2,1," 2-Turn OFF The L1");	
					//lcd_4bit_send_string_pos(&lcd1,3,1," 3-Turn ON The L2");
					//lcd_4bit_send_string_pos(&lcd1,4,1," 4-Turn OFF The L2");	
					_delay_ms(1000);
					keypad_read=0;
					do
					{
						keypad_get_value(&keypad,&keypad_read);
					} while (keypad_read == 0);									
					if(keypad_read == '1')
					{
						//send by uart
						UART_SendByte('1');
					}
					else if (keypad_read == '2')
					{
						//send by uart
						UART_SendByte('2');
					}
					keypad_read=0;				
					
				}
				if(keypad_read == '2')
				{
					lcd_4bit_send_comand(&lcd1,_LCD_CLEAR);
					//lcd_4bit_send_string_pos(&lcd1,1,2," 1-Turn ON The L1");
					//lcd_4bit_send_string_pos(&lcd1,2,1," 2-Turn OFF The L1");
					lcd_4bit_send_string_pos(&lcd1,1,1," 1-Turn ON The L2");
					lcd_4bit_send_string_pos(&lcd1,2,1," 2-Turn OFF The L2");
					_delay_ms(1000);
					keypad_read=0;
					do
					{
						keypad_get_value(&keypad,&keypad_read);
					} while (keypad_read == 0);
					if(keypad_read == '1')
					{
						//send by uart
						UART_SendByte('7');
					}
					else if (keypad_read == '2')
					{
						//send by uart
						UART_SendByte('8');
					}
					keypad_read=0;
					
				}
			}
			//rom2
			if(keypad_read == '2')
			{
				lcd_4bit_send_comand(&lcd1,_LCD_CLEAR);
				lcd_4bit_send_string_pos(&lcd1,1,2," 1-Lamp1");
				lcd_4bit_send_string_pos(&lcd1,1,10," 2-Lamp2");
				_delay_ms(1000);
				keypad_read=0;
				do
				{
					keypad_get_value(&keypad,&keypad_read);
				} while (keypad_read == 0);
				if(keypad_read == '1')
				{
					lcd_4bit_send_comand(&lcd1,_LCD_CLEAR);
					lcd_4bit_send_string_pos(&lcd1,1,2," 1-Turn ON The L1");
					lcd_4bit_send_string_pos(&lcd1,2,1," 2-Turn OFF The L1");
					//lcd_4bit_send_string_pos(&lcd1,3,1," 3-Turn ON The L2");
					//lcd_4bit_send_string_pos(&lcd1,4,1," 4-Turn OFF The L2");
					_delay_ms(1000);	
					keypad_read=0;
					do
					{
						keypad_get_value(&keypad,&keypad_read);
					} while (keypad_read == 0);									
					if(keypad_read == '1')
					{
						//send by uart
						UART_SendByte('3');
					}
					else if (keypad_read == '2')
					{
						//send by uart
						UART_SendByte('4');
					}
					
				}
				if(keypad_read == '2')
				{
					lcd_4bit_send_comand(&lcd1,_LCD_CLEAR);
					//lcd_4bit_send_string_pos(&lcd1,1,2," 1-Turn ON The L1");
					//lcd_4bit_send_string_pos(&lcd1,2,1," 2-Turn OFF The L1");
					lcd_4bit_send_string_pos(&lcd1,1,1," 1-Turn ON The L2");
					lcd_4bit_send_string_pos(&lcd1,2,1," 2-Turn OFF The L2");
					_delay_ms(1000);
					keypad_read=0;
					do
					{
						keypad_get_value(&keypad,&keypad_read);
					} while (keypad_read == 0);
					if(keypad_read == '1')
					{
						//send by uart
						UART_SendByte('9');
					}
					else if (keypad_read == '2')
					{
						//send by uart
						UART_SendByte('a');
					}
					keypad_read=0;
					
				}
			}
            //reception

			if(keypad_read == '3')
			{
				lcd_4bit_send_comand(&lcd1,_LCD_CLEAR);
				lcd_4bit_send_string_pos(&lcd1,1,2," 1-Lamp1");
				lcd_4bit_send_string_pos(&lcd1,1,10," 2-Lamp2");
				lcd_4bit_send_string_pos(&lcd1,2,1," 3-Lamp3");
				_delay_ms(1000);
				keypad_read=0;
				do
				{
					keypad_get_value(&keypad,&keypad_read);
				} while (keypad_read == 0);
				if(keypad_read == '1')
				{
					lcd_4bit_send_comand(&lcd1,_LCD_CLEAR);
					lcd_4bit_send_string_pos(&lcd1,1,2," 1-Turn ON The L1");
					lcd_4bit_send_string_pos(&lcd1,2,1," 2-Turn OFF The L1");
					//lcd_4bit_send_string_pos(&lcd1,3,1," 3-Turn ON The L2");
					//lcd_4bit_send_string_pos(&lcd1,4,1," 4-Turn OFF The L2");
					_delay_ms(1000);
					keypad_read=0;
					do
					{
						keypad_get_value(&keypad,&keypad_read);
					} while (keypad_read == 0);
					if(keypad_read == '1')
					{
						//send by uart
						UART_SendByte('5');
					}
					else if (keypad_read == '2')
					{
						//send by uart
						UART_SendByte('6');
					}
					
				}
				if(keypad_read == '2')
				{
					lcd_4bit_send_comand(&lcd1,_LCD_CLEAR);
					//lcd_4bit_send_string_pos(&lcd1,1,2," 1-Turn ON The L1");
					//lcd_4bit_send_string_pos(&lcd1,2,1," 2-Turn OFF The L1");
					lcd_4bit_send_string_pos(&lcd1,1,1," 1-Turn ON The L2");
					lcd_4bit_send_string_pos(&lcd1,2,1," 2-Turn OFF The L2");
					_delay_ms(1000);
					keypad_read=0;
					do
					{
						keypad_get_value(&keypad,&keypad_read);
					} while (keypad_read == 0);
					if(keypad_read == '1')
					{
						//send by uart
						UART_SendByte('b');
					}
					else if (keypad_read == '2')
					{
						//send by uart
						UART_SendByte('c');
					}
					keypad_read=0;
					
				}
				if(keypad_read == '3')
				{
					lcd_4bit_send_comand(&lcd1,_LCD_CLEAR);
					//lcd_4bit_send_string_pos(&lcd1,1,2," 1-Turn ON The L1");
					//lcd_4bit_send_string_pos(&lcd1,2,1," 2-Turn OFF The L1");
					lcd_4bit_send_string_pos(&lcd1,1,1," 1-Turn ON The L3");
					lcd_4bit_send_string_pos(&lcd1,2,1," 2-Turn OFF The L3");
					_delay_ms(1000);
					keypad_read=0;
					do
					{
						keypad_get_value(&keypad,&keypad_read);
					} while (keypad_read == 0);
					if(keypad_read == '1')
					{
						//send by uart
						UART_SendByte('d');
					}
					else if (keypad_read == '2')
					{
						//send by uart
						UART_SendByte('e');
					}
					keypad_read=0;
					
				}
			}
			if(keypad_read == '4')
			{
				read_uart=0;
				lcd_4bit_send_comand(&lcd1,_LCD_CLEAR);
				UART_SendByte('s');
				_delay_ms(500);
				lcd_4bit_send_string_pos(&lcd1,1,2,"Temperature: ");
				_delay_ms(100);
				lcd_4bit_send_char_data_pos(&lcd1,1,16,read_uart);
				_delay_ms(1000);
				lcd_4bit_send_char_data_pos(&lcd1,1,15,read_uart);
				_delay_ms(5000);
			}						
			_delay_ms(1000);
				}
			
		}
		
		
		else
		{
			tries--;
			if(tries>0)
			{
				lcd_4bit_send_comand(&lcd1,_LCD_CLEAR);
				_delay_ms(100);
				lcd_4bit_send_string_pos(&lcd1,1,2,"password wrong");
				_delay_ms(100);
				lcd_4bit_send_string_pos(&lcd1,2,2,"tries left");
				lcd_4bit_send_char_data_pos(&lcd1,2,15,tries+48);
				_delay_ms(3000);
			}
			else
			{
				lcd_4bit_send_comand(&lcd1,_LCD_CLEAR);
				_delay_ms(100);
				lcd_4bit_send_string_pos(&lcd1,1,2,"password wrong");
				_delay_ms(100);
				lcd_4bit_send_string_pos(&lcd1,2,2,"Attempts are over ");
				
				_delay_ms(3000);
				lcd_4bit_send_comand(&lcd1,_LCD_CLEAR);
				_delay_ms(100);
				lcd_4bit_send_string_pos(&lcd1,1,2,"press 1 to ");
				lcd_4bit_send_string_pos(&lcd1,2,4," set new pass");
				keypad_read=0;
				do
				{
					keypad_get_value(&keypad,&keypad_read);
				} while (keypad_read != '1');
				lcd_4bit_send_comand(&lcd1,_LCD_CLEAR);
				lcd_4bit_send_string_pos(&lcd1,1,2," check PIN ");
				_delay_ms(1000);
				keypad_read1=0;
				for(i=0;i<=3;i++)
				{
					do
					{
						keypad_get_value(&keypad,&keypad_read1);
					} while (keypad_read1 == 0);
					PIN[i]=keypad_read1;
					lcd_4bit_send_char_data(&lcd1,keypad_read1);
					_delay_ms(500);
					lcd_4bit_send_char_data_pos(&lcd1,1,12+i,'*');
					_delay_ms(500);
					keypad_read1=0;
					
				}
				keypad_read1=0;
				if(PIN[0] == '9' && PIN[1] == '9' && PIN[2] == '9' && PIN[3] == '9' )
				{
					lcd_4bit_send_comand(&lcd1,_LCD_CLEAR);
					lcd_4bit_send_string(&lcd1," new password");
					for(i=0;i<=3;i++)
					{
						do
						{
							keypad_get_value(&keypad,&keypad_read1);
						} while (keypad_read1 == 0);
						lcd_4bit_send_char_data(&lcd1,keypad_read1);
						_delay_ms(500);
						lcd_4bit_send_char_data_pos(&lcd1,1,14+i,'*');
						_delay_ms(500);
						EEPROM_Write_Byte(EEPROM_PASSWORD_LOCATION1+i,keypad_read1);
						keypad_read1=0;
						
						
					}
					tries=MAX_TRIES;
				}
				else
				{
					flag=1;
					lcd_4bit_send_comand(&lcd1,_LCD_CLEAR);
					lcd_4bit_send_string_pos(&lcd1,1,1," reset the system");
					for(uint8 j=0;j<5;j++)
					{
						lcd_4bit_send_char_data_pos(&lcd1,1,16+j,'.');
						_delay_ms(600);
					}
					_delay_ms(500);
					
				}
				keypad_read=0;
				_delay_ms(1000);
			}
			
		}
    }
}

void uart_interrupt(void)
{
	
	read_uart = UART_ReceiveByteNoBlock();
}