/* 
 * File:   ecu_keypad.h
 * Author: Mahmoud Abdelmoniem
 *
 * Created on 25 , 2022, 08:16 
 */

#ifndef ECU_KEYPAD_H
#define	ECU_KEYPAD_H

/*........section: includes......... */

#include "../MCAL/GPIO/GPIO.h"



/*........section: macro declarations......... */
#define ECU_KEYPAD_ROWS        4
#define ECU_KEYPAD_COLUMNS     4

/*........section: data type declarations......... */

typedef struct {
    pin_cofig_t keypad_row_pin[ECU_KEYPAD_ROWS];
    pin_cofig_t keypad_column_pin[ECU_KEYPAD_ROWS];
            
}keypad_t;
/*........section: macro function declarations......... */



/*........section: function declarations......... */

Std_ReturnType keypad_intialize(const keypad_t* _keypad);
Std_ReturnType keypad_get_value(const keypad_t* _keypad,uint8 * value);





#endif	/* ECU_KEYPAD_H */

