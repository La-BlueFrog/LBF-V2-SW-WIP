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
    // Success &= LBF_emWin_Init();

    // ERROR HANDLER
    /* Replace by your own as wished */
    Led_StopNBlinkOnFalse (Success);
    	// stops here if one of above inits has failed

    Stm32_Led_OFF();



/* ===================================================== */
/* Application Code Below */
/* ===================================================== */


/* ==  User Declarations =============================== */


uint16_t color_table[8] = {BLACK, YELLOW, RED, WHITE, GREEN, BLUE, ORANGE, CYAN} ;
uint32_t counter = 0;



/* ==  Body     ======================================== */



    OLED_Switch_ON();
    // Provide power (13V) to OLED panel, enable display

    OLED_Fill(0, 0, 160, 128, BLUE);      

    while(1)
    {
          if ( State_Switch1_IsOn() )
          {  
             OLED_Fill(0, 0, 160, 128, CYAN);  
          }
          if ( State_Switch2_IsOn() )
          {  
             counter++;

             // Fill the 160x128 screens with 8 colors organized as 2 rows of 4 blocks
             // Arguments: Xstart, Ystart, Width, Height, Color in RGB565 format 
             OLED_Fill(0, 0, 40, 64, color_table[(0+counter)%8] );  
             OLED_Fill(40, 0,40, 64, color_table[(1+counter)%8]);
             OLED_Fill(80, 0, 40, 64, color_table[(2+counter)%8]);
             OLED_Fill(120, 0, 40, 64, color_table[(3+counter)%8]);
             OLED_Fill(0, 64, 40, 64, color_table[(4+counter)%8]);
             OLED_Fill(40, 64, 40, 64, color_table[(5+counter)%8]);
             OLED_Fill(80, 64, 40, 64, color_table[(6+counter)%8]);
             OLED_Fill(120, 64, 40, 64, color_table[(7+counter)%8]);
          }

     }



    return 0;
}
 

/***************************************************************END OF FILE****/
