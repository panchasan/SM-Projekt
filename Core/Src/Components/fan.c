/*
 * fan.c
 *
 *  Created on: Jan 18, 2021
 *      Author: MacBook (Ziober)
 */

#include "Components/fan.h"


void FAN_Init(FAN_HandleTypeDef* hfan){

	HAL_TIM_PWM_Start(hfan->Timer, hfan->Channel);
}
void FAN_SetDuty(FAN_HandleTypeDef* hfan, float duty)
{
  hfan->Duty = duty;
  int COMPARE = (duty * (__HAL_TIM_GET_AUTORELOAD(hfan->Timer)+1)) / 100;

  /*Ustawienie nowej wartości wypełnienia*/
  __HAL_TIM_SET_COMPARE(hfan->Timer, hfan->Channel, COMPARE);
}
