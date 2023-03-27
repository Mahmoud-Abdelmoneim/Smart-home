/* 
 * File:   ecu_keypad.c
 * Author: Mahmoud Abdelmoniem
 *
 * Created on 25 , 2022, 08:16 
 * 
 * 
 */
#include "ecu_keypad.h"


static const uint8 btn_values[ECU_KEYPAD_ROWS][ECU_KEYPAD_COLUMNS] = {
                                                                        {'7', '8', '9', '/'},
                                                                        {'4', '5', '6', '*'},
                                                                        {'1', '2', '3', '-'},
                                                                        {'#', '0', '=', '+'}
                                                                     };

/**
 * @brief Initialize the keypad assigned pins
 * @param _keypad_obj pointer to the keypad configurations
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action
 */

Std_ReturnType keypad_intialize(const keypad_t* _keypad)
{
    Std_ReturnType ret=E_OK;
    uint8 rows_counter=ZERO_INIT,columns_counter=ZERO_INIT;
    if(NULL==_keypad )
        {
            ret=E_NOT_OK;
        }
    
    else 
        {
            for(rows_counter=ZERO_INIT;rows_counter<ECU_KEYPAD_ROWS;rows_counter++)
                {
                    ret = gpio_pin_init(&(_keypad->keypad_row_pin[rows_counter]));
                }
            
            for(columns_counter=ZERO_INIT;columns_counter<ECU_KEYPAD_COLUMNS;columns_counter++)
                {
                    ret = gpio_pin_dirction_init(&(_keypad->keypad_column_pin[columns_counter]));
                }
        }

    return ret;
}
/**
 * @brief Get the value of the button pressed by the user and performing the scanning algorithm
 * @param _keypad_obj pointer to the keypad configurations
 * @param value Value of the button pressed by the user
 * @return Status of the function
 *          (E_OK) : The function done successfully
 *          (E_NOT_OK) : The function has issue to perform this action 
 */
Std_ReturnType keypad_get_value(const keypad_t* _keypad, uint8 * value)
{
    Std_ReturnType ret=E_OK;
    uint8 l_rows_counter=ZERO_INIT,l_columns_counter=ZERO_INIT,rows_counter=ZERO_INIT;
    logic_t column_logic=GPIO_LOW;

    if(NULL==_keypad || value==NULL)
        {
            ret=E_NOT_OK;
        }
    
    else 
        {
            for(l_rows_counter=ZERO_INIT;l_rows_counter<ECU_KEYPAD_ROWS;l_rows_counter++)
                {
                    for(rows_counter=ZERO_INIT;rows_counter<ECU_KEYPAD_ROWS;rows_counter++)
                         {
                            gpio_write_logic(&(_keypad->keypad_row_pin[rows_counter]),GPIO_LOW);

                         }
                    gpio_write_logic(&(_keypad->keypad_row_pin[l_rows_counter]),GPIO_HIGH);
                        
                    for(l_columns_counter=ZERO_INIT;l_columns_counter<ECU_KEYPAD_COLUMNS;l_columns_counter++)
                        {
                           ret = gpio_read_logic(&(_keypad->keypad_column_pin[l_columns_counter]),&column_logic);
                           if(column_logic==GPIO_HIGH)
                           {
                              *value=btn_values[l_rows_counter][l_columns_counter]; 
                           }
                        }
                    
                }
        }

    return ret;


}