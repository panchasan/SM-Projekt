/*
 * lcd.h
 *
 *  Created on: Jan 18, 2021
 *      Author: MacBook (Ziober)
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

/* Includes */
#include "stm32f7xx_hal.h"

/* Typedef*/

#define LCD_PortType GPIO_TypeDef*
#define LCD_PinType uint16_t
#define LCD_I2C I2C_HandleTypeDef*

#define LENGTH_OF_LINE 17

/* Zdefiniowanie struktury wyświetlacza LCD */

typedef struct{
	LCD_I2C I2C;
	//Porty
	LCD_PortType SCL_Port;
	LCD_PinType SCL_Pin;
	LCD_PortType SDA_Port;
	LCD_PinType SDA_Pin;
	// Moje parametry LCD
	uint8_t LCD_ADDR; // adres ekspanera
	uint8_t BL;          // Podświetlenie

	char FirstLine[LENGTH_OF_LINE];
	char SecendLine[LENGTH_OF_LINE];


}LCD_HandleTypeDef;

/********************Definicja Pinów********************/

/*
 *  			PCF8574 <-> HD44780
 *
 * I2C I/O   P7 P6 P5 P4 P3 P2 P1 P0
 * LCD       D7 D6 D5 D4 BL EN RW RS
 *	                      8  4  2  1
 *
 * Opis pinów "mniej znaczących"
 *
 * RS - pin przełaczający miedzy komędną funkcjyną i zapisem do rejestru
 * RW - 0->zapis 1->odczyt
 * EN - zastrzaskuje bity D4-D7
 * BL - on/off podświetlenie wyświetlacza
 */
/*Definicja pinów*/
#define RS_PIN 0x01
#define RW_PIN 0x02
#define EN_PIN 0x04
#define BL_PIN 0x08

/********************Komendy********************/
/* Tryby pracy */
#define INIT_8_BIT_MODE	0x30 // 8 - bitowy
#define INIT_4_BIT_MODE	0x02 // 4 - bitowy

/* Czyszczenie LCD */
#define CLEAR_LCD	0x01

/*Kursor*/
#define UNDERLINE_OFF_BLINK_OFF		0x0C
#define UNDERLINE_OFF_BLINK_ON		0x0D
#define UNDERLINE_ON_BLINK_OFF		0x0E
#define UNDERLINE_ON_BLINK_ON		0x0F

/*Adresy 1 i 2 lini */

#define FIRST_CHAR_LINE_1	0x80
#define FIRST_CHAR_LINE_2	0xC0


/********************Definicje funkcji********************/
void LCD_Init(LCD_HandleTypeDef *hlcd);
void LCD_Write(LCD_HandleTypeDef *hlcd, uint8_t addr, uint8_t data, uint8_t xpin);
void LCD_Display(LCD_HandleTypeDef *hlcd);
void LCD_Clear(LCD_HandleTypeDef *hlcd);


/********************Kody błedów********************/

#endif /* INC_LCD_H_ */
