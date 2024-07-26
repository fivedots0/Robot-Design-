#include "main.h"
#include "tim.h"

typedef enum {
    LEFT, RIGHT, BOTH,
} DCMotor;

void turnServoMotor(uint8_t);
void initServoMotor(void);
void initDCMotor(void);
void updateDCMotorSpeed(uint16_t, DCMotor);


void initWheels(void){
	initServoMotor();
	initDCMotor();
}

//void initServoMotor(void) {
//	TIM5->ARR = 14999;
//	TIM5 -> PSC = 111;
//	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
//	TIM5->CCR1 = 320;
//	//turnServoMotor(90);
//}

void initServoMotor(void) {
	TIM5->ARR = 14999;
	TIM5 -> PSC = 111;
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
	//TIM5->CCR1 = 320;
	turnServoMotor(90);
}

void turnServoMotor(uint8_t degree) {
	uint16_t ans = degree;
	if(degree==0) {
		ans = 1;
	}
	TIM5->CCR1 = 320+degree*5+degree/2;
}

void updateWheels(){
	updateServoMotor();
}

void updateServoMotor(uint8_t degree){
	turnServoMotor(degree);
}

void initDCMotor(void) {
	TIM4->ARR = 199;
	TIM4 -> PSC = 41;
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
}

void testMotor(){
	updateDCMotorSpeed(140,BOTH);
}
void updateDCMotorSpeed(uint16_t speed, DCMotor option) {
	if(option == LEFT) {
		TIM4->CCR3 = speed;
	}else if(option == RIGHT) {
		TIM4->CCR4 = speed;
	}else if(option == BOTH){
		TIM4->CCR3 = speed;
		TIM4->CCR4 = speed;
	}

}
