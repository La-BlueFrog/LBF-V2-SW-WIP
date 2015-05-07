/*******************************************************************************
 * LBF_PWM4_Ch34_Init.c

 * Author: La BlueFrog, 2015
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 *************************************************************************/

#include "LBF_lowlev_API.h"

#include "LBF_PWM4_Ch34_Init.h"


TIM_HandleTypeDef htim4;  // global variable used by SPI HAL functions 


/*******************************************************************************
 * @brief  : Initialise and configure PWM of a Timer 
 * @param  : Aucun.
 * @return : Rien.
 ******************************************************************************/
void LBF_PWM4_Ch34_Init ( void )
{
// Derived from Cube MX code and STM32L152 project example in Cube Library package

// To set Prescaler and Pulse values:
//  Prescaler = (TIMxCLK / Prescaled clock) - 1
//  ARR Period = (Prescaled clock / Target PWM Fqcy) - 1
//  TIM2 Channely duty cycle = (TIMx_CCRy/ TIMx_ARR + 1)* 100%


  TIM_OC_InitTypeDef sConfigOC; // or NEEDS TO BE GLOBAL ????

// First, confirgure the TIM (timer) peripheral

  htim4.Instance = TIM4;
  htim4.Init.Prescaler = (HAL_RCC_GetPCLK1Freq()/1000000) - 1; // so prescaled clock always 1MHz
    // Caution: TIM4CLK is 1x PCLK2 if APB1 prescaler=1 else is 2x -- see STM32 UserMan
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 0 ;  // Initial Value, Target Value to be set by Application 
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; // ie no division
  HAL_TIM_PWM_Init(&htim4);

// Second, configure the PWM channel

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0 ;  // Initial Value, Target Value to be set by Application
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3);

  sConfigOC.Pulse = 0 ;  // Initial Value, Target Value to be set by Application
  HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4);

}


/***************************************************************END OF FILE****/
