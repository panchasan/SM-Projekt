/*
 * heater.c
 *
 *  Created on: Jan 18, 2021
 *      Author: MacBook (Ziober)
 */
#include "Components/heater.h"


void HEATER_Init(HEATER_HandleTypeDef* hheater){

	HAL_TIM_PWM_Start(hheater->Timer, hheater->Channel);
}
void HEATER_SetDuty(HEATER_HandleTypeDef* hheater, float duty)
{
  hheater->Duty = duty;
  int COMPARE = (duty * (__HAL_TIM_GET_AUTORELOAD(hheater->Timer)+1)) / 100;

  /*Ustawienie nowej wartości wypełnienia*/
  __HAL_TIM_SET_COMPARE(hheater->Timer, hheater->Channel, COMPARE);
}
