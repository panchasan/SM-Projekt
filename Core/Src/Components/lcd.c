/*
 * lcd.c
 *
 *  Created on: Jan 18, 2021
 *      Author: MacBook (Ziober)
 */

#include "Components/lcd.h"


/********************Ciała funkcji********************/
void LCD_Init(LCD_HandleTypeDef *hlcd){
	/*Ustawienei podświetlenia */
	if (hlcd->BL) {

	}

	/****Sekwencja inicjalizacyjna dla 4-bitowego trybu (str. 46 dokumentacji)****/
	HAL_Delay(40);
	LCD_Write(hlcd,hlcd->LCD_ADDR,INIT_8_BIT_MODE, hlcd->BL); //Function set
	HAL_Delay(5);
	LCD_Write(hlcd,hlcd->LCD_ADDR,INIT_8_BIT_MODE, hlcd->BL); //Function set
	HAL_Delay(1);
	LCD_Write(hlcd,hlcd->LCD_ADDR,INIT_8_BIT_MODE, hlcd->BL); //Function set

	/*Właczenie 4-bitowego trybu*/
	LCD_Write(hlcd,hlcd->LCD_ADDR,INIT_4_BIT_MODE, hlcd->BL); //Function set

	/*Ustwaniea kursora*/
	LCD_Write(hlcd,hlcd->LCD_ADDR,UNDERLINE_OFF_BLINK_OFF, hlcd->BL); //Function set

	/*Czyszczenei ekranu*/
	LCD_Clear(hlcd);
}

void LCD_Write(LCD_HandleTypeDef *hlcd, uint8_t addr, uint8_t data, uint8_t xpin){
/*
 * Zmienna xpin - zmienna stanu miej znaczących pinów (dodatkowe ustawienia)
 */
	/*Formawanie komunikatu 4 bitowego*/
	uint8_t message[4];

	message[0]=(data & 0xF0)|EN_PIN|xpin; //0xF0 -> 1111000 - zerowanie 4 ostatnich bitów
	message[1] = (data & 0xF0) | xpin;
	message[2] = (data << 4) | EN_PIN | xpin;
	message[3] = (data << 4) | xpin;

	/*Wysłanie za pomocą I2C*/
	HAL_I2C_Master_Transmit(hlcd->I2C, addr, message, 4, 100);
	HAL_Delay(5);//czas przetwarzania (dokumentacja)

}
void LCD_Display(LCD_HandleTypeDef *hlcd){
	/*Zmienne pomocnicze*/
	int i =0;

	/*Czyszczenei ekranu*/
	LCD_Clear(hlcd);
	/*Informacja która linia*/
	LCD_Write(hlcd, hlcd->LCD_ADDR, FIRST_CHAR_LINE_1, hlcd->BL);

	/* RS_PIN by zapisać dane do wyświetlacza */
	while(hlcd->FirstLine[i]) {
		LCD_Write(hlcd, hlcd->LCD_ADDR, hlcd->FirstLine[i], (hlcd->BL|RS_PIN));
		i++;
	}
	i=0;
	/*Informacja która linia*/
	LCD_Write(hlcd, hlcd->LCD_ADDR, FIRST_CHAR_LINE_2, hlcd->BL);
	while(hlcd->SecendLine[i]) {
		LCD_Write(hlcd, hlcd->LCD_ADDR, hlcd->SecendLine[i], (hlcd->BL|RS_PIN));
		i++;
	}
	i=0;
}
void LCD_Clear(LCD_HandleTypeDef *hlcd){
	/*Czyszczenei ekranu - wysłanie komendy*/
	LCD_Write(hlcd, hlcd->LCD_ADDR, CLEAR_LCD, hlcd->BL);
}
