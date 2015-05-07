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

// REMINDER :
// Usage of pins of extension connector as GPIO, PWM etc
// to be specified in User_Configuration.h

// On EXtension Connector Position 2:
// Global Enable for MC33926 carrier board 
#define MC33926_EN_LOW()           GPIO_LOW(CONN_POS2_PORT, CONN_POS2_PIN)
#define MC33926_EN_HIGH()          GPIO_HIGH(CONN_POS2_PORT, CONN_POS2_PIN)

// On EXtension Connector Position 3:
// Fault Status from MC33926, active low
#define STATUS_FAULT_MC33926()     IS_GPIO_RESET(CONN_POS3_PORT, CONN_POS3_PIN)
// returns a uint8_t result

// On Extension Connector Position 5:
// Direction (Common to Motors 1 and 2)
#define MC33926_DIR_BWD()          GPIO_LOW(CONN_POS5_PORT, CONN_POS5_PIN)
#define MC33926_DIR_FWD()          GPIO_HIGH(CONN_POS5_PORT, CONN_POS5_PIN)
// On Extension Connector Position 6:
// notDirection (Common to Motors 1 and 2)
#define MC33926_nDIR_BWD()         GPIO_HIGH(CONN_POS6_PORT, CONN_POS6_PIN)
#define MC33926_nDIR_FWD()         GPIO_LOW(CONN_POS6_PORT, CONN_POS6_PIN)
// !!!!!!!!  MCU must drive both DIR and nDIR 


// On Extension Connector Position 9:
// Enable Motor 1 - PWM signal (supports up to 20KHz)

// On Extension Connector Position 10:
// Enable Motor 2 - PWM signal (supports up to 20KHz)



/* ==  Body     ======================================== */


Stm32_Led_ON();

Delay_ms(2500);

// Set Direction = Forward
MC33926_DIR_FWD();
MC33926_nDIR_FWD();

// Enable Motor Carrier
MC33926_EN_LOW();
Delay_ms(1000);
MC33926_EN_HIGH();


// Turn immedialetely off if Fault, 
   if ( STATUS_FAULT_MC33926() ) 
      {
         MC33926_EN_LOW();
	 Led_StopNBlinkOnFalse( FALSE );  //  
      }


LBF_PWM_Start( PWM4_CH3 );
LBF_PWM_Start( PWM4_CH4 );


LBF_PWM_SetPeriod_us( PWM4, 60);   // 60us period PWM = 17KHz
LBF_PWM_SetPulse_us (PWM4_CH3, 30);   // 30/60 = 50% duty cycle 
LBF_PWM_SetPulse_us (PWM4_CH4, 30);   // 30/60 = 50% duty cycle 

while(1)
{

  // Turn immedialetely off if Fault, 
   if ( STATUS_FAULT_MC33926() ) 
      {
         MC33926_EN_LOW();
	 Led_StopNBlinkOnFalse( FALSE );  //  
      }

  // Run for a while and Stop Motor
    Delay_ms( 2000 );  // run for a while and stop 
    LBF_PWM_SetPulse_us (PWM4_CH3, 0);   
    LBF_PWM_SetPulse_us (PWM4_CH4, 0);  

    Stm32_Led_ON();

    Delay_ms( 2000 );  // wait for a while and restart 
    LBF_PWM_SetPulse_us (PWM4_CH3, 30);   // 30/60 = 50% duty cycle 
    LBF_PWM_SetPulse_us (PWM4_CH4, 30);   // 30/60 = 50% duty cycle 

    Stm32_Led_OFF();

}
    

return 0;
}



/***************************************************************END OF FILE****/
