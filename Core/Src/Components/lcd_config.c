/*
 * lcd_config.c
 *
 *  Created on: Jan 18, 2021
 *      Author: MacBook (Ziober)
 */

/********************Include********************/
#include "Components/lcd.h"
#include "Components/lcd_config.h"

#include "main.h"
#include "i2c.h"


/********************Definicja struktury LCD*******************/

LCD_HandleTypeDef hlcd ={

		.I2C= &hi2c1,

		.SCL_Port=LCD_SCL_GPIO_Port,
		.SCL_Pin=LCD_SCL_Pin,
		.SDA_Port=LCD_SDA_GPIO_Port,
		.SDA_Pin=LCD_SDA_Pin,

		//Parametry
		.LCD_ADDR=(0x27 << 1),//Dymyślny bo nie ma żadnych zworek
		.BL=BL_PIN
};
