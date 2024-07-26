#include "main.h"
#include "stdio.h"
#include "camera/camera.h"
#define true 1
#define false 0
#define bool uint8_t

void arc_camera_atr_init(void) {
	cam_set_state(CAM_CAPTURING);
	cam_set_colormode(CAM_GRAYSCALE);
	//cam_set_zoomscale(1.7f);
	//cam_set_framesize(QQVGA_120x160);
	//cam_set_frameoffset(40,0);
	//cam_set_saturation(0);
	//cam_set_brightness(0);
	//cam_set_contrast(0);
	//cam_set_framerate(75);
	//cam_set_colormode(CAM_GRAYSCALE);
}

