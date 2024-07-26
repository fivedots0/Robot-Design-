#include "main.h"
#include "lcd/lcd.h"
#include "lcd/lcd_graphics.h"

/** Design ideologies:
 * More small functions >>> Few giant functions
 * Reusability and Extendability is most important (Dont-Repeat-Yourself)
 * then Readability (comments are good, self documenting code is best)
 * No "magic numbers", use defines, constants, variables etc
 * ALWAYS INDENT CORRECTLY
 * No blocking (long while loops, HAL_Delay)
 */

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

ButtonState btn_state(void) {
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

/* Private variables START */

ButtonState state = ALL_OFF;

/* Private variables END */

/* Private function prototypes START */

/* Private function prototypes END */
#define false 0
#define true 1
void gpio_classwork(void) {
    /* Your code start here */
	static uint32_t last_ticks = 0;
	static uint32_t BTN2_last_toggle_ticks = 0;
	static uint8_t BTN2_is_toggle = false;

	static uint8_t both_btn_state = 0;
	static uint32_t both_btn_last_tick = 0;
	uint32_t tmp_tick = HAL_GetTick();
	if(btn_state() == BTN1_ONLY) {

				gpio_set(LED2);
				gpio_set(LED3);
		gpio_reset(LED1);
	}else if(btn_state() == BTN2_ONLY){
		gpio_set(LED1);
				gpio_set(LED3);
		if(!BTN2_is_toggle) {
			BTN2_last_toggle_ticks = tmp_tick;
			BTN2_is_toggle = true;
		}
		if((tmp_tick - BTN2_last_toggle_ticks)%50 == 0) {
				gpio_toggle(LED2);
			}
		//flash(LED2, BTN2_last_toggle_ticks, tmp_tick);
	}else if(btn_state()==BOTH_BTN) {
		if(both_btn_state == 0 || both_btn_state == 1){
			if(both_btn_state == 0) {
				gpio_set(LED1);
				gpio_set(LED2);
				gpio_set(LED3);
				both_btn_last_tick = tmp_tick;
				both_btn_state++;
			}
			if(both_btn_state == 1 && tmp_tick - both_btn_last_tick>=1000) {
				both_btn_state = 2;
			}
			gpio_reset(LED1);
			gpio_reset(LED3);
			if((tmp_tick - both_btn_last_tick)%50 == 0) {
							gpio_toggle(LED2);
						}
		}if(both_btn_state == 2 || both_btn_state == 3) {
			if(both_btn_state == 2) {
				gpio_set(LED1);
				gpio_set(LED2);
				gpio_set(LED3);
				both_btn_last_tick = tmp_tick;
				both_btn_state++;
			}
			if(both_btn_state == 3 && tmp_tick - both_btn_last_tick>=1000) {
				both_btn_state = 0;
			}
			if((tmp_tick - both_btn_last_tick)%50 == 0) {
										gpio_toggle(LED1);
									}
			if((tmp_tick - both_btn_last_tick)%50 == 0) {
										gpio_toggle(LED3);
									}
			//flash(LED1, both_btn_last_tick, tmp_tick);
			//flash(LED3, both_btn_last_tick, tmp_tick);
			gpio_reset(LED2);

		}
	}else{
		gpio_set(LED1);
		gpio_set(LED2);
		gpio_set(LED3);
		BTN2_is_toggle = false;
		both_btn_state = 0;
	}
	last_ticks = HAL_GetTick();
    /* Your code end here */
}

#define WHITE           ((0xFFFFFF))
#define BLACK           ((0x000000))
#define DARK_GREY       ((0x555555))
#define GREY            ((0xAAAAAA))
#define RED             ((0xFF0000))
#define DARK_RED        ((0x800000))
#define ORANGE          ((0xFF9900))
#define YELLOW          ((0xFFFF00))
#define GREEN           ((0x00FF00))
#define DARK_GREEN      ((0x00CC00))
#define BLUE            (RGB888TO565(0x0000FF))
#define BLUE2           (RGB888TO565(0x202060))
#define SKY_BLUE        (RGB888TO565(0x11CFFF))
#define CYAN            (RGB888TO565(0x8888FF))
#define PURPLE          (RGB888TO565(0x00AAAA))
#define PINK            (RGB888TO565(0xC71585))

void tft_classwork(void) {
	static uint32_t color[10] = {0xFFFFFF, 0x000000, 0x555555,0xAAAAAA,0xFF0000,0x800000,0xFF9900,0xFFFF00,0x00FF00,0x00CC00};
    /* Your code start here */
	static last_tick = 0;
	static color_index = 0;
	uint32_t tmp_tick = HAL_GetTick();
	uint8_t ms = tmp_tick%1000;
	uint8_t second = tmp_tick/1000%60;
	uint8_t minute = tmp_tick/1000/60;
	tft_prints(0,0, "%d:%d:%d",minute,second,ms);
	if(tmp_tick - last_tick >=1000 ){
		last_tick = tmp_tick;
		color_index ++;
	}
	for(uint8_t i = 1; i<=50; i++ ){
				drawLine(0,i+15,50,i+15,RGB888TO565(color[color_index]));
			}
    /* Your code end here */
}

void tutorial2_homework(void) {
    /* Your code start here */

		q2();

    /* Your code end here*/
}

// You can define your helper functions down below
// You can also modify the function definition above as long as your move is reasonable
void q1(void){
	static uint32_t last_flash_tick = 0;
	uint32_t tmp = HAL_GetTick();
	if(btn_state()==BTN1_ONLY){
		gpio_set(LED1);
		tft_prints(0,0,"Hello, Wong Wing Him");
	}else if(tmp - last_flash_tick >=50){
		gpio_toggle(LED1);
		last_flash_tick = tmp;
	}
}

void q2(void) {
	static uint32_t btn_press_start_tick = 0;
	static uint8_t status = 0; // 0 = nothing, 1 = pressed, 2 = released
	static uint32_t text_print_start_tick = 0;
	static uint8_t print_text = false;
	static uint32_t flash_start_tick = 0;
	static uint8_t flash_LED = false;
	uint32_t tmp = HAL_GetTick();
	if(print_text) {
		if(tmp - text_print_start_tick >=1000) {
			print_text = false;
		}else{
			tft_prints(0,0,"Hello, Wong Wing Him");
		}
	}

	if(flash_LED) {
		if(tmp - flash_start_tick >=1000) {
			flash_LED = false;
		}else if((tmp - flash_start_tick)%50==0){
			gpio_toggle(LED1);
		}
	}else{
		gpio_set(LED1);
	}
	if(status == 0){
		if(btn_state()==BTN1_ONLY){
			status = 1;
			btn_press_start_tick = tmp;
			print_text = true;
											text_print_start_tick = tmp;
		}
	}else if(status == 1) {
		if(btn_state()==ALL_OFF){
			status = 2;

		}
	}else if(status == 2) {
		//if(tmp - btn_press_start_tick <=200){
	//		print_text = true;
	//		text_print_start_tick = tmp;
	//	}
		//print_text = true;
		//			text_print_start_tick = tmp;
		status = 0;
		flash_LED = true;
		flash_start_tick = tmp;
	}
}

void bonus(void) {
	static uint8_t r = 5;
	static uint8_t x = MAX_WIDTH/2-5;
	static uint8_t y = MAX_HEIGHT/2+5;
	static uint8_t btn1_state = false;
	static uint8_t btn2_state = false;
	static uint32_t btn1_press_time = 0;
	static uint32_t btn2_press_time = 0;
	uint32_t tmp = HAL_GetTick();
	drawCircle(x,y,r,RGB888TO565(0x0000FF));
	drawCircle(x+2,y+2,r,RGB888TO565(0x0000FF));
	if(btn1_state == false && btn_state()==BTN1_ONLY) {
		btn1_state = true;
		btn1_press_time = tmp;
	}else if(btn1_state == true && btn_state()==ALL_OFF) {
		if(tmp - btn1_press_time >= 300 && y-CHAR_HEIGHT>=0) {
			y-=CHAR_HEIGHT;
		}else if(tmp - btn1_press_time <300 && x-CHAR_WIDTH >=0 ){
			x-=CHAR_WIDTH;
		}
		btn1_state = false;
		tft_force_clear();
	}

	if(btn2_state == false && btn_state()==BTN2_ONLY) {
			btn2_state = true;
			btn2_press_time = tmp;
		}else if(btn2_state == true && btn_state()==ALL_OFF) {
			if(tmp - btn2_press_time >= 300 && y+CHAR_HEIGHT<MAX_HEIGHT) {
				y+=CHAR_HEIGHT;
			}else if(tmp - btn2_press_time <300 && x+CHAR_WIDTH <MAX_WIDTH ){
				x+=CHAR_WIDTH;
			}
			btn2_state = false;
			tft_force_clear();
		}
}
