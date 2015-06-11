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


//  PWM for Motor 1/2 (Left/right side of robot) use Timer4 Channels 3 and 4
const uint16_t  PWM_Period_us = 60; 	//17KHz pwm fqcy ==> 60us period
uint16_t	PWM1_Pulse_us ; //defines PWM1 duty cycle = Pulse/Period
uint16_t	PWM2_Pulse_us ; //defines PWM2 duty cycle

int8_t		Speed =0; //-100 to +100 (0=stop, +100=max forward, negative = backwards)
int8_t 		Turn = 0;  // -10 to +10 (0= no turn, 10=sharpest, sign defines lfet or right)
boolean_t       Stop = FALSE;
boolean_t	Backwards = FALSE;

// ----  Macros for MC33926 Motor Driver  ---------------------

// (2 motors: 1 for left wheels, 1 for right wheels)

// Global Enable for MC33926 carrier board 
#define MC33926_EN_LOW()           GPIO_LOW(CONN_POS2_PORT, CONN_POS2_PIN)
#define MC33926_EN_HIGH()          GPIO_HIGH(CONN_POS2_PORT, CONN_POS2_PIN)

// Fault Status from MC33926, active low
#define STATUS_FAULT_MC33926()     IS_GPIO_RESET(CONN_POS3_PORT, CONN_POS3_PIN)

// Direction (Common to Motors 1 and 2):  Dir on Pos5 ad nDir on Pos6
#define MC33926_DIR_BWD()  { GPIO_LOW(CONN_POS5_PORT, CONN_POS5_PIN); GPIO_HIGH(CONN_POS6_PORT, CONN_POS6_PIN);}
#define MC33926_DIR_FWD()  { GPIO_HIGH(CONN_POS5_PORT, CONN_POS5_PIN); GPIO_LOW(CONN_POS6_PORT, CONN_POS6_PIN);}



// ----  Private Function    ------------------------------------

void Get_Requirements(int8_t* pSpeed, int8_t* pTurn, boolean_t* pStop );



/* ==  Body     ======================================== */

    Stm32_Led_ON();

    // --- Initialize motors  --------------------------

    // Initial motor state = OFF
    MC33926_EN_LOW();

    // Initialize PWMs: Timer4,  Ch.3 and Ch.4 
    // available on connector posn 9 and 10 (cf User_Configuration.h):

    // 1) Set-up Timer 4 and specify period 
    LBF_Timer_Setup( TIMER4, TIMER_UNIT_US, PWM_Period_us );  
    // 2) Set-up Channel 3 and 4, use initial value 75% duty cycle)
    PWM1_Pulse_us = (PWM_Period_us*3)/4;
    PWM2_Pulse_us = PWM1_Pulse_us;
    LBF_PWMchannel_Setup( TIMER4, CHANNEL3, PWM1_Pulse_us );   
    LBF_PWMchannel_Setup( TIMER4, CHANNEL4, PWM2_Pulse_us );   
    // 3) Start PWM (robot won't start as Enable still low)
    LBF_PWMChannel_Start( TIMER4, CHANNEL3 );  
    LBF_PWMChannel_Start( TIMER4, CHANNEL4 );  

    // Enable Motor
    MC33926_DIR_FWD();
    MC33926_EN_HIGH();
    Delay_ms(1000);

    while(1)
    {
        Delay_ms(1000);
	Stm32_Led_TOGGLE();

        Get_Requirements( &Speed, &Turn, &Stop );
        // Speed = -128 to +128 (0=stop, +100=max forward, negative = backwards)
        // Turn = -8 to +8 (0= no turn, 8=sharpest, sign defines lfet or right)
        // Stop = FALSE or TRUE


        // --- Decode Requirements ----------------

        if (Speed<0)
        {
	    Speed = -Speed;
            Backwards = TRUE;
        }
        else
        {
            Backwards = FALSE;
        }

        PWM1_Pulse_us = (uint16_t) ((uint32_t)(PWM_Period_us * (uint8_t)Speed)/128);
        PWM2_Pulse_us = PWM1_Pulse_us;
        
        if (Turn<0) // turn left --> reduce speed on Motor1
        {
            PWM1_Pulse_us = PWM1_Pulse_us / (uint8_t)(-Turn);
        }
        else if (Turn>0) // turn right --> reduce speed on Motor2
        {
            PWM2_Pulse_us = PWM2_Pulse_us / (uint8_t)Turn;
        }
        // No change if Turn==0


        // --- Implement Requirements --------------

        if (Stop)
        {
            // Disable motors
    	    MC33926_EN_LOW();
            // Reset motor speed
            PWM1_Pulse_us = 0;
            PWM2_Pulse_us = 0;
        }
        else
        {
	    // Adjust Speed
  	    LBF_PWMChannel_UpdatePulse (TIMER4, CHANNEL3, PWM1_Pulse_us);
            LBF_PWMChannel_UpdatePulse (TIMER4, CHANNEL4, PWM2_Pulse_us);

	    // Set direction
            if (Backwards)
            {
		MC33926_DIR_BWD();
            }
            else
            {
		MC33926_DIR_FWD();
	    }

   	    // Re-Enable Motor in case was formerly disabled
            MC33926_EN_HIGH();
         }

    }  // end of while(1)


return 0;
}

// =====  PRIVATE FUNCTIONS ===================================================


void Get_Requirements(int8_t* pSpeed, int8_t* pTurn, boolean_t* pStop )
{
static uint8_t i =0;
static int8_t dir = 1;
static side = +1;

// TODO:
// GET this from remote BlueFrog (using accelerometer data) through BTLE

*pSpeed= dir*(100-(10*i));
*pTurn = side;
*pStop = FALSE;

if ((++i) == 5)
{
    i=0;
    side = -side;
    dir=-dir;
}
 
}



/***************************************************************END OF FILE****/
