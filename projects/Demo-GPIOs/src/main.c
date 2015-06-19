/*******************************************************************************
 * main.c
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


#include "LBF_Global.h"
#include "User_Configuration.h"



/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int main(void)
{

boolean_t  Success = TRUE;

/* ================================================================ */
/* Board Initializations and Configurations except OLED             */
/* (clocks, I/Os, on-chip peripherals, on-board ICs)                */
/* ================================================================ */

    LBF_Board_Fixed_Inits();
    LBF_Board_Selective_Inits(); 
             // actions driven by User_Configuration.h

   Stm32_Led_ON();

/* ================================================================ */
/* Optional initialization of Middleware libraries :                */
/* USBD drivers, FatFS File System, STemWin GUI                     */
/* ================================================================ */

    // UNCOMMENT AS NEEDED:
    // (refer to INFO.txt for details on actions performed)

    /* ... To use La BlueFrog as USB Mass Storage (Full Speed)      */
    // Success &= LBF_LaunchUSB_MassStorage();
 
    /* ... To initialize FatFS                                      */
    /*     and mount the Data Flash as FAT File System              */
    // Success &= LBF_FatFS_Init();

    /* ... To initialize the STemWin Graphical Library              */ 
    /*     Caution: reserves some RAM - keep some for stack/heap    */
    //Success &= LBF_emWin_Init();

    // ERROR HANDLER
    /* Replace by your own as wished */
    Led_StopNBlinkOnFalse (Success);
    	// stops here if one of above inits has failed

    Stm32_Led_OFF();




/* ===================================================== */
/* Application Code Below */
/* ===================================================== */


/* ==  User Declarations =============================== */

const uint16_t  PWM_Period_ms = 200; //200ms pwm period
const uint16_t  PWM_Pulse_ms = 50; //50ms pwm high pulse



/* ==  Body     ======================================== */

// Note: this is making use of macros defined in LBF_GPIO_lowlevAPI.h
// and pin aliased in pin_aliases.h


// Pre-requisite: User Configuration.h supposed to have set 
// - position 2 as GPIO input
// - position 5 as GPIO output
// and
// - position 10 as Timer4 Ch4 output (i view of PWM generation)


   // ===   1st part of demo : Setting and reading GPIOs on ext. connector ===


   // Initialize connector position 5 (GPIO output) to Logic0
   GPIO_LOW(CONN_POS5_PORT,CONN_POS5_PIN);

   do 
   {

	Delay_ms(250);
	GPIO_TOGGLE(CONN_POS5_PORT,CONN_POS5_PIN);

	if (IS_GPIO_SET(CONN_POS2_PORT,CONN_POS2_PIN) )
	{
	    Stm32_Led_ON();
	}
	else
	{
	    Stm32_Led_OFF();
	}

   }   while ( !State_Switch1_IsOn() && !State_Switch2_IsOn() );
   // exit loop when either side-push button is pressed



   // ===   2nd part of demo : PWM   ====================================

    // Initialize PWMs: Timer4  Ch.4 available on connector posn 10 

    // 1) Set-up Timer 4 and specify period 
    LBF_Timer_Setup( TIMER4, TIMER_UNIT_MS, PWM_Period_ms );  

    // 2) Set-up PWM channel 4 of Timer 4
    LBF_PWMchannel_Setup( TIMER4, CHANNEL4, PWM_Pulse_ms );  
 
    // 3) Start PWM 
    LBF_PWMChannel_Start( TIMER4, CHANNEL4 );  


    while(1)
    {	
        // Now we will get LED to follow state of PWM 
        // by checking value present on position 10 of Connector 
        if ( IS_GPIO_SET( CONN_POS10_PORT, CONN_POS10_PIN ) )
        {
           Stm32_Led_ON();
        }
        else
        {
            Stm32_Led_OFF();
        }
    }

return 0;

}



/***************************************************************END OF FILE****/
