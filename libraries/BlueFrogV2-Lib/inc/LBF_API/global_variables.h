/*******************************************************************************
 * global_variables.h
 * 
 * Authorv La BlueFrog, 2015
 *
 * This program is free software; you can redistribute it and/or modify
 * t under the terms of the GNU General Public License as published by
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
 ******************************************************************************/




/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GLOBAL_VARIABLES_H
#define __GLOBAL_VARIABLES_H


/**** Declarations of Global Variables accessed from different code modules  **/


/*   ------- Structure used by HAL API Functions   ------------- */

extern UART_HandleTypeDef huart1;   // initialized in LBF_UART1_Init.c
extern USART_HandleTypeDef husart2; // initialized in LBF_USART2_Init.c
extern UART_HandleTypeDef huart3; // initialized in LBF_USART3_Init.c
extern I2C_HandleTypeDef hi2c1;     // initialized in LBF_I2C1_Init.c
extern I2C_HandleTypeDef hi2c2;     // initialized in LBF_I2C2_Init.c
extern SPI_HandleTypeDef hspi1;     // initialized in LBF_SPI1_Init.c
extern SPI_HandleTypeDef hspi3;     // initialized in LBF_SPI3_Init.c
extern TIM_HandleTypeDef htim4;     // initialized in LBF_PWM4_Ch3-4_Init.c



#endif  /*__GLOBAL_VARIABLES_H*/

/***************************************************************END OF FILE****/
