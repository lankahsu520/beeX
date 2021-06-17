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
#ifndef __BEEX_123_H__
#define __BEEX_123_H__

//******************************************************************************
//** include **
//******************************************************************************
#include "utilx9.h"

#include "beex_api.h"

#define TAG "BEEX_123"
#define BEEX_UUID "1234567890"

//******************************************************************************
//** feature **
//******************************************************************************
#define USE_BEEX_123_UV
#define USE_ASYNC_CREATE

#define USE_MCTT_BEE
#define USE_MCTT_BEE_DEMO


//******************************************************************************
//** define **
//******************************************************************************
#define FILENAME_OF_MCTT_HONEYCOMB "mctt_honeycomb.json"


//******************************************************************************
//** function **
//******************************************************************************

void app_save(void);


#endif
