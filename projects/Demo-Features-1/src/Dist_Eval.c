/*******************************************************************************
 * Dist_Eval.c
 * 
 * Author: La BlueFrog, 2015
 *
 *******************************************************************************/

#include "LBF_Global.h"
#include "User_Configuration.h"

boolean_t 	Detect_NegZ_Accel(void);


/*******************************************************************************/
void DistEval_Demo( void )
{
 
uint16_t  	Distance_mm;
uint8_t		CoarseRange;
uint16_t 	ColorTable[7] = {0x319F, 0x001F, 0x001C, 0x0017, 0x0013, 0x000F, 0x000C } ;
#define 	ORIG_BAR_X	120
#define 	ORIG_BAR_Y	10
#define 	SEGMENT_HEIGHT	15	
#define 	WIDTH_BAR	30		 
uint8_t		Slice[7] = {0, 0, 0, 0, 0, 0, 0} ;
uint8_t		i;	 


   // --  Display Intensity Scale

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


   // -- Set VL6180X for Continuous Range Measurement 

   // Set bits [2:0] of Reg.0x14 to 0b100: enable internal IT on sample ready
   I2C2_WriteSingleReg (VL6180X_CHIPID, 0x0014, 0x4);

    // Wait until device ready to start ranging
    while ( ( I2C2_ReadSingleReg(VL6180X_CHIPID, 0x004D) & 0x01) != 0x01);

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

        // Calculte Distance
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


         // Do we need to exit? 
         if ( Detect_NegZ_Accel() )
         {
           I2C2_WriteSingleReg(VL6180X_CHIPID, 0x18, 0x01); // stop ranging
	   return;
         }

   }  // end while(1)

}

/**************************************END OF FILE****/// 
