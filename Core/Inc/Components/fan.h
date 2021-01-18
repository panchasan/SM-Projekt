/*
 * fan.h
 *
 *  Created on: Jan 18, 2021
 *      Author: MacBook (Ziober)
 */

#ifndef INC_COMPONENTS_FAN_H_
#define INC_COMPONENTS_FAN_H_

/*Includes*/
#include "stm32f7xx_hal.h"

/*Typedef*/

#define FAN_TimerType   TIM_HandleTypeDef*
#define FAN_ChannelType uint16_t

/********************Definicja struktury grzałki********************/
typedef struct{
	  FAN_TimerType Timer;
	  FAN_ChannelType Channel;
	  float Duty; //Wypełnienie w %
}FAN_HandleTypeDef;


/********************Definicja funkcji********************/
void FAN_Init(FAN_HandleTypeDef* hfan);
void FAN_SetDuty(FAN_HandleTypeDef* hfan, float duty);

#endif /* INC_COMPONENTS_FAN_H_ */
