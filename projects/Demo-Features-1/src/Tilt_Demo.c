
/*******************************************************************************
 * Tilt_Demo.c
 *
 * Private function of main()
 * 
 * Author: La BlueFrog, 2015
 *
 *******************************************************************************/

#include "LBF_Global.h"
#include "User_Configuration.h"

boolean_t 	DemoChangeRequest(void);


void	Tilt_Demo( void )
{

#define  SCREEN_SIZE_X  160
#define  SCREEN_SIZE_Y  128
#define  BALL_SIZE	6

void GetNewPos ( int16_t BallPosX, int16_t BallPosY, int16_t* pBallPosXNew,int16_t* pBallPosYNew );
void Redraw_Ball ( int16_t BallPosX, int16_t BallPosY, int16_t pBallPosXNew, int16_t pBallPosYNew );

int16_t		BallPosX, BallPosY;
int16_t		BallPosXNew, BallPosYNew;


    OLED_Clear();
    BallPosX = SCREEN_SIZE_X /2;        
    BallPosY = SCREEN_SIZE_Y /2;        


    while(1)
    {
        GetNewPos( BallPosX, BallPosY, &BallPosXNew, &BallPosYNew );
        Redraw_Ball ( BallPosX, BallPosY, BallPosXNew, BallPosYNew );

        BallPosX = BallPosXNew;
        BallPosY = BallPosYNew;

        if (DemoChangeRequest())
           return;
    }
}


/* ================================================== */

void GetNewPos ( int16_t BallPosX, int16_t BallPosY, int16_t* pBallPosXNew,int16_t* pBallPosYNew )
{

uint8_t  X_Accel_LowByte, X_Accel_HighByte;
uint8_t  Y_Accel_LowByte, Y_Accel_HighByte;

int16_t   X_Accel, Y_Accel;
int16_t   DeltaX, DeltaY;


    while ( (I2C2_ReadSingleReg(LSM6DS3_CHIPID, 0x1E) & 0x01) == 0x00);
        // Status Reg
        // Wait for XLDA bit to be set


    X_Accel_LowByte = I2C2_ReadSingleReg(LSM6DS3_CHIPID, 0x28);
    X_Accel_HighByte = I2C2_ReadSingleReg(LSM6DS3_CHIPID, 0x29);
    Y_Accel_LowByte = I2C2_ReadSingleReg(LSM6DS3_CHIPID, 0x2A);
    Y_Accel_HighByte = I2C2_ReadSingleReg(LSM6DS3_CHIPID, 0x2B);


    // Get Acceleration, signed value, full scale = +-2g
    X_Accel = (int16_t) ( (X_Accel_HighByte << 8) + X_Accel_LowByte );
    Y_Accel = (int16_t) ( (Y_Accel_HighByte << 8) + Y_Accel_LowByte );

    DeltaX = X_Accel /256 ; 
    DeltaY = Y_Accel /256 ; 
 
    // Clip Deltas
    if (DeltaX > 2)  DeltaX = 2;
    if (DeltaX < -2)  DeltaX = -2;
    if (DeltaY > 2)  DeltaY = 2;
    if (DeltaY < -2)  DeltaY = -2;


    //Update Positions  Roll over if needed
    BallPosX += DeltaX;
    if (BallPosX  < 0)  
	{ BallPosX = 0; }
    if (BallPosX  > (SCREEN_SIZE_X - BALL_SIZE))  
	{ BallPosX = SCREEN_SIZE_X - BALL_SIZE;  }

    BallPosY -= DeltaY;  //take into account screen connector is folded
    if (BallPosY  < 0)  
	{ BallPosY = 0; }
    if (BallPosY  > (SCREEN_SIZE_Y - BALL_SIZE))  
	{ BallPosY = SCREEN_SIZE_Y - BALL_SIZE;  }

    *pBallPosXNew = BallPosX;
    *pBallPosYNew = BallPosY;

}

/* ================================================== */


void Redraw_Ball ( int16_t BallPosX, int16_t BallPosY, int16_t BallPosXNew, int16_t BallPosYNew )
{
    if ( (BallPosX != BallPosXNew) || (BallPosY != BallPosYNew) )
    {
	OLED_Fill( (uint8_t)BallPosX, (uint8_t)BallPosY, 
                   BALL_SIZE, BALL_SIZE,
		   BLACK ); //dark grey
	OLED_Fill( (uint8_t)BallPosXNew, (uint8_t)BallPosYNew, 
                   BALL_SIZE, BALL_SIZE,
		   CYAN );
    }
}


