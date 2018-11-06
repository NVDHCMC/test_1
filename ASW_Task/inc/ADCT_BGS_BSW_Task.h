/**
  ******************************************************************************
  * @file    ADCT_BGS_BSW_Task.h
  * @author  ADCT Team - VuNTDH
  * @version V0.0.1
  * @date    23-Mar-2017
  * @brief   This file contains data structure and function prototype for runable entity used in SNMP_FPT project.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 ADCT TEAM</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
#ifndef ASW_TASK_INC_ADCT_BGS_BSW_TASK_H_
#define ASW_TASK_INC_ADCT_BGS_BSW_TASK_H_
#include "ADCT_BGS_DataTypes.h"
#include "ADCT_BGS_FPT_Delay.h"
void ADCT_BGS_ASW_100ms_TaskHandler(TM_DELAY_Timer_t* timer, void * parameter);
void ADCT_BGS_ASW_100ms_Init(void);
void ADCT_BGS_ASW_10ms_Init(void);
void ADCT_BGS_ASW_1s_Init(void);
void ADCT_BGS_ASW_1ms_Init(void);
#endif /* ASW_TASK_INC_ADCT_BGS_BSW_TASK_H_ */
