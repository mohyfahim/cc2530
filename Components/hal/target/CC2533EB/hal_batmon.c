/**************************************************************************************************
  Filename:       _hal_batmon.c
  Revised:        $Date: 2010-10-12 17:32:20 -0700 (Tue, 12 Oct 2010) $
  Revision:       $Revision: 24108 $

  Description:    This file is the implementation for the HAL BATMON subsystem.


  Copyright 2015 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, 
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

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */

#include "hal_board_cfg.h"
#include "hal_batmon.h"
#include "hal_defs.h"
#include "hal_mcu.h"
#include "hal_sleep.h"

/**************************************************************************************************
 * @fn          HalBatMonRead
 *
 * @brief       This function is the subsystem utility read function and should be called before
 *              any Vdd-critical operation (e.g. flash write or erase operations.)
 *
 * input parameters
 *
 * @param       vddMask - A valid BATTMON_VOLTAGE mask.
 *
 * output parameters
 *
 * None.
 *
 * @return      TRUE if the measured Vdd exceeds the paramter level; FALSE otherwise.
 **************************************************************************************************
 */
uint8 HalBatMonRead(uint8 vddMask)
{
  uint8 rtrn = TRUE;

#if (defined HAL_BATMON) && (HAL_BATMON == TRUE)
  MONMUX = 0;           // Setup BATTMON mux to measure AVDD5.
  BATMON = vddMask;
  halSleepWait(2);      // Wait at least 2 us before reading BATTMON_OUT.
  rtrn = (BATMON & BATTMON_OUT) ? TRUE : FALSE;
  BATMON = BATTMON_PD;  // Turn off for power saving.
#endif

  return rtrn;
}

/**************************************************************************************************
*/
