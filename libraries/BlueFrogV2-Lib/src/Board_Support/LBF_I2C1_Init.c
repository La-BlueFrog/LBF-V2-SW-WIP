/*******************************************************************************
 * LBF_I2C1_Init.c

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

#include "LBF_I2C1_Init.h"


I2C_HandleTypeDef hi2c1;  // global variable used by SPI HAL functions 


/*******************************************************************************
 * @brief  : Initialise et configure le peripherique I2C #2
 * @param  : Aucun.
 * @return : Rien.
 ******************************************************************************/
void LBF_I2C1_Init (uint32_t I2C1_Speed)
{
// Based on Cube MX

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = I2C1_Speed;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED; 
  HAL_I2C_Init(&hi2c1);

}



/***************************************************************END OF FILE****/
