/*******************************************************************************
 * LBF_PWR_IOcfg.c
 * 
 * 
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

#include "LBF_PWR_IOcfg.h"

/*******************************************************************************
 * @brief  : Configure GPIOs dedicated to PMIC - LTC533, VDD_OLED Power Switch
 * @param  : none.
 * @return : none.
 ******************************************************************************/
void LBF_PWR_IOcfg(void)
{

GPIO_InitTypeDef GPIO_InitStruct;


/* --- LTC3533 PMIC ---*/


// Inputs

    // ONOFF_STAT (PC13) Input: is IRQ, see Interrupt Config section
 

// Outputs

    // Pin  BUCK3V_ON (PB6)  -  Output  (NB- ext. pull-up on initial board)
    // Std CMOS output, no pull_up/-down res., very low speed
    GPIO_InitStruct.Pin = BUCK3V_ON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; //Could be OD when pull-up R2 is present
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(BUCK3V_ON_PORT, &GPIO_InitStruct) ;

    // Pin  HPWR (PC2)  -  Output, std CMOS 
    GPIO_InitStruct.Pin = HPWR_PIN;
    HAL_GPIO_Init(HPWR_PORT, &GPIO_InitStruct) ;


/* --- TPS22929 Power Switch */
/* --- (DC-DC Boost Converter On/Off for OLED) */

    // Pin  VDDH_EN (PC0)  -  Output, std CMOS 
    GPIO_InitStruct.Pin = VDDH_EN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(VDDH_EN_PORT, &GPIO_InitStruct) ;


/* ---------------------------------------------- */
/* -- Set BUCK3V_ ON to active state           -- */

    GPIO_HIGH(BUCK3V_ON_PORT, BUCK3V_ON_PIN);


/* ---------------------------------------------- */
/* -- Enable input current up to 500mA         -- */

    GPIO_HIGH(HPWR_PORT, HPWR_PIN);


/* ---------------------------------------------- */
/* -- Make sure OLED VDDH is off at start-up   -- */

    Turn_VDDH_Off();


}



/***************************************************************END OF FILE****/
