
/*******************************************************************************
 * ApplyTilt_XY.c
 *
 * Private function of main()
 * 
 * Author: La BlueFrog, 2015
 *
 *******************************************************************************/

#include "LBF_Global.h"
#include "User_Configuration.h"

// Update an XY coordinate according to accelerometer XY information
// Accelerometer assumed to already have been initlialized


void ApplyTilt_XY ( int8_t* pDelta_X, int8_t* pDelta_Y)
{

int8_t  X_Accel;
int8_t  Y_Accel;



    while ( (I2C2_ReadSingleReg(LSM6DS3_CHIPID, 0x1E) & 0x01) == 0x00);
        // Status Reg
        // Wait for XLDA bit to be set

    X_Accel = (int8_t) I2C2_ReadSingleReg(LSM6DS3_CHIPID, 0x29); //High Byte only
    Y_Accel = (int8_t) I2C2_ReadSingleReg(LSM6DS3_CHIPID, 0x2B); // High Byte only


    //Update Positions  :

//    if (X_Accel > 16)
    if (X_Accel > 1)
	*pDelta_X = (-1);
//    else if (X_Accel < -16)
    else if (X_Accel < -1)
	*pDelta_X = (+1);
    else  
	*pDelta_X = 0;

//    if (Y_Accel < -16)
    if (Y_Accel < -1)
	*pDelta_Y = (-1);
//    else if (Y_Accel > 16)
    else if (Y_Accel > 1)
	*pDelta_Y = (+1);
    else  
	*pDelta_Y = 0;

}

/* ***  END OF FILE ********************************************/



