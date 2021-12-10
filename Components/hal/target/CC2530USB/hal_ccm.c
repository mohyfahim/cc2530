/**************************************************************************************************
  Filename:       hal_ccm.c
  Revised:        $Date: 2012-10-01 13:43:52 -0700 (Mon, 01 Oct 2012) $
  Revision:       $Revision: 31664 $

  Description:    Describe the purpose and contents of the file.


  Copyright 2007-2012 Texas Instruments Incorporated. All rights reserved.

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

/******************************************************************************
 * INCLUDES
 */

#include "osal.h"
#include "hal_aes.h"
#include "hal_ccm.h"
#include "hal_dma.h"
#include "hal_assert.h"

/******************************************************************************
 * MACROS
 */

/******************************************************************************
 * CONSTANTS
 */

/******************************************************************************
 * TYPEDEFS
 */

/******************************************************************************
 * LOCAL VARIABLES
 */

/******************************************************************************
 * GLOBAL VARIABLES
 */

/******************************************************************************
 * FUNCTION PROTOTYPES
 */

/******************************************************************************
 * @fn      SSP_CCM_Auth
 *
 * @brief   Generates CCM Authentication tag U.
 *
 * This is a deprecated function. Use SSP_CCM_Auth_Encrypt instead
 * 
 * input parameters
 *
 * @param   Mval    - Length of authentication field in octets [0,2,4,6,8,10,12,14 or 16]
 * @param   N       - Pointer to 13-byte Nonce
 * @param   M       - Pointer to octet string 'm'
 * @param   len_m   - Length of M[] in octets
 * @param   A       - Pointer to octet string 'a'
 * @param   len_a   - Length of A[] in octets
 * @param   AesKey  - Pointer to AES Key or Pointer to Key Expansion buffer.
 * @param   Cstate  - Pointer to output buffer
 * @param   ccmLVal - ccm L Value to be used. 
 *
 * output parameters
 *
 * @param   Cstate[]    - The first Mval bytes contain Authentication Tag T
 *
 * @return  ZStatus_t
 *
 */
ZStatus_t SSP_CCM_Auth (uint8 Mval, uint8 *N, uint8 *M, uint16 len_m, uint8 *A,
                                    uint16 len_a, uint8 *AesKey, uint8 *Cstate, uint8 ccmLVal)
{
  uint8   B[STATE_BLENGTH], *bptr, *msg_in;
  uint8   remainder;
  uint16  blocks, msg_len;

  (void)AesKey;
  /* Check if authentication is even requested.  If not, exit. */
  if (!Mval) return ZSuccess;

  /* Construct B0 */
  B[0] = (ccmLVal - 1);                               /* L=2, L-encoding = L-1 = 1 */
  if (len_a)  B[0] |= 0x40;               /* Adata bit */
  if (Mval)  B[0] |= (Mval-2) << 2;       /* M encoding */

  osal_memcpy (B+1, N, 13);               /* append Nonce (13-bytes) */

  B[14] = (uint8)(len_m >> 8);            /* append l(m) */
  B[15] = (uint8)(len_m);

  osal_memset (Cstate, 0, STATE_BLENGTH); /* X0 = 0 */

  /* Calculate msg length start with B0 (16) and len_a encode padded with zeros (16) */
  msg_len = 32;

  /* Add len_a with zero pads */
  if (len_a > 14)
  {
    msg_len += (((len_a-14)>>4)<<4);
    msg_len += (((len_a-14)&0x0f) > 0 ? 16 : 0);
  }

  /* Add len_m with zero pads */
  msg_len += ((len_m >> 4) << 4); 
  msg_len += ((len_m & 0x000f) > 0 ? 16 : 0);

  /* allocate OSAL buffer */
  bptr = msg_in = (uint8 *)osal_mem_alloc( msg_len );
  if (!bptr) return ZMemError;

  /* Move B0 to scratch memory */
  osal_memcpy( bptr, B, STATE_BLENGTH );
  bptr += STATE_BLENGTH;

  /* Encode l(a) and move l(a) to scratch memory */
  *bptr++ = (uint8) (len_a >> 8);
  *bptr++ = (uint8) (len_a);

  if (len_a > 14)
  {
    osal_memcpy (bptr, A, 14);
  }
  else
  {
    osal_memset (bptr, 0, 14);
    osal_memcpy (bptr, A, len_a);
  }
  bptr += 14;

  if (len_a > 14)
  {
    len_a -= 14;
    blocks = len_a >> 4;
    remainder = len_a & 0x0f;

    osal_memcpy(bptr, A+14, blocks << 4);
    bptr += (blocks << 4);

    if (remainder)
    {
      osal_memset (bptr, 0, 16);
      osal_memcpy (bptr, A+14+(blocks << 4), remainder);
      bptr += 16;
    }
  }

  /* Move M to scratch memory */
  blocks = len_m >> 4;
  remainder = len_m & 0x0f;
  osal_memcpy(bptr, M, blocks << 4);
  bptr += (blocks << 4);
  if (remainder)
  {
    osal_memset (bptr, 0, 16);
    osal_memcpy (bptr, M+(blocks << 4), remainder);
    bptr += 16;
  }

  /* The allocated buffer size must be the same as
   * the distance of moving buffer pointer.
   */
  HAL_ASSERT(msg_len == (uint16)(bptr-msg_in));

#if (defined HAL_AES_DMA) && (HAL_AES_DMA == TRUE)
  /* Prepare CBC-MAC */
  AES_SETMODE(CBC_MAC);
  AesLoadIV( Cstate );
  AesDmaSetup( Cstate, STATE_BLENGTH, msg_in, msg_len );
  AES_SET_ENCR_DECR_KEY_IV( AES_ENCRYPT );

  /* Calculate the block size and kick it off */
  blocks = (msg_len >> 4) - 1;
  while (blocks--)
  {
    /* CBC-MAC does not generate output until the last block */
    AES_START();
    while( !(ENCCS & 0x08) );
  }

  /* Switch to CBC mode for the last block and kick it off */
  AES_SETMODE(CBC);
  AES_START();
  while( !HAL_DMA_CHECK_IRQ( HAL_DMA_AES_OUT ) );
#else
  /* Prepare CBC-MAC */
  AES_SETMODE(CBC_MAC);
  AesLoadIV( Cstate );
  AES_SET_ENCR_DECR_KEY_IV( AES_ENCRYPT );

  /* Store the beginning of the input buffer */
  bptr = msg_in;

  /* Calculate the block size and kick it off */
  blocks = (msg_len >> 4) - 1;
  while (blocks--)
  {
    /* Load the block */
    AesLoadBlock( bptr );
    bptr += STATE_BLENGTH;
  }

  /* Switch to CBC mode for the last block and kick it off */
  AES_SETMODE(CBC);

  /* Delay is required for non-DMA AES */
  HAL_AES_DELAY();

  /* CBC-MAC does not generate output until the last block */
  AesStartBlock( Cstate, bptr );
#endif

  osal_mem_free( msg_in );

  return ZSuccess;
}

/******************************************************************************
 * @fn      SSP_CCM_Encrypt
 *
 * @brief   Performs CCM encryption.
 *
 * This is a deprecated function. Use SSP_CCM_Auth_Encrypt instead
 * 
 * input parameters
 *
 * @param   Mval    - Length of authentication field in octets [0,2,4,6,8,10,12,14 or 16]
 * @param   N       - Pointer to 13-byte Nonce
 * @param   M       - Pointer to octet string 'm'
 * @param   len_m   - Length of M[] in octets
 * @param   AesKey  - Pointer to AES Key or Pointer to Key Expansion buffer.
 * @param   Cstate  - Pointer to Authentication Tag U
 * @param   ccmLVal - ccm L Value to be used. 
 *
 * output parameters
 *
 * @param   M[]         - Encrypted octet string 'm'
 * @param   Cstate[]    - The first Mval bytes contain Encrypted Authentication Tag U
 *
 * @return  ZStatus_t
 *
 */
ZStatus_t SSP_CCM_Encrypt (uint8 Mval, uint8 *N, uint8 *M, uint16 len_m,
                                                  uint8 *AesKey, uint8 *Cstate, uint8 ccmLVal)
{
  uint8   A[STATE_BLENGTH], T[STATE_BLENGTH];
  uint8 *bptr = NULL;
  uint8   remainder;
  uint16  blocks;
#if (!defined HAL_AES_DMA) || (HAL_AES_DMA == FALSE)
  uint8  *bptr_save;
#endif

  (void)AesKey;

  osal_memcpy (T, Cstate, Mval);

  A[0] = (ccmLVal - 1);                   /* L=2, L-encoding = L-1 = 1 */
  osal_memcpy (A+1, N, 13);   /* append Nonce */
  A[14] = A[15] = 0;          /* clear the CTR field */

  /* Claculate block sizes */
  blocks = len_m >> 4;
  remainder = len_m & 0x0f;
  if (remainder) blocks++;

  if(len_m > 0)
  {
    /* Allocate OSAL memory for message buffer */
    bptr = (uint8 *)osal_mem_alloc( blocks*STATE_BLENGTH );
    if (!bptr) return ZMemError;
    
    /* Move message into position and pad with zeros */
    osal_memcpy( bptr, M, len_m );
    if(remainder)
    {
      osal_memset( bptr+len_m, 0, STATE_BLENGTH-remainder );
    }
  }

#if (defined HAL_AES_DMA) && (HAL_AES_DMA == TRUE)
  /* Set OFB mode and encrypt T to U */
  AES_SETMODE(OFB);
  AesLoadIV(A);
  AesDmaSetup( Cstate, STATE_BLENGTH, T, STATE_BLENGTH ); /* T -> U */
  AES_SET_ENCR_DECR_KEY_IV( AES_ENCRYPT );

  /* Kick it off */
  AES_START();
  while( !HAL_DMA_CHECK_IRQ( HAL_DMA_AES_OUT ) );

  /* Switch to CTR mode to encrypt message. CTR field must be greater than zero */
  if (blocks > 0)
  {
    AES_SETMODE(CTR);
    A[15] = 1;
    AesLoadIV(A);
    AesDmaSetup( bptr, blocks*STATE_BLENGTH, bptr, blocks*STATE_BLENGTH );
    AES_SET_ENCR_DECR_KEY_IV( AES_ENCRYPT );
    
    /* Kick it off */
    while (blocks--)
    {
      AES_START();
      while ( !(ENCCS & 0x08) );
    }
  }
#else
  /* Set OFB mode and encrypt T to U */
  AES_SETMODE(OFB);
  AesLoadIV(A);
  AES_SET_ENCR_DECR_KEY_IV( AES_ENCRYPT );

  /* Load and start the short block */
  AesStartShortBlock( Cstate, T );

  /* Switch to CTR mode to encrypt message. CTR field must be greater than zero */
  AES_SETMODE(CTR);
  A[15] = 1;
  AesLoadIV(A);
  AES_SET_ENCR_DECR_KEY_IV( AES_ENCRYPT );

  /* Backup bptr to bptr_save */
  bptr_save = bptr;
  while(blocks--)
  {
    AesStartShortBlock( bptr, bptr );
    bptr += STATE_BLENGTH;
  }

  /* restore bptr to be freed */
  bptr = bptr_save;
#endif

  if(bptr)
  {
    /* Copy the encrypted message back to M and return OSAL memory */
    osal_memcpy( M, bptr, len_m );
    osal_mem_free( bptr );
  }

  return ZSuccess;
}

/******************************************************************************
 * @fn      SSP_CCM_Decrypt
 *
 * @brief   Performs CCM decryption.
 *
 * This is a deprecated function. Use SSP_CCM_InvAuth_Decrypt instead
 *
 * input parameters
 *
 * @param   Mval    - Length of authentication field in octets [0,2,4,6,8,10,12,14 or 16]
 * @param   N       - Pointer to 13-byte Nonce
 * @param   C       - Pointer to octet string 'c', where 'c' = encrypted 'm' || encrypted auth tag U
 * @param   len_c   - Length of C[] in octets
 * @param   AesKey  - Pointer to AES Key or Pointer to Key Expansion buffer.
 * @param   Cstate  - Pointer AES state buffer (cannot be part of C[])
 * @param   ccmLVal - ccm L Value to be used. 
 *
 * output parameters
 *
 * @param   C[]         - Decrypted octet string 'm' || auth tag T
 * @param   Cstate[]    - The first Mval bytes contain  Authentication Tag T
 *
 * @return  ZStatus_t
 *
 */
ZStatus_t SSP_CCM_Decrypt (uint8 Mval, uint8 *N, uint8 *C, uint16 len_c,
                                                     uint8 *AesKey, uint8 *Cstate, uint8 ccmLVal)
{
  uint8   *bptr;
  uint8   A[STATE_BLENGTH], U[STATE_BLENGTH];
  uint8   i;
  uint16  blocks;
#if (!defined HAL_AES_DMA) || (HAL_AES_DMA == FALSE)
  uint8  *bptr_save;
#endif

  (void)AesKey;

  A[0] = (ccmLVal - 1);                  /* L=2, L-encoding = L-1 = 1 */
  osal_memcpy (A+1, N, 13);  /* append Nonce */
  A[14] = A[15] = 0;         /* clear the CTR field */

  /* Seperate M from C */
  i = len_c - Mval;
  blocks = i >> 4;
  if (i & 0x0f) blocks++;

  /* Extract U and pad it with zeros */
  osal_memset(U, 0, STATE_BLENGTH);
  osal_memcpy(U, C+i, Mval);

#if (defined HAL_AES_DMA) && (HAL_AES_DMA == TRUE)
  /* Set OFB mode to encrypt U to T */
  AES_SETMODE(OFB);
  AesLoadIV(A);
  AesDmaSetup( Cstate, STATE_BLENGTH, U, STATE_BLENGTH ); /* U -> T */
  AES_SET_ENCR_DECR_KEY_IV( AES_ENCRYPT );

  /* Kick it off */
  AES_START();
  while ( !HAL_DMA_CHECK_IRQ( HAL_DMA_AES_OUT ) );
#else
  /* Set OFB mode to encrypt U to T */
  AES_SETMODE(OFB);
  AesLoadIV(A);
  AES_SET_ENCR_DECR_KEY_IV( AES_ENCRYPT );

  /* Load and start short block */
  AesStartShortBlock( Cstate, U );
#endif

  /* Allocate OSAL memory for message buffer */
  bptr = (uint8 *)osal_mem_alloc( blocks*STATE_BLENGTH );
  if (!bptr) return ZMemError;

  /* Move message into position and pad with zeros */
  osal_memset( bptr, 0, blocks*STATE_BLENGTH );
  osal_memcpy( bptr, C, i );

#if (defined HAL_AES_DMA) && (HAL_AES_DMA == TRUE)
  /* Switch to CTR mode to decrypt message. CTR field must be greater than zero */
  if (blocks > 0)
  {
    AES_SETMODE(CTR);
    A[15] = 1;
    AesLoadIV(A);
    AesDmaSetup( bptr, blocks*STATE_BLENGTH, bptr, blocks*STATE_BLENGTH );
    AES_SET_ENCR_DECR_KEY_IV( AES_DECRYPT );
    
    /* Kick it off */
    while (blocks--)
    {
      AES_START();
      while ( !(ENCCS & 0x08) );
    }
  }
#else
  /* Switch to CTR mode to decrypt message. CTR field must be greater than zero */
  AES_SETMODE(CTR);
  A[15] = 1;
  AesLoadIV(A);
  AES_SET_ENCR_DECR_KEY_IV( AES_DECRYPT );

  /* Backup bptr to bptr_save */
  bptr_save = bptr;
  while (blocks--)
  {
    /* Load and start short block */
    AesStartShortBlock( bptr, bptr );
    bptr += STATE_BLENGTH;
  }

  /* Restore bptr to be freed */
  bptr = bptr_save;
#endif

  /* Copy the decrypted message back to M and return OSAL memory */
  osal_memcpy( C, bptr, i );
  osal_mem_free(bptr);

  /* Copy T to where U used to be */
  osal_memcpy(C+i, Cstate, Mval);

  return ZSuccess;
}

/******************************************************************************
 * @fn      SSP_CCM_InvAuth
 *
 * @brief   Verifies CCM authentication.
 *
 * This is a deprecated function. Use SSP_CCM_InvAuth_Decrypt instead
 *
 * input parameters
 *
 * @param   Mval    - Length of authentication field in octets [0,2,4,6,8,10,12,14 or 16]
 * @param   N       - Pointer to 13-byte Nonce
 * @param   C       - Pointer to octet string 'c' = 'm' || auth tag T
 * @param   len_c   - Length of C[] in octets
 * @param   A       - Pointer to octet string 'a'
 * @param   len_a   - Length of A[] in octets
 * @param   AesKey  - Pointer to AES Key or Pointer to Key Expansion buffer.
 * @param   Cstate  - Pointer to AES state buffer (cannot be part of C[])
 * @param   ccmLVal - ccm L Value to be used. 
 *
 * output parameters
 *
 * @param   Cstate[]    - The first Mval bytes contain computed Authentication Tag T
 *
 * @return  ZStatus_t
 *
 */
ZStatus_t SSP_CCM_InvAuth (uint8 Mval, uint8 *N, uint8 *C, uint16 len_c, uint8 *A,
                                        uint16 len_a, uint8 *AesKey, uint8 *Cstate, uint8 ccmLVal)
{
  uint8   i, t;
  ZStatus_t status;

  // Check if authentication is even requested.  If not, return
  // success and exit.  This check is actually not needed because
  // the rest of the code works fine even with Mval==0.  I added
  // it to reduce unnecessary calculations and to speed up
  // performance when Mval==0
  if (!Mval) return ZSuccess;

  t = len_c - Mval;

  if( (status = SSP_CCM_Auth (Mval, N, C, t, A, len_a, AesKey, Cstate, ccmLVal)) != ZSuccess )
  {
    return status;
  }

  for (i = 0; i < Mval; i++)
  {
    if (Cstate[i] != C[t++])
    {
      status = ZFailure;
      break;
    }
  }
  return status;
}

/******************************************************************************
 * @fn      SSP_CCM_Auth_Encrypt
 *
 * @brief   CCM Authentication and Encryption.
 *
 * input parameters
 *
 * @param   encrypt  - set to true to encrypt, false for no encryption
 * @param   Mval    - Length of authentication field in octets [0,2,4,6,8,10,12,14 or 16]
 * @param   N       - Pointer to 13-byte Nonce
 * @param   M       - Pointer to octet string 'm'
 * @param   len_m   - Length of M[] in octets
 * @param   A       - Pointer to octet string 'a'
 * @param   len_a   - Length of A[] in octets
 * @param   AesKey  - Pointer to AES Key or Pointer to Key Expansion buffer.
 * @param   Cstate  - Pointer to output buffer
 * @param   ccmLVal - ccm L Value to be used. 
 *
 * output parameters
 *
 * @param   Cstate[]    - The first Mval bytes contain Authentication Tag T
 *
 * @return  ZStatus_t
 *
 */
ZStatus_t SSP_CCM_Auth_Encrypt (bool encrypt, uint8 Mval, uint8 *N, 
                                  uint8 *M, uint16 len_m, uint8 *A, 
                                  uint16 len_a, uint8 *AesKey, uint8 *Cstate, 
                                  uint8 ccmLVal)
{
  ZStatus_t status = ZSuccess;
  status = SSP_CCM_Auth(Mval, N, M, len_m, A, len_a, AesKey, Cstate, ccmLVal);
  
  if (status == ZSuccess)
  {
    status = SSP_CCM_Encrypt(Mval, N, M, len_m, AesKey, Cstate, ccmLVal);
  }
  return status;
}

/******************************************************************************
 * @fn      SSP_CCM_InvAuth_Decrypt
 *
 * @brief   CCM Inverse authentication and Decryption.
 *
 * input parameters
 *
 * @param   decrypt  - set to true to decrypt, false for no decryption
 * @param   Mval    - Length of authentication field in octets [0,2,4,6,8,10,12,14 or 16]
 * @param   N       - Pointer to 13-byte Nonce
 * @param   C       - Pointer to octet string 'c' = 'm' || auth tag T
 * @param   len_c   - Length of C[] in octets
 * @param   A       - Pointer to octet string 'a'
 * @param   len_a   - Length of A[] in octets
 * @param   AesKey  - Pointer to AES Key or Pointer to Key Expansion buffer.
 * @param   Cstate  - Pointer to AES state buffer (cannot be part of C[])
 * @param   ccmLVal - ccm L Value to be used. 
 *
 * output parameters
 *
 * @param   Cstate[]    - The first Mval bytes contain computed Authentication Tag T
 *
 * @return  ZStatus_t
 *
 */
ZStatus_t SSP_CCM_InvAuth_Decrypt (bool decrypt, uint8 Mval, uint8 *N, 
                                     uint8 *C, uint16 len_c, uint8 *A,
                                     uint16 len_a, uint8 *AesKey, uint8 *Cstate,
                                     uint8 ccmLVal)
{
  ZStatus_t status = ZSuccess;
  
  status = SSP_CCM_Decrypt(Mval, N, C, len_c, AesKey, Cstate, ccmLVal);
  
  if (status == ZSuccess)
  {
    status = SSP_CCM_InvAuth(Mval, N, C, len_c, A, len_a, AesKey, Cstate, ccmLVal);
						  
  }
  return status;
}

