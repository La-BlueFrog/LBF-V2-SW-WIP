/*******************************************************************************
 * DecodeBMPFile.c
 *
 * Private function of main()
 * 
 * Author: La BlueFrog, 2015
 *
 *******************************************************************************/
#include "LBF_Global.h"
#include "User_Configuration.h"

#include "string.h"

void ApplyTilt_XY ( int8_t *pPosX, int8_t *pPosY );
boolean_t 	Detect_VL6180X_Covered(void);
boolean_t 	Detect_NegZ_Accel(void);


void 	SplashScreen(void)
{

boolean_t 	DemoChangeRequest(void);
boolean_t  	DecodeBMPFile(char* FileName);

int16_t		OriginPoint_X = 0, OriginPoint_Y = 0;
int8_t		Delta_X = 0, Delta_Y = 0;

//int8_t  	X_Accel = 0, Y_Accel = 0;
uint8_t 	Temp_L, Temp_H;
int32_t		Temp;


   // -----   First, display BlueFrog + Text + Temp  ---------------

    DecodeBMPFile("SPLASH.SPE"); 
   // Decode file if present else blue screen

   GUI_SetFont(&GUI_Font20_1);
   GUI_SetColor(0xFFFFFF);  // White
   GUI_SetTextMode(GUI_TM_TRANS);  // write with transparent background

   // Display Name
   GUI_DispStringAt(" La\n BlueFrog", 0,87 );  

   // Get and Display Temperature (16LSB /°C)
   while ( (I2C2_ReadSingleReg(LSM6DS3_CHIPID, 0x1E) & 0x04) == 0x00);
        // Wait for temp sensor to provide relevant data (bit TDA)
   Temp_L = I2C2_ReadSingleReg(LSM6DS3_CHIPID, 0x20);
   Temp_H = I2C2_ReadSingleReg(LSM6DS3_CHIPID, 0x21);
   Temp = (int32_t) ( ((int16_t) (Temp_H<<8 | Temp_L) ) / 16 ) + 25; 
   GUI_DispDecAt( Temp, 120, 105, 2 );  
   GUI_DispString(" C");



   // -----   Wait for user gesture before allowing to move Frog
    while ( !Detect_VL6180X_Covered() );  


   // -----   Erase Text + Temp  before moving Frog ---------------

  
   // Erase Text: Overwrite with bknd color
   GUI_SetColor(0x2C587F);
   // Erase Name
   GUI_DispStringAt(" La\n BlueFrog", 0, 87 );  
   //Erase Temp
   GUI_DispDecAt( Temp, 120, 105, 2 );  
   GUI_DispString(" C");


    while(1) 
    {
     
    // -----   Move Frog ---------------

        ApplyTilt_XY ( &Delta_X, &Delta_Y );

        if ( Delta_X > 0 )
	{
	   if (OriginPoint_X == 159)
		OriginPoint_X = 0;
//	   else if ( (OriginPoint_X < 24) || (OriginPoint_X > 124) )
	   else if ( (OriginPoint_X < 59) || (OriginPoint_X > 105) )
		OriginPoint_X++;
        }

        if ( Delta_X < 0 )
	{
	   if (OriginPoint_X == 0)
		OriginPoint_X = 159;
//	   else if ( (OriginPoint_X < 25) || (OriginPoint_X > 125) )
	   else if ( (OriginPoint_X < 60) || (OriginPoint_X > 106) )
		OriginPoint_X--;
        }

        if ( Delta_Y > 0 )
	{
	   if (OriginPoint_Y == 127)
		OriginPoint_Y = 0;
//	   else if ( (OriginPoint_Y < 16) || (OriginPoint_Y > 114) )
	   else if ( (OriginPoint_Y < 35) || (OriginPoint_Y > 85) )		OriginPoint_Y++;
        }

        if ( Delta_Y < 0 )
	{
	   if (OriginPoint_Y == 0)
		OriginPoint_Y = 127;
//	   else if ( (OriginPoint_Y < 17) || (OriginPoint_Y > 115) )
	   else if ( (OriginPoint_Y < 36) || (OriginPoint_Y > 86) )
		OriginPoint_Y--;
        }


        OLED_WriteReg( 0x2E, OriginPoint_X ); 
          // X that will correspond to first display memory address
        OLED_WriteReg( 0x2F, OriginPoint_Y ); 
          // Y that will correspond to first display memory address

 //       Delay_ms(10);


    // -----   Do we need to exit? ---------------


        if ( Detect_NegZ_Accel() )
        {
           OLED_WriteReg( 0x2E, 0x00 );   // restore initial values
           OLED_WriteReg( 0x2F, 0x00 );   
	   return;
        }

    }
}






/*===============  END OF FILE ===========================================*/


