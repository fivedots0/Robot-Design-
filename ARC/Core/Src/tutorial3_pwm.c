#include "main.h"
#include "tim.h"


/* Private variables END */

/* Private function prototypes START */

/* Private function prototypes END */

// Enums are always good
typedef enum {
    BOTH_BTN, BTN1_ONLY, BTN2_ONLY, ALL_OFF,
} ButtonState;

typedef enum {
    BUTTON1, BUTTON2,

    //This trick means the NumButtons will equal how many buttons there are (how many enum values)
    //This works as long as the enum values are simple (count up from 0)
    NUM_BUTTON,
} Button;

typedef enum {
    LED1, LED2, LED3, LED4, NUM_LED,
} LED;

/**
 * @brief read the button state
 * return 1 if the button is pressed;
 * return 0 if the button is released;
 */
static inline uint8_t read_button(Button btn) {
    switch (btn) {
    case BUTTON1:
        return !btn_read(BTN1);
    case BUTTON2:
        return !btn_read(BTN2);
    default:
        return 0;
    }
}

static ButtonState btn_state(void) {
    if (read_button(BUTTON1) && read_button(BUTTON2)) {
        return BOTH_BTN;
    } else if (read_button(BUTTON1)) {
        return BTN1_ONLY;
    } else if (read_button(BUTTON2)) {
        return BTN2_ONLY;
    } else {
        return ALL_OFF;
    }
}

void turn(uint8_t degree) {
	if(degree==0) {
		degree = 1;
	}
	TIM5->CCR1 = 300+degree*5+degree/2;
}

void pwm_init(void) {
	// init the pwm prescaler value and auto-reload value and start the pwm
	/* Your code start here */
	TIM5->ARR = 14999;
	TIM5 -> PSC = 111;
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
	//TIM5->CCR1 = 1300;
	//turn(170);
/*
	TIM4->ARR = 149;
	TIM4 -> PSC = 55;
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	TIM4->CCR3 = 140;
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
	TIM4->CCR4 = 140;
*/
	/* Your code end here */
}

#define false 0
#define true 1

void pwm_test(void){
	if(btn_state()==BTN1_ONLY) {
				TIM5->CCR1 = 2000;
			}
			if(btn_state()==BTN2_ONLY) {
				TIM5->CCR1 = 300;
			}
}

void pwm_classwork(void) {
	/* Your code start here */
	/*static uint32_t lastTick = 0;
	uint32_t tmp = HAL_GetTick();
	static uint8_t status = 0;
	static uint8_t isPressed = false;
	if(status == 0 && tmp - lastTick>=1000) {
		lastTick = tmp;
		status = 1;
		TIM5->CCR1 = 749;
	}else if(status == 1 && tmp - lastTick >=1000) {
		lastTick = tmp;
		status = 2;
		TIM5->CCR1 = 1124;
	}else if(status == 2 && tmp - lastTick >=1000) {
		lastTick = tmp;
		status = 3;
		TIM5->CCR1 = 1499;
	}else if(status == 3 && tmp - lastTick >=1000) {
		lastTick = tmp;
		status = 0;
		TIM5->CCR1 = 374;
	}*/

	static uint32_t lastTick = 0;
		uint32_t tmp = HAL_GetTick();
		static uint8_t status = 0;
		static uint8_t isPressed = false;
		static uint8_t isReleased = true;
		if(isPressed == false && btn_state()==BTN1_ONLY) {
			isPressed = true;
		}if(isPressed == true && btn_state()==ALL_OFF) {
			isPressed = false;
		}

		if(status == 0 && isPressed && tmp - lastTick>=1000) {
			lastTick = tmp;
			status = 1;
			TIM5->CCR1 = 749;
		}else if(status == 1 && isPressed && tmp - lastTick>=1000) {
			lastTick = tmp;
			status = 2;
			TIM5->CCR1 = 1124;
		}else if(status == 2 && isPressed && tmp - lastTick>=1000) {
			lastTick = tmp;
			status = 3;
			TIM5->CCR1 = 1499;
		}else if(status == 3 && isPressed && tmp - lastTick>=1000) {
			lastTick = tmp;
			status = 0;
			TIM5->CCR1 = 374;
		}

	//tft_prints(0,0,"%d", status);
	//TIM5->CCR1 = 749;
	//TIM5->CCR1 = 1124;
	//TIM5->CCR1 = 1499;
	/* Your code end here */
}

uint8_t isWin(uint8_t marbles[]) {
	for(uint8_t i = 0; i<6; i++) {
		if(marbles[i]==0) {
			return false;
		}
	}
	return true;
}

void printAllMarbles(uint8_t marbles[]) {
	//tft_force_clear();
	uint8_t index = 1;
	for(uint8_t i = 0; i<6; i++) {
		if(marbles[i] > 0) {
			tft_prints(0,i+2,"marble %d: %d", index++, marbles[i]);
		}
	}
}

void updateAllMarblesLevel(uint8_t *marbles, const uint8_t speed, uint8_t *marblesStatus,uint8_t TOPLevel){
	for(uint8_t i = 0; i<6; i++) {

		//marbles[i]--;
		if(marbles[i]==0) {
			marbles[i] = 0;
			marblesStatus[i] = 0;
		}
		if(marblesStatus[i]>1){
			marbles[i]+=speed;
			marblesStatus[i]--;
		}if(marblesStatus[i]==1) {
			marbles[i]-=speed;
		}
		if(marbles[i]>TOPLevel){
			marbles[i] = TOPLevel;
		}
	}
}

#define left 0
#define right 1
uint8_t degreeToCCR(uint8_t degree) {
	return (1.5+0.005*degree)/1000*50*(TIM5->ARR + 1 )-1;
}

uint8_t toDegree(uint8_t degree) {
	TIM5->CCR1 = degreeToCCR(degree);
}

void addMarbles(uint8_t *marbles, uint8_t *marblesStatus, uint8_t cnt){
	for(uint8_t i = 0; i<6; i++) {
		if(cnt==0) break;
		if(marbles[i]==0) {
			cnt--;
			marbles[i] = 40;
			marblesStatus[i] =1;
		}
	}
}

void hit(uint8_t *marbles, uint8_t *marblesStatus){
	for(int i = 0; i<6; i++) {
		if(marbles[i]>=5 && marbles[i]<=10) {
			marblesStatus[i] = 5;
		}
	}
}


void pwm_homework(void) {
	static uint32_t updateTimeEveryMs = 500;
	static uint8_t TOPLevel = 50;
	static uint8_t speed = 1;
	static uint32_t lastUpdateTick = 0;
	static uint8_t marbles[6] = {38,30,50,0,0,0};
	static uint8_t marblesStatus[6] = {1,1,1,0,0,0};
	static uint8_t trigger = false;
	static uint32_t lastTriggerTick = 0;

	static uint8_t btn1IsPressed = false;
	static uint32_t btn1LastPressTick = 0;

	static uint8_t btn2IsPressed = false;
	static uint32_t btn2LastPressTick = 0;

	static uint8_t paddleDirection = right;
	static uint8_t isDoubleMove = false;

	static uint8_t singleMoveCount = 0;
	static triggerStage = 0;

	uint32_t tmp_tick = HAL_GetTick();
	tft_prints(0,0,"%d", degreeToCCR(90));
	if(isWin(marbles)) {
		tft_prints(0,0,"Congratulations! You Won!");
		tft_prints(0,2," You Won!");
		return;
	}
	tft_prints(0,0,"Double Move: %d", singleMoveCount/2);
	if(tmp_tick - lastUpdateTick >=updateTimeEveryMs) {
			lastUpdateTick = tmp_tick;
			updateAllMarblesLevel(marbles, speed, marblesStatus, TOPLevel);
	}
	printAllMarbles(marbles);
	if(!btn1IsPressed && !trigger && btn_state()==BTN1_ONLY) {
		btn1LastPressTick = tmp_tick;
		btn1IsPressed = true;
	}
	static uint8_t specialMove = false;


	if(btn1IsPressed) {
		uint32_t tmp = tmp_tick - btn1LastPressTick;
		if(tmp >= 500) {
					gpio_reset(LED4);
				}
				if(tmp>=1000) {
					gpio_reset(LED3);
				}
				if(tmp>=1500) {
					gpio_reset(LED2);
				}
				if(tmp>=2000){
					gpio_reset(LED1);
				}
	}
	if( btn1IsPressed && btn_state()==ALL_OFF) {
		if(tmp_tick - btn1LastPressTick >=1500){
			btn1IsPressed = false;
			specialMove = true;
		}else{
			btn1IsPressed = false;
					trigger = true;
					singleMoveCount++;
		}

	}


	if(!btn2IsPressed && !trigger && btn_state()==BTN2_ONLY && singleMoveCount>=2) {
		singleMoveCount-=2;
		gpio_set(LED1);gpio_set(LED2);gpio_set(LED3);gpio_set(LED4);
		btn2LastPressTick = tmp_tick;
		btn2IsPressed = true;
			TIM5->CCR1 = 1499;

	}

	if(btn2IsPressed) {
		uint32_t tmp = tmp_tick - btn2LastPressTick;
		if(btn_state()==ALL_OFF) {
			btn2IsPressed = false;
			if(tmp>=2000){
				addMarbles(marbles, marblesStatus, 2);
				isDoubleMove = true;
			}
			trigger = true;
			gpio_set(LED1);gpio_set(LED2);gpio_set(LED3);gpio_set(LED4);
		}

		if(tmp >= 500) {
			gpio_reset(LED1);
		}
		if(tmp>=1000) {
			gpio_reset(LED2);
		}
		if(tmp>=1500) {
			gpio_reset(LED3);
		}
		if(tmp>=2000){
			gpio_reset(LED4);
		}
	}

	if(specialMove){
		if( triggerStage == 0 && tmp_tick - lastTriggerTick >= updateTimeEveryMs) {
			gpio_set(LED1);gpio_set(LED2);gpio_set(LED3);gpio_set(LED4);
					hit(marbles, marblesStatus);
					lastTriggerTick = tmp_tick;
					TIM5->CCR1 = 600;
					triggerStage++;
					gpio_reset(LED3);
				}else if(triggerStage==1 && tmp_tick - lastTriggerTick >= updateTimeEveryMs){
					lastTriggerTick = tmp_tick;

						TIM5->CCR1 = 374;

					triggerStage++;
					gpio_reset(LED4);
				}else if(triggerStage==2 && tmp_tick - lastTriggerTick >= updateTimeEveryMs) {
					hit(marbles, marblesStatus);
					lastTriggerTick = tmp_tick;
					TIM5->CCR1 = 600;
					triggerStage++;
					gpio_reset(LED2);

				}else if(triggerStage==3 && tmp_tick - lastTriggerTick >= updateTimeEveryMs){
					lastTriggerTick = tmp_tick;

						TIM5->CCR1 = 374;

					triggerStage++;
					gpio_reset(LED4);
				}else if(triggerStage==4 && tmp_tick - lastTriggerTick >= updateTimeEveryMs) {
					lastTriggerTick = tmp_tick;
					TIM5->CCR1 = 600;
					triggerStage++;
					gpio_reset(LED2);
				}else if(triggerStage==5 && tmp_tick - lastTriggerTick >= updateTimeEveryMs){

									TIM5->CCR1 = 1900;

								triggerStage = 0;
								trigger = false;
								isDoubleMove = false;
								gpio_set(LED1);gpio_set(LED2);gpio_set(LED3);gpio_set(LED4);
								specialMove = false;
				}

	}

	if(trigger && !isDoubleMove) {
		if( triggerStage == 0 && tmp_tick - lastTriggerTick >= updateTimeEveryMs) {
			gpio_set(LED1);gpio_set(LED2);gpio_set(LED3);gpio_set(LED4);
			hit(marbles, marblesStatus);
			lastTriggerTick = tmp_tick;
			TIM5->CCR1 = 374;
			triggerStage++;
		}else if(triggerStage==1 && tmp_tick - lastTriggerTick >= updateTimeEveryMs*2){

			TIM5->CCR1 = 1900;
			triggerStage = 0;
			trigger = false;
		}
	}else if(trigger && isDoubleMove){
		if( triggerStage == 0 && tmp_tick - lastTriggerTick >= updateTimeEveryMs) {
			hit(marbles, marblesStatus);
			lastTriggerTick = tmp_tick;
			TIM5->CCR1 = 374;
			triggerStage++;
		}else if(triggerStage==1 && tmp_tick - lastTriggerTick >= updateTimeEveryMs+300){
			lastTriggerTick = tmp_tick;

				TIM5->CCR1 = 1900;

			triggerStage++;
		}else if(triggerStage==2 && tmp_tick - lastTriggerTick >= updateTimeEveryMs) {
			hit(marbles, marblesStatus);
			lastTriggerTick = tmp_tick;
			TIM5->CCR1 = 374;
			triggerStage++;
		}else if(triggerStage==3 && tmp_tick - lastTriggerTick >= updateTimeEveryMs+300){

							TIM5->CCR1 = 1900;

						triggerStage = 0;
						trigger = false;
						isDoubleMove = false;
						gpio_set(LED1);gpio_set(LED2);gpio_set(LED3);gpio_set(LED4);
		}

	}

}
//1900
//1499 = -90
//600 = 0
//374

/*
void pwm_homework(void) {

	//BTN1 = single, BTN2 = double
	static uint8_t TOPLevel = 50;
	static uint8_t speed = 1;
	static uint8_t updateTimeEveryMs = 100;
	static uint32_t lastUpdateTick = 0;
	static uint8_t marbles[6] = {50,50,50,0,0,0};
	static uint8_t marblesStatus[6] = {-1,-1,-1,0,0,0};
	static uint8_t marblesAmount = 3;
	static uint8_t isDoubleMove = false;

	static uint32_t btn2LastPressTick = 0;
	static uint8_t btn2IsPressed = false;

	static uint32_t btn1LastPressTick = 0;
	static uint8_t btn1IsPressed = false;

	static uint8_t paddleDirection = left;

	static uint8_t trigger = false;
	static uint32_t lastTriggerTime = 0;
	static uint8_t moving = false;
	uint8_t tmp_tick = HAL_GetTick();

//	tft_force_clear();


	if(isWin(marbles)) {
			tft_prints(10,75,"Congratulations! You Won!");
			return;
	}
	if(tmp_tick - lastUpdateTick >=updateTimeEveryMs) {
		lastUpdateTick = tmp_tick;
		updateAllMarblesLevel(marbles, speed, marblesStatus, TOPLevel);
	}


	printAllMarbles(marbles);

	if(!trigger && btn_state()==BTN1_ONLY) {
		trigger = true;
		btn1IsPressed = true;
		btn1LastPressTick = tmp_tick;
	}

	if(!trigger && btn_state() == BTN2_ONLY) {
		btn2IsPressed = true;
		btn2LastPressTick = tmp_tick;
	}

	if(btn1IsPressed && btn_state()==ALL_OFF) {
		btn1IsPressed = false;
	}

	if(btn2IsPressed && btn_state() == ALL_OFF) {
		btn2IsPressed = false;
		trigger = true;
	}else if(btn2IsPressed && tmp_tick - btn2LastPressTick >=500) {
		if(paddleDirection==left) {
			TIM5->CCR1 = degreeToCCR(40);
		}else{
			TIM5->CCR1 = degreeToCCR(140);
		}
		gpio_reset(LED1);
	}else if(btn2IsPressed && tmp_tick - btn2LastPressTick >=1000) {
		gpio_reset(LED2);
	}else if(btn2IsPressed && tmp_tick - btn2LastPressTick >=1500) {
		gpio_reset(LED3);
	}else if(btn2IsPressed && tmp_tick - btn2LastPressTick >=2000) {
		gpio_reset(LED4);
		isDoubleMove = true;
	}

	if(trigger && tmp_tick - lastTriggerTime >=100){
		lastTriggerTime = tmp_tick;
		if(paddleDirection==left) {
			TIM5->CCR1 = TIM5->CCR1 +degreeToCCR(1);
		}else{
			TIM5->CCR1 = TIM5->CCR1 -degreeToCCR(1);
		}
	}

	if(TIM5->CCR1>=1000 && TIM5->CCR1<=1200) {
		if(paddleDirection==left){
			TIM5->CCR1 = degreeToCCR(180);
			paddleDirection = right;
		}else{
			TIM5->CCR1 = degreeToCCR(0);
			paddleDirection = left;
		}
		hit(marbles, marblesStatus);
		btn1IsPressed = false;
		trigger = false;
		if(isDoubleMove) {
			trigger  = true;
			if(paddleDirection==left) {
				TIM5->CCR1 = degreeToCCR(40);
			}else{
				TIM5->CCR1 = degreeToCCR(140);
			}
			isDoubleMove = false;
		}

	}*/
	/* Your code end here*/

