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
    //Delay_ms(1000);
    //Success &= LBF_LaunchUSB_MassStorage();  

    /* ... To initialize FatFS                                      */
    /*     and mount the Data Flash as FAT File System              */
    Success &= LBF_FatFS_Init();

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

// NOTE: Here, as Data Flash may not be formatted yet,
//  we don't use LBF_LaunchUSB_MassStorage in the initalization phase above


/* ==  User Declarations =============================== */

    FIL MyFile;    /* FatFS File object */
    UINT bw;       /* Number of bytes written in file */


/* ==  Body     ======================================== */

     Stm32_Led_ON();
    // Launch reformat only when any Push Button pressed
    while ( !State_Switch1_IsOn() && !State_Switch2_IsOn() );

    Stm32_Led_OFF();

    /* Create FAT volume with default cluster size */
    Success &= ( f_mkfs("", 0, 0) == FR_OK); 

    /* Create a file as new */
    Success &= ( f_open(&MyFile, "FS_Formt.log", FA_CREATE_NEW | FA_WRITE) == FR_OK);

    /* Write a message */
    Success &= ( f_write(&MyFile, "Formatted by FatFS\r\n", 20, &bw)  == FR_OK);

    /* Close the file */
    f_close(&MyFile);
 
    // Launch USB
    Success &= LBF_LaunchUSB_MassStorage();  

    /* Error handler */
    Led_StopNBlinkOnFalse (Success);


  // Done !
  Stm32_Led_ON();
  while (1);

    
}


/***  END OF FILE ************************************************************/
