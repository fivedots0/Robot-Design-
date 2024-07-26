#include "main.h"
#include <stdio.h>
#define true 1
#define false 0
#define bool uint8_t

uint16_t threshold_transform(uint16_t value){
	if(value>0xffff/2) {
		return 0xffff;
	}
	return 0;
}

void threshold(uint16_t* img, uint16_t width, uint16_t height) {
	for(uint16_t i = 0; i<width*height; i++) {
		*(img+i)=threshold_transform(*(img+i));
	}
}

uint16_t indexCal(uint8_t i, uint8_t j) {
	uint16_t tmp1 = i;
	uint16_t tmp2 = j;
	return tmp1*120+tmp2;
}

void sobelOperator(uint16_t *img, uint8_t width, uint8_t height){
	uint16_t tmp1[128] = {0};
	uint16_t tmp2[128] = {0};

	for(uint8_t i = 1; i<(height-1);i++){
		for(uint8_t j = 1; j<width-1; j++){
			int gx = *(img + indexCal(i-1,j-1))*-1
					+*(img + indexCal(i, j-1))*-2
					+*(img + indexCal(i+1,j-1))*-1
					+*(img + indexCal(i-1,j+1))
					+*(img + indexCal(i,j+1))*2
					+*(img + indexCal(i+1,j+1));

			int gy = *(img + indexCal(i-1, j-1))*-1
					+*(img + indexCal(i-1,j))*-2
					+*(img + indexCal(i-1,j+1))*-1
					+*(img + indexCal(i+1,j-1))*1
					+*(img + indexCal(i+1,j))*2
					+*(img + indexCal(i+1,j+1));
			if(gx<0) gx*=-1;
			if(gy<0) gy*=-1;
			uint16_t value = gx+gy;
			if(i%2==1){
				tmp1[j] = value;
			}else{
				tmp2[j] = value;
			}
			if(i>=2 && i%2==1){
				*(img + indexCal(i-2,j)) = tmp2[j]>0x8000?0xffff:0;
			}else if(i>=2){
				*(img + indexCal(i-2,j)) = tmp1[j]>0x8000?0xffff:0;
			}
		}

	}
}

uint8_t detectSensor(uint16_t *img, uint8_t startI, uint8_t startJ, uint8_t endI, uint8_t endJ) {
	uint8_t ans = 0;

	for(uint8_t i = startI; i<endI; i++) {
				for(uint8_t j = startJ; j<endJ; j++) {
					if(ans>40){
						break;
					}
					if(*(img+indexCal(i,j))==0xffff){
						ans++;
					}else{
						*(img+indexCal(i,j)) = 255;
					}
				}
			}
	return ans;
}

uint8_t lineDetection(uint16_t *img, uint8_t width, uint8_t height){
	uint8_t cnt[9] = {0,0,0,0,0,0,0,0,0,0};
	//for(uint8_t i = 0; i<10; i++){
		cnt[0] = detectSensor(img, 0, 0, 40, 40);
		cnt[1] = detectSensor(img, 0, 40, 40, 80);
		cnt[2] = detectSensor(img, 0, 80, 40, 120);
		cnt[3] = detectSensor(img, 40, 0, 80, 40);
		cnt[4] = detectSensor(img, 40, 40, 80, 80);
		cnt[5] = detectSensor(img, 40, 80, 80, 120);
		cnt[6] = detectSensor(img, 80, 0, 120, 40);
		cnt[7] = detectSensor(img, 80, 40, 120, 80);
		cnt[8] = detectSensor(img, 80, 80, 120, 120);

		// 8 7 6
		// 5 4 3
		// 2 1 0

		// 0 1 2
		uint8_t tmp[3] = {0,0,0};
		if(cnt[0]>=40) {
			tmp[2] = 0;
		}else if(cnt[3]>=40) {
			tmp[2] = 1;
		}else if(cnt[6]>=40) {
			tmp[2] = 2;
		}else{
			tmp[2] = 3;
		}

		if(cnt[1]>=40) {
			tmp[1] = 0;
		}else if(cnt[4]>=40) {
			tmp[1] = 1;
		}else if(cnt[7]>=40) {
			tmp[1] = 2;
		}else{
			tmp[1] = 3;
		}

		if(cnt[2]>=40) {
			tmp[0] = 0;
		}else if(cnt[5]>=40) {
			tmp[0] = 1;
		}else if(cnt[8]>=40) {
			tmp[0] = 2;
		}else{
			tmp[0] = 3;
		}

		uint8_t slope = 0;
		if(tmp[0]>tmp[2]) {
			return 90+22*(tmp[0]-tmp[2]);
		}else if(tmp[2]>tmp[0]){
			return 90;
		}else if(tmp[0]==tmp[1] && tmp[1]==tmp[2]){
			return 90;
		}else{
			return 30;
		}
	return 90;
}



uint8_t straightLine(uint16_t *img, uint8_t width, uint8_t height){

	uint8_t cnt[4] = {0,0,0,0};
	for(uint8_t i = 0; i<30; i++) {
			for(uint8_t j = 0; j<width; j++) {
				if(cnt[0]>40){
					break;
				}
				if(*(img+indexCal(i,j))==0xffff){
					cnt[0]++;
				}else{
					*(img+indexCal(i,j)) = 255;
				}
			}
		}
	for(uint8_t i = 30; i<60; i++) {
			for(uint8_t j = 0; j<width; j++) {
				if(cnt[1]>40){
									break;
								}
				if(*(img+indexCal(i,j))==0xffff){
					cnt[1]++;
				}else{
					*(img+indexCal(i,j)) = 500;
				}

			}
		}
	for(uint8_t i = 60; i<90; i++) {
				for(uint8_t j = 0; j<width; j++) {
					if(cnt[2]>40){
										break;
									}
					if(*(img+indexCal(i,j))==0xffff){
						cnt[2]++;
					}else{
						*(img+indexCal(i,j)) = 1000;
					}

				}
			}
	for(uint8_t i = 90; i<120; i++) {
				for(uint8_t j = 0; j<width; j++) {
					if(cnt[3]>40){
										break;
									}
					if(*(img+indexCal(i,j))==0xffff){
						cnt[3]++;
					}else{
						*(img+indexCal(i,j)) = 1500;
					}

				}
			}
	/*for(uint8_t i = height/2; i<height; i++) {
			for(uint8_t j = 0; j<width; j++) {
				if(*(img+indexCal(i,j))==0xffff){
					cnt2++;
				}
			}
		}*/
	//turnServoMotor(90);
	if(cnt[0]>40) {
		//turnServoMotor(170);
		//degree = 170;
		//updateDCMotorSpeed(160,0);
		//updateDCMotorSpeed(150,1);
		return 170;
		//TIM5->CCR1 = 980;
		//
	}else if(cnt[1]>50){
		//turnServoMotor(140);
		//updateDCMotorSpeed(155,0);
		//updateDCMotorSpeed(150,1);
		return 140;
		//degree = 140;
		//TIM5->CCR1 = 700;
		//updateDCMotorSpeed(205,2);
	}else if(cnt[2]>50){
		//turnServoMotor(60);
		//updateDCMotorSpeed(150,0);
		//updateDCMotorSpeed(155,1);
		return 60;
		//degree = 60;
		//TIM5->CCR1 = 500;
		//updateDCMotorSpeed(205,2);
	}else if(cnt[3]>50){
		//turnServoMotor(40);
		//updateDCMotorSpeed(150,0);
		//updateDCMotorSpeed(160,1);
		return 40;
		//degree = 40;
		//TIM5->CCR1 = 400;
		//updateDCMotorSpeed(205,2);
	}
	//updateDCMotorSpeed(150,0);
	//updateDCMotorSpeed(165,1);
	return 0;


}

uint8_t* pathFinder(uint16_t *img, uint8_t width, uint8_t height) {
	led_off(LED1);
	led_off(LED2);
	led_off(LED3);
	led_off(LED4);
	uint8_t startHeight = 20;
	bool sensor[6] = {0};
	uint8_t cnt = 0;
	static uint32_t lastTurnTick = 0;
	static uint8_t lastSensor = 5;
	for(uint8_t i = startHeight+85; i<startHeight+90; i++){
		for(uint8_t j = 105; j<110; j++) {
			if(*(img+indexCal(i,j))==0xffff){
				cnt++;
			}
			*(img+indexCal(i,j)) = 255;
		}
	}
	if(cnt>=5) {
		sensor[0]=1;
	}
	cnt= 0 ;
	for(uint8_t i = startHeight+50; i<startHeight+55; i++){
			for(uint8_t j = 105; j<110; j++) {
				if(*(img+indexCal(i,j))==0xffff){
					cnt++;
				}
				*(img+indexCal(i,j)) = 255;
			}
		}
	if(cnt>=5) {
			sensor[1]=1;
		}
	cnt= 0 ;
	for(uint8_t i = startHeight+85; i<startHeight+90; i++){
			for(uint8_t j = 60; j<65; j++) {
				if(*(img+indexCal(i,j))==0xffff){
					cnt++;
				}
				*(img+indexCal(i,j)) = 255;
			}
		}
	cnt= 0 ;
	if(cnt>=5) {
			sensor[2]=1;
		}
	for(uint8_t i = startHeight+50; i<startHeight+55; i++){
				for(uint8_t j = 60; j<65; j++) {
					if(*(img+indexCal(i,j))==0xffff){
						cnt++;
					}
					*(img+indexCal(i,j)) = 255;
				}
			}
	if(cnt>=5) {
			sensor[3]=1;
		}
		cnt= 0 ;
		for(uint8_t i = startHeight+85; i<startHeight+90; i++){
					for(uint8_t j = 20; j<25; j++) {
						if(*(img+indexCal(i,j))==0xffff){
							cnt++;
						}
						*(img+indexCal(i,j)) = 255;
					}
				}
		if(cnt>=5) {
				sensor[4]=1;
			}
			cnt= 0 ;
			for(uint8_t i = startHeight+50; i<startHeight+55; i++){
						for(uint8_t j = 20; j<25; j++) {
							if(*(img+indexCal(i,j))==0xffff){
								cnt++;
							}
							*(img+indexCal(i,j)) = 255;
						}
					}
			if(cnt>=5) {
					sensor[5]=1;
				}
				cnt= 0 ;

	if(sensor[0]==1 && sensor[5]==1) {
		turnServoMotor(140);
	}else if(sensor[1]==1 && sensor[4]==1){
		turnServoMotor(40);
	}
	//turnServoMotor(120);
	return 0;
}

void autoBrightness(uint16_t *img, uint16_t width, uint16_t height) {
	//uint16_t samplePixel = 10000;
	uint16_t shift = 3000;
	for(uint16_t i = 0; i<width*height; i++) {
		if(*(img+i)<0xffff-shift){
			*(img+i) += shift;
		}
	}
}

void autoContrast(uint16_t *img, uint16_t width, uint16_t height) {
	//uint16_t samplePixel = *(img+height/2*width+width/2);
	uint16_t shift = 9000;
	for(uint16_t i = 0; i<width*height; i++) {
		uint16_t pixel = *(img+i);
		if(pixel>0xffff/2){
			if(*(img+i)<0xffff-shift){
				*(img+i) = pixel+shift;
			}else{
				*(img+i) = 0xffff;
			}
		}else if(pixel<0xffff/2){
			if(pixel>shift){
				*(img+i) = pixel-shift;
			}else{
				*(img+i) = 0;
			}
		}
	}
}

void change_contrast(uint16_t* Image, uint16_t height, uint16_t width, uint16_t contrast){
    for(uint16_t i = 0; i < height; i++) {
        for (uint16_t j = 0; j < width; j++) {
            uint16_t value = (*(Image + width*i + j) - 128) * contrast + 128;
            if(0 <= value <= 255){
                *(Image + width*i + j) = value;
            }
            if(value >= 255){
                *(Image + width*i + j) = 255;
            }
            if(0 >= value){
                *(Image + width*i + j) = 0;
            }

        }
    }
}


// the auto correct function
void autocorrect(uint16_t* Image, uint16_t height, uint16_t width){
    uint16_t shift =  128 - *(Image + height*height/2 + width/2); // the center pixel of the image
    change_brightness(Image, height, width, shift); // change brightness to around 128
    uint16_t max = 0;
    for(uint16_t i = 0; i < height; i++) {
        for (uint16_t j = 0; j < width; j++) {
            if(*(Image + width*i + j) > max){
                max = *(Image + width*i + j);
            }
        }
    }
    float contrast = 127/(max-128); // maximize contrast
    change_contrast(Image, height, width, contrast);
}
