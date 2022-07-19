/***************************************************************************
 * Copyright (C) 2017 - 2020, Lanka Hsu, <lankahsu@gmail.com>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
#ifndef __BEEX_DEF_H__
#define __BEEX_DEF_H__

#ifdef __cplusplus
extern "C" {
#endif

#define LIBBEEX_VERSION 0x01000000

//******************************************************************************
//** include **
//******************************************************************************


//******************************************************************************
//** define **
//******************************************************************************
typedef struct
{
	uint8_t cmd;
	uint8_t state;
	uint8_t key[LEN_OF_NAME128];
	uint16_t size;
	uint8_t data[LEN_OF_BUF1024];
} ir_manager_t, *ir_manager_p;

//******************************************************************************
//** function **
//******************************************************************************



#ifdef __cplusplus
}
#endif
#endif
