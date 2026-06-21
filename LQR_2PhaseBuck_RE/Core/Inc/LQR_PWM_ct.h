/*
 * LQR_PWM_ct.h
 *
 *  Created on: Apr 3, 2026
 *      Author: AORUS
 */

#ifndef INC_LQR_PWM_CT_H_
#define INC_LQR_PWM_CT_H_

#include "main.h"

void LQR_Init();
void LQR_Reset();
void LQR_Start();
void LQR_Stop();
void LQR_FaultStop();
void LQR_execute(void);


#endif /* INC_LQR_PWM_CT_H_ */
