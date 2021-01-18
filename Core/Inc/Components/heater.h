/*
 * heater.h
 *
 *  Created on: Jan 18, 2021
 *      Author: MacBook (Ziober)
 */

#ifndef INC_COMPONENTS_HEATER_H_
#define INC_COMPONENTS_HEATER_H_

/*Includes*/
#include "stm32f7xx_hal.h"

/*Typedef*/

#define HEATER_TimerType   TIM_HandleTypeDef*
#define HEATER_ChannelType uint16_t

/********************Definicja struktury grzałki********************/
typedef struct{
	  HEATER_TimerType Timer;
	  HEATER_ChannelType Channel;
	  float Duty; //Wypełnienie w %
}HEATER_HandleTypeDef;


/********************Definicja funkcji********************/
void HEATER_Init(HEATER_HandleTypeDef* hheater);
void HEATER_SetDuty(HEATER_HandleTypeDef* hheater, float duty);

#endif /* INC_COMPONENTS_HEATER_H_ */
