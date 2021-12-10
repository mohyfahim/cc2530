/**************************************************************************************************
  Filename:       MacComDef.h
  Revised:        $Date: 2014-10-09 14:02:20 -0700 (Thu, 09 Oct 2014) $
  Revision:       $Revision: 40546 $

  Description:    Type definitions and macros.


  Copyright 2014 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License"). You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product. Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
**************************************************************************************************/

#ifndef MACCOMDEF_H
#define MACCOMDEF_H

#ifdef __cplusplus
extern "C"
{
#endif


/*********************************************************************
 * INCLUDES
 */
#include "comdef.h"
#include "saddr.h"


/*********************************************************************
 * CONSTANTS
 */

 /*** Return Values ***/
#define ZSUCCESS                          SUCCESS

  
#define MAC_NV_EX_LEGACY                   0x0000


// OSAL NV item IDs
#define MAC_NV_EXTADDR                     0x0001

#define MAC_NV_INITIALIZED                 0x0010

#define MAC_NV_COORD_EXTENDED_ADDRESS      0x0021
#define MAC_NV_COORD_SHORT_ADDRESS         0x0022
#define MAC_NV_PAN_ID                      0x0023
#define MAC_NV_RX_ON_WHEN_IDLE             0x0024
#define MAC_NV_SHORT_ADDRESS               0x0025
#define MAC_NV_SECURITYENABLED             0x0025
#define MAC_NV_LOGICALCHANNEL              0x0026
#define MAC_NV_EXTENDED_ADDRESS            0x0027
#define MAC_NV_LOGICAL_CHANNEL             0x0028
#define MAC_NV_ASSOCIATION_PERMIT          0x0029
#define MAC_NV_DEVICE_RECORD               0x002A
#define MAC_NV_DEVICE_RECORD_NUM           0x002B
#define MAC_NV_SRC_MATCH_ENABLE            0x002C
   
#define MAC_NV_PAN_COORD_SHORT_ADDRESS     0x0030
#define MAC_NV_PAN_COORD_EXTENDED_ADDRESS  0x0031
#define MAC_NV_DEVICE_TABLE_ENTRIES        0x0032
#define MAC_NV_KEY_TABLE_ENTRIES           0x0033
  
#define MAC_NV_DEVICE_TABLE                0x0040 //50 allocated for the same
#define MAC_NV_KEY_TABLE                   0x0050
  
  

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* MACCOMDEF_H */
