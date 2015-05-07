/*******************************************************************************
 * Led_Based_Debug.c
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


/* Privates prototypes -------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/


/* =============================================================== */

void Led_BlinkOnFalse_FixedOnTrue( boolean_t Status )
{
  while(1)
  {
    Stm32_Led_ON();
    Delay_ms(100);
    if (Status != TRUE)
    {
	Stm32_Led_OFF();
    }
    Delay_ms(300);
  }
}

/* =============================================================== */

void Led_StopNBlinkOnFalse( boolean_t Status )
{
  if (Status == FALSE)
  {
    while(1)
    {
      Stm32_Led_ON();
      Delay_ms(100);
      Stm32_Led_OFF();
      Delay_ms(300);
    }
  }
}





/***************************************************************END OF FILE****/
