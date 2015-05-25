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
    // Delay_ms(1000);
    // Success &= LBF_LaunchUSB_MassStorage();
 
    /* ... To initialize FatFS                                      */
    /*     and mount the Data Flash as FAT File System              */
    // Success &= LBF_FatFS_Init();

    /* ... To initialize the STemWin Graphical Library              */ 
    /*     Caution: reserves some RAM - keep some for stack/heap    */
    Success &= LBF_emWin_Init();

    // ERROR HANDLER
    /* Replace by your own as wished */
    Led_StopNBlinkOnFalse (Success);
    	// stops here if one of above inits has failed

    Stm32_Led_OFF();




/* ===================================================== */
/* Application Code Below */
/* ===================================================== */


/* ==  User Declarations =============================== */

const uint16_t  Period = 200; //200ms pwm period
uint16_t	Pulse_Selection[4] = {20, 40, 80, 160};  // 0.2, 0.4, 0.8 or 1.6ms high pulse
uint8_t		PulseIndex = 0;


/* ==  Body     ======================================== */


    Stm32_Led_ON();
    

    // *** Timer-based interrupt generation demo section ************

    // Timers 2 and 3 will be used as Time Base Units 
    LBF_Timer_Setup( TIMER2, TIMER_UNIT_US, 30000);  //Configure Timer2 for 1us prescaled clock, 30ms period
    LBF_Timer_Setup( TIMER3, TIMER_UNIT_MS, 500);  //Configure Timer3 for 1ms prescaled clock, 0.5s period

   // Start timer2 to fire IT when elapsed (so, every 30ms)
   // Then in IT handler (cf stm32_it.c), LED gets toggled
   // => Should see LED blinking fast
   // Switch off after 2s
    LBF_Timer_Start_ITout( TIMER2 );  //Start Timer2 with IT generation
    Delay_ms(2000);
    LBF_Timer_Stop( TIMER2 );  //Now stop Timer2 

    // Wait a bit....
    Stm32_Led_OFF();
    Delay_ms(1000);  

   // Start timer3 to fire IT when elapsed (so, every 0.5s)
   // Then in IT handler (cf stm32_it.c), LED gets toggled
   // => Should see LED blinking slowly
   // Switch off after 4s
    LBF_Timer_Start_ITout( TIMER3 );  //Start Timer2 with IT generation
    Delay_ms(4000);
    LBF_Timer_Stop( TIMER3 );  //Now stop Timer2 

    // Wait a bit....
    Stm32_Led_OFF();
    Delay_ms(1000);  



    // *** PWM demo section  (PWM on Timer4, Channel 4)  *******************

    // Set-up Timer 4 and specify period 
    LBF_Timer_Setup( TIMER4, TIMER_UNIT_MS, Period );  


    // Set-up Channel 4 of Timer 4 and specify high pulse duration (duty cycle = pulse / period)
    LBF_PWMchannel_Setup( TIMER4, CHANNEL4, Pulse_Selection[PulseIndex]);   

    // Launch PWM
    LBF_PWMChannel_Start( TIMER4, CHANNEL4 );  
    // available on Ext. Conn; Position 10 - cf User_Configuration.h


    while(1)
    {	
        // Now we will get LED to follow state of PWM 
        // by checking value present on position 10 of Connector 
        // (this requires the #define POS10_IS_PWM_TIM4_CH4  in User_Configuration.h )
        if ( IS_GPIO_SET( CONN_POS10_PORT, CONN_POS10_PIN ) )
        {
           Stm32_Led_ON();
        }
        else
        {
            Stm32_Led_OFF();
        }

        // ...and we will increase or decrease the duty cycle when user presses side-push buttons 
        if (State_Switch1_IsOn() )
        {   
            while (State_Switch1_IsOn()); // wait for release
            if ( PulseIndex-- == 0)
	       PulseIndex = 3;
        }
        if (State_Switch2_IsOn() )
        {
            while (State_Switch2_IsOn()); // wait for release
           if ( ++PulseIndex == 4)
	       PulseIndex = 0;
        }
        LBF_PWMChannel_UpdatePulse (TIMER4, CHANNEL4, Pulse_Selection[PulseIndex]);

    }  // end while(1)


return 0;
}




/***************************************************************END OF FILE****/
