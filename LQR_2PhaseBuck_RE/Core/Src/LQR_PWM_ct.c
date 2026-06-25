/*
 * LQR_PWM_ct.c
 *
 *  Created on: Apr 3, 2026
 *      Author: AORUS
 */


#include "LQR_PWM_ct.h"

extern HRTIM_HandleTypeDef hhrtim1;

// +Vref_ADC / 4095 = 3/4095
#define GAIN_iL1 	0.0222000f // (3/4095) * 1/0.033
#define GAIN_iL2 	0.0222000f // (3/4095) * 1/0.033
#define GAIN_vo 	0.0073260f  // (3/4095) * 10

#define i1_Offset 	49.0f; // 1.65/0.033
#define i2_Offset 	49.054f; // 1.65/0.033

#define VREF 12.0f
#define IREF 0.0f

#define DUTYMAX 0.9f
#define DUTYMIN 0.05f
#define MAX_TICKS 10880 // 0.9  * 13600 = 12240
#define MIN_TICKS 680	// 0.05 * 13600

#define TICK_TO_DUTY  0.00007352941f



// Gain Matrix
__attribute__((section(".ccmram.rodata")))const float K1_x_iL1=0.02984711f, K1_x_iL2=0.00229339f, K1_x_vc=0.01043646f, K1_d_u1=0.29967563f, K1_d_u2=0.02310254f, K1_e_vo=-0.00586658f, K1_e_iL=-0.00019042f;
__attribute__((section(".ccmram.rodata")))const float K2_x_iL1=0.00229339f, K2_x_iL2=0.02984711f, K2_x_vc=0.01043646f, K2_d_u1=0.02310254f, K2_d_u2=0.29967563f, K2_e_vo=-0.00586658f, K2_e_iL=0.00019042f;


typedef struct {
	float iL1_prev;
	float iL2_prev;
	float vo_prev;
	float u1_prev;
	float u2_prev;
	float u1_prev_2;
	float u2_prev_2;
	float e_vo;
	float e_iL;
} LQR_vars;

__attribute__((section(".ccmram.data"))) LQR_vars lqr = {0};

// Filter Variable
__attribute__((section(".ccmram.data"))) static float vo_filtered = VREF;
__attribute__((section(".ccmram.data"))) static float iL1_filtered = 0.0f;
__attribute__((section(".ccmram.data"))) static float iL2_filtered = 0.0f;


__attribute__((section(".ccmram.text"))) void LQR_execute(void) {

	GPIOD->BSRR = (1U << 2); // SET PD2

	float ADC_iL1 	= (float)(ADC1->JDR1); 	 	// Raw Data: Inductor-1's current
	float ADC_iL2 	= (float)(ADC2->JDR1);			// Raw Data: Inductor-2's current
	float ADC_vo 	= (float)(ADC3->JDR1);			// Raw Data: Output Voltage

	// Converts to Actual Data
	float iL1_now 	= (ADC_iL1 	* GAIN_iL1) - i1_Offset;
	float iL2_now 	= (ADC_iL2 	* GAIN_iL2) - i2_Offset;
	float vo_now  	=  ADC_vo  	* GAIN_vo;

	// y(n)	=	(a * x(n) )+ (1-a)(y(n-1))
	iL1_filtered = (0.35 * iL1_now) + (0.65 * iL1_filtered);
	iL2_filtered = (0.35 * iL2_now) + (0.65 * iL2_filtered);
	vo_filtered  = (0.25 * vo_now)  + (0.75 * vo_filtered);


	// Delta of State Variable
	float delta_iL1 	= 	iL1_filtered - lqr.iL1_prev;
	float delta_iL2 	= 	iL2_filtered - lqr.iL2_prev;
	float delta_vo 		= 	vo_filtered  - lqr.vo_prev;
	float delta_d_u1 	= 	lqr.u1_prev - lqr.u1_prev_2;
	float delta_d_u2 	= 	lqr.u2_prev - lqr.u2_prev_2;

	// Delta od Disturbance


	// Integral Action (Error)
	lqr.e_vo = VREF - vo_filtered;

	lqr.e_iL = IREF - (iL1_filtered  - iL2_filtered);


	float delta_u1_fb = -( (K1_x_iL1 * delta_iL1) + (K1_x_iL2 * delta_iL2) + (K1_x_vc * delta_vo)
						  +(K1_d_u1 * delta_d_u1 ) +  (K1_d_u2 * delta_d_u2)
						  +(K1_e_vo * lqr.e_vo) + (K1_e_iL * lqr.e_iL));

	float delta_u2_fb = -( (K2_x_iL1 * delta_iL1) + (K2_x_iL2 * delta_iL2) + (K2_x_vc * delta_vo)
						  +(K2_d_u1 * delta_d_u1 ) +  (K2_d_u2 * delta_d_u2)
						  +(K2_e_vo * lqr.e_vo) + (K2_e_iL * lqr.e_iL));



	int delta_ticks_1 = (int)(delta_u1_fb * 13600.0f);

//	if(delta_ticks_1 >= -1 && delta_ticks_1 <= 1) delta_ticks_1 = 0;

	int cmp1_val = hhrtim1.Instance->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP1xR	+  delta_ticks_1;

	if(cmp1_val > MAX_TICKS) cmp1_val = MAX_TICKS; else if(cmp1_val < MIN_TICKS) cmp1_val = MIN_TICKS;
	hhrtim1.Instance->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP1xR = cmp1_val;


	int delta_ticks_2 = (int)(delta_u2_fb * 13600.0f);

//	if(delta_ticks_2 >= -1 && delta_ticks_2 <= 1) delta_ticks_2 = 0;

	int cmp2_val = hhrtim1.Instance->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_F].CMP1xR	+  delta_ticks_2;


	if(cmp2_val > MAX_TICKS) cmp2_val = MAX_TICKS; else if(cmp2_val < MIN_TICKS) cmp2_val = MIN_TICKS;
	hhrtim1.Instance->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_F].CMP1xR = cmp2_val;


//	DAC1->DHR12R1 = (uint32_t)(1241.0f * i);

	GPIOD->BSRR = (1U << (2 + 16)); // RESET PD2


	// Shift State
	lqr.iL1_prev 	= iL1_filtered ;		// iL1(n-1) = iL1(n)
	lqr.iL2_prev 	= iL2_filtered ;		// iL2(n-1) = iL2(n)
	lqr.vo_prev     = vo_filtered;

	lqr.u1_prev_2	= lqr.u1_prev; 		// u1(n-2) =  u1(n-1)
	lqr.u2_prev_2	= lqr.u2_prev;		// u2(n-2) =  u2(n-1)
	lqr.u1_prev     = (float)cmp1_val * TICK_TO_DUTY;
	lqr.u2_prev     = (float)cmp2_val * TICK_TO_DUTY;

}

// Please Reset before LQR execute
void LQR_Reset(){
	lqr.iL1_prev 	= 0.0f;
	lqr.iL2_prev 	= 0.0f;
	lqr.vo_prev 	= 0.0f;
	lqr.u1_prev 	= (float)MIN_TICKS * TICK_TO_DUTY;
	lqr.u2_prev 	= (float)MIN_TICKS * TICK_TO_DUTY;
	lqr.u1_prev_2 	= (float)MIN_TICKS * TICK_TO_DUTY;
	lqr.u2_prev_2 	= (float)MIN_TICKS * TICK_TO_DUTY;
	lqr.e_vo		= 0.0f;;
	lqr.e_iL		= 0.0f;;

	vo_filtered = VREF;
	iL1_filtered = 0.0f;
	iL2_filtered = 0.0f;

	hhrtim1.Instance->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP1xR = MIN_TICKS;
	hhrtim1.Instance->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_F].CMP1xR = MIN_TICKS;
}

void LQR_Init() {

	LQR_Start();

}

void LQR_Start() {

	LQR_Reset();


	ADC3->IER &= ~ADC_IER_JEOCIE;

	HAL_HRTIM_WaveformCountStart_IT(&hhrtim1, HRTIM_TIMERID_MASTER|HRTIM_TIMERID_TIMER_A|HRTIM_TIMERID_TIMER_F|HRTIM_TIMERID_TIMER_E);
	HAL_HRTIM_WaveformOutputStart(&hhrtim1, HRTIM_OUTPUT_TA1|HRTIM_OUTPUT_TA2|HRTIM_OUTPUT_TF1|HRTIM_OUTPUT_TF2|HRTIM_OUTPUT_TE1);


	if (ADC3->ISR & ADC_ISR_JEOC) {
		ADC3->ISR |= ADC_ISR_JEOC; // Injected End of Conversion Flag
	}
	ADC3->IER |= ADC_IER_JEOCIE; // Injected End of Conversion Interrupt Enable

}


void LQR_Stop() {

	ADC3->IER &= ~ADC_IER_JEOCIE;

	HAL_HRTIM_WaveformOutputStop(&hhrtim1, HRTIM_OUTPUT_TA1|HRTIM_OUTPUT_TA2|HRTIM_OUTPUT_TF1|HRTIM_OUTPUT_TF2|HRTIM_OUTPUT_TE1);
	HAL_HRTIM_WaveformCountStop_IT(&hhrtim1, HRTIM_TIMERID_MASTER|HRTIM_TIMERID_TIMER_A|HRTIM_TIMERID_TIMER_F|HRTIM_TIMERID_TIMER_E);

	LQR_Reset();


}


void LQR_FaultStop() {

	ADC3->IER &= ~ADC_IER_JEOCIE;

	HAL_HRTIM_WaveformOutputStop(&hhrtim1, HRTIM_OUTPUT_TA1|HRTIM_OUTPUT_TA2|HRTIM_OUTPUT_TF1|HRTIM_OUTPUT_TF2|HRTIM_OUTPUT_TE1);
	HAL_HRTIM_WaveformCountStop_IT(&hhrtim1, HRTIM_TIMERID_MASTER|HRTIM_TIMERID_TIMER_A|HRTIM_TIMERID_TIMER_F|HRTIM_TIMERID_TIMER_E);

	LQR_Reset();

}
