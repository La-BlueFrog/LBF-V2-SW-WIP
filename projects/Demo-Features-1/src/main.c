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



/* --  Private Functions ----------------------------------------  */

static void   VL6180X_Tuning_Settings(void);

void 	SplashScreen(void);
void 	Carroussel_Demo(void);
void 	Tilt_Demo(void);
void 	DistEval_Demo(void);


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
    Success &= LBF_LaunchUSB_MassStorage();
    Delay_ms(1000);

    /* ... To initialize FatFS                                      */
    /*     and mount the Data Flash as FAT File System              */
    Success &= LBF_FatFS_Init();

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

uint8_t	DemoStep = 0;

/* ==  Body     ======================================== */


// ===  User Initializations  ===  


    OLED_Switch_ON();
    // Provide power (13V) to OLED panel, enable display


    // -- Accelero Inits ----------------------------

    //Check Accelerometer LSM6DS3 can be accessed
    Success =  ( I2C2_ReadSingleReg(LSM6DS3_CHIPID, LSM6DS3_WHOAMI) == LSM6DS3_WHOAMI_CONTENTS);
    Led_StopNBlinkOnFalse (Success);

    // Start-up accelerometer
    I2C2_WriteSingleReg(LSM6DS3_CHIPID, 0x18, 0x38);  // Reg. CTRL9_XL: enable XYZ axes
    I2C2_WriteSingleReg(LSM6DS3_CHIPID, 0x10, 0x60);  // Reg. CTRL1_XL: 416Hz (high-perf)
    I2C2_WriteSingleReg(LSM6DS3_CHIPID, 0x0D, 0x01);  // Reg. INT1_CTRL: data ready on INT1


    // -- VL6180X Inits ----------------------------

    //Check VL6180X can be accessed
    Success = ( I2C2_ReadSingleReg(VL6180X_CHIPID, VL6180X_WHOAMI) == VL6180X_WHOAMI_CONTENTS);
    Led_StopNBlinkOnFalse ( Success );

    // Make sure VL6180X is out of reset before continuing
    while ( I2C2_ReadSingleReg(VL6180X_CHIPID, 0x0016) != 0x01);

    // Clear out-of-reset bit
    I2C2_WriteSingleReg (VL6180X_CHIPID, 0x0016, 0x00);

    //Apply Tuning Settings
    VL6180X_Tuning_Settings();


// ===  Main Loop  ===  


while (1)
{

    switch( DemoStep)    
    { 
        case 0 :	SplashScreen();
        break;

        case 1 :	Carroussel_Demo();
        break;

        case 2 :	DistEval_Demo();
        break;

        default :	SplashScreen();
 
    }

    if(DemoStep++ == 2)  DemoStep = 0;
}

}  // end of main()



/****  PRIVATE FUNCTIONS  of main()      **************************/

 

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


/**************************************END OF FILE****/// 
