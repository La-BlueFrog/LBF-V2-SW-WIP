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

#include "stm32_it.h"  // move to global include ?


/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int main(void)
{

boolean_t  InitSuccess = TRUE;

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
    // InitSuccess &= LBF_LaunchUSB_MassStorage();
 
    /* ... To initialize FatFS                                      */
    /*     and mount the Data Flash as FAT File System              */
    // InitSuccess &= LBF_FatFS_Init();

    /* ... To initialize the STemWin Graphical Library              */ 
    /*     Caution: reserves some RAM - keep some for stack/heap    */
    InitSuccess &= LBF_emWin_Init();

    // ERROR HANDLER
    /* Replace by your own as wished */
    Led_StopNBlinkOnFalse (InitSuccess);
    	// stops here if one of above inits has failed

    Stm32_Led_OFF();



/* ===================================================== */
/* Application Code Below */
/* ===================================================== */


/* ==  User Declarations =============================== */

boolean_t Success;

IMU_6AXES_InitTypeDef  LSM6DS3_InitStruct; 
  // Structure to contain Inertial Motion Unit (IMU, i.e. LSM6DS3 here) 
  // initialization parameters

volatile uint16_t FIFO_DataOut_L, FIFO_DataOut_H;
int16_t		Init_Accel_Vector[3]; // X, Y, Z initial accel vector
int16_t		Accel_Vector[3]; 
int16_t		Accel_X_Raw, Accel_Y_Raw, Accel_Z_Raw;

int32_t		BarHeight;
int32_t		Power, Score;
int32_t		Best_Score = 0;
uint16_t	i;


/* ==  Body     ======================================== */

  OLED_Switch_ON();
    // Provide power (13V) to OLED panel, enable display

  GUI_SetTextMode(GUI_TM_TRANS);  // write with transparent background


 // ==  Init. Accelerometer    =========================

  // *** Init. parameters for LSM6DS3 Gyroscope ("G") :
  LSM6DS3_InitStruct.G_X_Axis = LSM6DS3_G_XEN_DISABLE;
  LSM6DS3_InitStruct.G_Y_Axis = LSM6DS3_G_YEN_DISABLE;
  LSM6DS3_InitStruct.G_Z_Axis = LSM6DS3_G_ZEN_DISABLE;
    // so, no need to initialize other Gyro parameters

  // *** Init. parameters for LSM6DS3 Accelero ("XL") :
  LSM6DS3_InitStruct.X_X_Axis = LSM6DS3_XL_XEN_ENABLE;
  LSM6DS3_InitStruct.X_Y_Axis = LSM6DS3_XL_YEN_ENABLE;
  LSM6DS3_InitStruct.X_Z_Axis = LSM6DS3_XL_ZEN_ENABLE;
  LSM6DS3_InitStruct.X_FullScale = LSM6DS3_XL_FS_4G;
  LSM6DS3_InitStruct.X_OutputDataRate = LSM6DS3_XG_FIFO_ODR_6600HZ;

  //*** LSM6DS3_Init( &LSM6DS3_InitStruct );
  Success = LSM6DS3_Init( &LSM6DS3_InitStruct );
  Led_StopNBlinkOnFalse ( Success );


  //*** Define "Wake Up IT" as trigger event  :

  // Set SLOPE_FDS bit[4] in register TAP_CFG 0x58 
  I2C2_WriteSingleReg( LSM6DS3_CHIPID, 
			LSM6DS3_XG_TAP_CFG, 0x10 );

  // Define duration =0 so IT generated as soon as value exceeds threshold
  // Bits WAKE_DUR = [6:5] of reg. WAKE_UP_DUR (0x5C)
  I2C2_WriteSingleReg( LSM6DS3_CHIPID, 
			LSM6DS3_XG_WAKE_UP_DUR, 0x00 );

  // Set wake-up threshold @ slope (i.e delta/2) ~0.5g (so delta=g) 
  // using bits [5:0] of register WAKE_UP_THS (@ 0x5B) (1 LSB = FullScale / 2**6)
  // Here FullScale =4g, so g/2 = FullScale / 2**3 = (FullScale/2**6)*2**3
  I2C2_WriteSingleReg( LSM6DS3_CHIPID, 
			LSM6DS3_XG_WAKE_UP_THS, 0x07 );  //0x8 for delat=1g

  // Assign wake-up event to INT1 pin 
  // Set bit [5] INT1_WU of register MD1_CFG, disable other IT
  I2C2_WriteSingleReg( LSM6DS3_CHIPID, 
			LSM6DS3_XG_MD1_CFG, 0x20 );

  // Set LIR bit [0] to latch interrupt
  I2C2_RmodWSingleReg( LSM6DS3_CHIPID, 
			LSM6DS3_XG_TAP_CFG, 0x01, 0x01 );

  // Enable External Interrupt from LSM6DS3 IT1 on STM32
  Enable_ExtIT( INT1_ACC_GYR_PORT, INT1_ACC_GYR_PIN, RISING);

  //*** Set-up FIFO: what to store, at what rate, etc.


  // FIFO_CTRL1, FIFO_CTRL2: set Fifo Threshold = 8000/2 = 4000 = 0xFA0
  I2C2_WriteSingleReg( LSM6DS3_CHIPID, 
			LSM6DS3_XG_FIFO_CTRL1 , 0xA0 );
  I2C2_WriteSingleReg( LSM6DS3_CHIPID, 
			LSM6DS3_XG_FIFO_CTRL2 , 0x0F );

  // FIFO_CTRL3:  store accelero samples, no decimation: DEC_FIFO_XL [2:0]=0b001
  I2C2_WriteSingleReg( LSM6DS3_CHIPID, 
			LSM6DS3_XG_FIFO_CTRL3 , 0x01 );

  // FIFO_CTRL4: default OK

  // FIFO_CTRL5: FIFO initially in bypass mode, FIFO_ODR = 833Hz
   I2C2_WriteSingleReg ( LSM6DS3_CHIPID, 
		LSM6DS3_XG_FIFO_CTRL5,
		LSM6DS3_XG_FIFO_ODR_6600HZ | LSM6DS3_XG_FIFO_MODE_BYPASS );

  //CTRL4_C: stop FIFO on threshold
  I2C2_WriteSingleReg( LSM6DS3_CHIPID, 
			LSM6DS3_XG_CTRL4_C , 0x01 );

  // Note:
  // FIFO content is reset by entering Bypass mode
  // (I assume this also means FIFO read and write pointers back to head)




  //-----------------------------------------------------------

  while (1)
  {

   Delay_ms(4000);  
     // leave time to read results of previous attempt 

   OLED_Clear();
   GUI_SetFont(&GUI_Font16_1);
   GUI_SetColor(GUI_WHITE);  
   GUI_DispStringAt("",50,55); // set position for next text
   OLED_PrintString("READY !!!");


   //---Before Impact :   ---------------------------------


   // Calibration: Measure Accel vector  without impact (gravity only)
   Stm32_Led_ON(); // Get ready for calibration
   Delay_ms(200);
   LSM6DS3_X_GetAxesRaw( Init_Accel_Vector );
   Stm32_Led_OFF(); //  Calibration done

   //Clear (latched) IRQ bit by reading status reg in case it fired upon pressing button
   I2C2_ReadSingleReg ( LSM6DS3_CHIPID, LSM6DS3_XG_WAKE_UP_SRC );


   // Switch FIFO to "Bypass-to-continuous" Mode (FIFO_CTRL5)
   // Note: going thru bypass mode clears the FIFO
   I2C2_RmodWSingleReg ( LSM6DS3_CHIPID, 
    			LSM6DS3_XG_FIFO_CTRL5,
   			LSM6DS3_XG_FIFO_MODE_MASK, 
   			LSM6DS3_XG_FIFO_MODE_BYPASS_THEN_CONTINUOUS );

   // Upon hit, WakeUp IT will fire and 1) cause FIFO to store Accel samples
   // plus 2) cause program to execute ISR (with proper Interrupt Settings)
   // At this point stop acquisition and evaluate magnitude of impact

   Wait_For_Impact();  // waits for Interrupt from LSM6DS3 -- in stm32_it.c
   	// FIFO Mode is enabled in this function as soon as Impact detected



   //---After Impact :   ---------------------------------


   // When Impact occurs and continuous mode starts, we have the equivalent of
   // 8KBytes = 4K samples before 1st sample gets overwritten. 
   // At ~6KHz sample rate that's about 0.7sec, which is enough 

 
   OLED_Clear();
   Score = 0;

   /* Read FIFO results and perform integration */
   	// Integrate squared magnitude of impact over ~10ms 
   	//  which at 833Hz sampling rate is ~10 samples - or 40 samples 3.3KHz
   for (i=0; i < 40; i++)
   {

       // Get Acceleration Vectors:

       FIFO_DataOut_L = I2C2_ReadSingleReg (LSM6DS3_CHIPID, LSM6DS3_XG_FIFO_DATA_OUT_L);
       FIFO_DataOut_H = I2C2_ReadSingleReg (LSM6DS3_CHIPID, LSM6DS3_XG_FIFO_DATA_OUT_H);
       Accel_X_Raw = (int16_t) ( (FIFO_DataOut_H <<8) |  FIFO_DataOut_L );

       FIFO_DataOut_L = I2C2_ReadSingleReg (LSM6DS3_CHIPID, LSM6DS3_XG_FIFO_DATA_OUT_L);
       FIFO_DataOut_H = I2C2_ReadSingleReg (LSM6DS3_CHIPID, LSM6DS3_XG_FIFO_DATA_OUT_H);
       Accel_Y_Raw = (int16_t) ( (FIFO_DataOut_H <<8) |  FIFO_DataOut_L );

       FIFO_DataOut_L = I2C2_ReadSingleReg (LSM6DS3_CHIPID, LSM6DS3_XG_FIFO_DATA_OUT_L);
       FIFO_DataOut_H = I2C2_ReadSingleReg (LSM6DS3_CHIPID, LSM6DS3_XG_FIFO_DATA_OUT_H);
       Accel_Z_Raw = (int16_t) ( (FIFO_DataOut_H <<8) |  FIFO_DataOut_L );

       Accel_Vector[0] = Accel_X_Raw - Init_Accel_Vector[0];
       Accel_Vector[1] = Accel_Y_Raw - Init_Accel_Vector[1];
       Accel_Vector[2] = Accel_Z_Raw - Init_Accel_Vector[2];

#define DIV_FACTOR  1800
      Power =      (Accel_Vector[2]/DIV_FACTOR) * (Accel_Vector[2]/DIV_FACTOR) 
		 + (Accel_Vector[1]/DIV_FACTOR) * (Accel_Vector[1]/DIV_FACTOR)
		 + (Accel_Vector[0]/DIV_FACTOR) * (Accel_Vector[0]/DIV_FACTOR) ;


      BarHeight = Power ; // always positive
      if (BarHeight>3200) BarHeight = 3200;
      BarHeight = BarHeight /32; // from 0 to + 100
      OLED_Fill( 10+ i*2, (uint8_t)(100-BarHeight),     2, (uint8_t)BarHeight,     CYAN );


      Score += Power;
     // Empirically dtermied algorithm :
     // Integrate squared magnitude of acceleration vector -- includes all vibrations
     // Works pretty well with perhaps some saturation effects at violent hits


  }  // end of for i loop


  // ***  Display results :     -------------------------------------

  Score = Score/32 ;
  #define CHAMPION_SCORE 1200
  #define GOOD_SCORE 800
  #define CORRECT_SCORE 500

  GUI_SetColor(GUI_WHITE);
  GUI_DispStringAt("Score : ", 10 , 4);  
  GUI_SetFont(&GUI_Font24_1);
  GUI_DispDecMin( Score );
  GUI_SetFont(&GUI_Font13_1);
  if (Score > CHAMPION_SCORE)
       GUI_DispStringAt("Woooaow!!!", 10, 24);  
  else if (Score > GOOD_SCORE)
       GUI_DispStringAt("Bravo!", 10, 24);  
  else if (Score > CORRECT_SCORE)
       GUI_DispStringAt("Pas mal!", 10, 24);  
  else 
       GUI_DispStringAt("Bon debut...", 10, 24);  


  //--  Report Result as graph bar :  ------------------------

  BarHeight = (Score * 128) / 1024  ;
  if ( BarHeight > 128)
                    BarHeight = 128;
  
  // first darw full bar
  GUI_DrawGradientV( 120, 0,  159,127, 0xFF0000, 0xFFFF00);  // yellow to vertical gradient 

  // then hide what needs to be
  OLED_Fill( 120, 0,  40, 128-BarHeight,  BLACK );


  // -- Display Best Score  ------------------------------------

  GUI_SetFont(&GUI_Font13_1);
  GUI_DispStringAt("Meilleur Score:",10,110); 

  if  (Score > Best_Score)
  {
     Best_Score = Score;
     GUI_SetFont(&GUI_Font20_1);
     GUI_DispDecMin( Best_Score );
     GUI_DispString ("!");
  }   
  else
  {
     GUI_DispDecMin( Best_Score );
  }



   // Back to "FIFO Bypass" Mode (FIFO_CTRL5) to clear FIFO
   I2C2_RmodWSingleReg ( LSM6DS3_CHIPID, 
			LSM6DS3_XG_FIFO_CTRL5,
			LSM6DS3_XG_FIFO_MODE_MASK, 
			LSM6DS3_XG_FIFO_MODE_BYPASS );

   //... clear (latched) IRQ bit by reading status reg
   I2C2_ReadSingleReg ( LSM6DS3_CHIPID, LSM6DS3_XG_WAKE_UP_SRC );


 }  //end of while(1)

}



/***************************************************************END OF FILE****/
