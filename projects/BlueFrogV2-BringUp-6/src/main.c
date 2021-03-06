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


void VL6180X_Tuning_Settings(void);

uint16_t  	Distance_mm;
uint8_t		CoarseRange;
uint16_t 	ColorTable[7] = {0x319F, 0x001F, 0x001C, 0x0017, 0x0013, 0x000F, 0x000C } ;
#define 	ORIG_BAR_X	120
#define 	ORIG_BAR_Y	10
#define 	SEGMENT_HEIGHT	15	
#define 	WIDTH_BAR	30		 
uint8_t		Slice[7] = {0, 0, 0, 0, 0, 0, 0} ;
uint8_t		i;	 


/* ==  Body     ====================================================== */

    OLED_Switch_ON();
    // Provide power (13V) to OLED panel, enable display


   /* ---     Display Scale for Proximity Results    ----------- */

   OLED_Clear();

   GUI_SetColor(0xFFFFFF);  // White
   GUI_SetTextMode(GUI_TM_TRANS);  // write with transparent background

   GUI_SetFont(&GUI_Font16_1);
   GUI_DispStringAt("How Far ?", 4 , 30);  

   GUI_SetFont(&GUI_Font13_1);
   GUI_DispStringAt("20cm --- ", ORIG_BAR_X - 30, 
		ORIG_BAR_Y +4);  
   GUI_DispStringAt("10cm --- ", ORIG_BAR_X - 30, 
		ORIG_BAR_Y+ (3*SEGMENT_HEIGHT) + (SEGMENT_HEIGHT>>2) +4);  
   GUI_DispStringAt(" 0cm --- ",  ORIG_BAR_X - 30, 
		ORIG_BAR_Y+ (6*SEGMENT_HEIGHT) + (SEGMENT_HEIGHT>>2) +4 );  



  /* ---     Initialize VL6180X               ------------------ */

    // Check Access OK to VL6180X ALS + Proximity Sensor 
    Success = ( I2C2_ReadSingleReg(VL6180X_CHIPID, VL6180X_WHOAMI) == VL6180X_WHOAMI_CONTENTS);
    Led_StopNBlinkOnFalse ( Success );

    // Make sure VL6180X is out of reset before continuing
    while ( I2C2_ReadSingleReg(VL6180X_CHIPID, 0x0016) != 0x01);

    // Clear out-of-reset bit
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x0016, 0x00);

    //Apply Tuning Settings
    VL6180X_Tuning_Settings();

    // Set bits [2:0] of Reg.0x14 to 0b100: enable internal IT on sample ready
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x0014, 0x4);

    // Start cont. range measurements: write 0x03 to 0x18
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x0018, 0x3);


    while (1)
    {

        // Wait until bits [2:0] of Reg.0x4F are 0b100 (measurement complete)
        while( (I2C2_ReadSingleReg(VL6180X_CHIPID, 0x004F) & 0x07) != 0x4 );

       // Read distance ("range value") in mm from Reg.0x0062
       Distance_mm = I2C2_ReadSingleReg(VL6180X_CHIPID, 0x0062);

       // Set bits [2:0] of Reg.0x15 to 0b111  (clear all interrupts)
       I2C2_WriteSingleReg (VL6180X_CHIPID, 0x0015, 0x7);

        // Calculate Distance
       if (Distance_mm > 220)
			Distance_mm = 220;
       CoarseRange = Distance_mm / 32;	// 0 to 6

        // Display Bars according to distance

         for (i=0; i<7; i++)
	 {   
		// If Slice was ON and needs to go OFF: redraw in black
		if ( (Slice[i]==1) && (CoarseRange < i) )
		{
              	    OLED_Fill( ORIG_BAR_X, 
				ORIG_BAR_Y + (6-i)*SEGMENT_HEIGHT,		 
				WIDTH_BAR, SEGMENT_HEIGHT, 
				BLACK);
		    Slice[i] = 0;  // slice now OFF
		}
		else   // if slice was OFF and needs to go ON: 
		if  ( (Slice[i]==0) && (CoarseRange >= i) )
		{
                    OLED_Fill( ORIG_BAR_X, 
				ORIG_BAR_Y + (6-i)*SEGMENT_HEIGHT,		 
				WIDTH_BAR, SEGMENT_HEIGHT, 
				ColorTable[i] );
		    Slice[i] = 1;  // slice now ON
		 }
		 // else nothing to do: DON'T redraw (speed up rendering)

	 }  // end For


    }  // end of while(1)

}

/* ------------------------------------------------------------------ */
/* -- Private Functions ---------------------------------------------- */
/* ------------------------------------------------------------------ */


/* ------------------------------------------------------------------ */

void VL6180X_Tuning_Settings(void)
{
/* as per AN DT0037 Rev 1 from ST */
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x0207, 0x01);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x0208, 0x01);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x0133, 0x01); 
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x0096, 0x00);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x0097, 0xFD);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x00e3, 0x00);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x00e4, 0x04);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x00e5, 0x02);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x00e6, 0x01);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x00e7, 0x03);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x00f5, 0x02);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x00D9, 0x05);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x00DB, 0xCE);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x00DC, 0x03);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x00DD, 0xF8);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x009f, 0x00);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x00a3, 0x3C);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x00b7, 0x00);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x00bb, 0x3C);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x00b2, 0x09);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x00ca, 0x09);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x0198, 0x01);

    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x01b0, 0x17);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x01ad, 0x00);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x00FF, 0x05);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x0100, 0x05);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x0199, 0x05);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x0109, 0x07);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x010a, 0x30);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x003f, 0x46);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x01a6, 0x1B);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x01ac, 0x3E);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x01a7, 0x1F);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x0103, 0x01);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x0030, 0x00);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x001b, 0x0A);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x003e, 0x0A);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x0131, 0x04);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x0011, 0x10);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x0014, 0x24);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x0031, 0xFF);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x00d2, 0x01);
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x00f2, 0x01);
}



/***************************************************************END OF FILE****/
