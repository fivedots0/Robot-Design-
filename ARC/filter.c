#include <math.h>
#include <string.h>
#include <stdio.h>

/*
I try to make a file containing all filters
But errors pop out in line 57, 83, 110 telling me that 'subscripted value is neither array nor pointer nor vector'.
for reference, those lines are the same and they are "new_value += window[i] * filter[i];".
*/


void mean_filter(uint16_t* Image, int height, int width){
    for(int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
		
		int value = 0;
		for (int u=i-1; u<=i+1; u++) {
                	for (int v=j-1; v<=j+1; v++) {
                    		value += *(Image + width * u + v);
                	}
           	 }
		*(Image + width * i + j) = value;
            
        }
    }
}

void GaussianFilter_3x3(const uint8_t *cameraDataPtr, uint8_t *processedDataPtr, uint16_t width, uint16_t height) {
    uint8_t window[9];
    int8_t filter = {1,2,1,2,4,2,1,2,1};
    // Loop though entire image array
    // ignored the outer edge of the image to simplify the code
    for (uint16_t x=1; x<width-1; x++) {
        for (uint16_t y=1; y<height-1; y++) {
            // Fill the 3x3 window
	    uint8_t k = 0;
	    int16_t new_value = 0;
            for (int u=x-1; u<=x+1; u++) {
                for (int v=y-1; v<=y+1; v++) {
                    window[k++] = *(cameraDataPtr + width * v + u);
                }
            }

            // Find the Gaussian mean
            for(int i = 0; i < 9; ++i){
            	new_value += window[i] * filter[i];
            }
            // Assign the median to the processed image array
            *(processedDataPtr + width * y + x) = new_value/16;
        }
    }
}


void medianFilter_3x3(const uint8_t *cameraDataPtr, uint8_t *processedDataPtr, uint16_t width, uint16_t height) {
    uint8_t window[9];
    // Loop though entire image array
    // ignored the outer edge of the image to simplify the code
    for (uint16_t x=1; x<width-1; x++) {
        for (uint16_t y=1; y<height-1; y++) {
            // Fill the 3x3 window
	    uint8_t k = 0;
            for (int u=x-1; u<=x+1; u++) {
                for (int v=y-1; v<=y+1; v++) {
                    window[k++] = *(cameraDataPtr + width * v + u);
                }
            }
            // Find the median
            for(uint8_t i=0; i<5; i++) {
                for (uint8_t j=i+1; j<9; j++) {
                    if (window[j] < window[i]) {
                        uint8_t temp = window[i];
                        window[i] = window[j];
                        window[j] = temp;
                    }
                }
            }
            // Assign the median to the processed image array
            *(processedDataPtr + width * y + x) = window[4];
        }
    }
}

void GaussianFilter_3x3(const uint8_t *cameraDataPtr, uint8_t *processedDataPtr, uint16_t width, uint16_t height) {
    uint8_t window[9];
    int8_t filter = {1,2,1,2,4,2,1,2,1};
    // Loop though entire image array
    // ignored the outer edge of the image to simplify the code
    for (uint16_t x=1; x<width-1; x++) {
        for (uint16_t y=1; y<height-1; y++) {
            // Fill the 3x3 window
	    uint8_t k = 0;
	    int16_t new_value = 0;
            for (int u=x-1; u<=x+1; u++) {
                for (int v=y-1; v<=y+1; v++) {
                    window[k++] = *(cameraDataPtr + width * v + u);
                }
            }

            // Find the Gaussian mean
            for(int i = 0; i < 9; ++i){
            	new_value += window[i] * filter[i];
            }
            // Assign the median to the processed image array
            *(processedDataPtr + width * y + x) = new_value/16;
        }
    }
}

void Sobel_x_Filter_3x3(const uint8_t *cameraDataPtr, uint8_t *processedDataPtr, uint16_t width, uint16_t height) {
    uint8_t window[9];
    int8_t filter = {-1,0,1,-2,0,2,-1,0,1};
    // Loop though entire image array
    // ignored the outer edge of the image to simplify the code
    for (uint16_t x=1; x<width-1; x++) {
        for (uint16_t y=1; y<height-1; y++) {
            // Fill the 3x3 window
	    uint8_t k = 0;
	    int16_t new_value = 0;
            for (int u=x-1; u<=x+1; u++) {
                for (int v=y-1; v<=y+1; v++) {
                    window[k++] = *(cameraDataPtr + width * v + u);
                }
            }
            // Find the value

            for(int i = 0; i < 9; ++i){
            	new_value += window[i] * filter[i];
            }
            // Assign the median to the processed image array
            *(processedDataPtr + width * y + x) = new_value;
        }
    }
}

void Sobel_y_Filter_3x3(const uint8_t *cameraDataPtr, uint8_t *processedDataPtr, uint16_t width, uint16_t height) {
    uint8_t window[9];

    int8_t filter = {-1,-2,-1,0,0,0,1,2,1};
    // Loop though entire image array
    // ignored the outer edge of the image to simplify the code
    for (uint16_t x=1; x<width-1; x++) {
        for (uint16_t y=1; y<height-1; y++) {
            // Fill the 3x3 window
	    uint8_t k = 0;
	    int16_t new_value = 0;
            for (int u=x-1; u<=x+1; u++) {
                for (int v=y-1; v<=y+1; v++) {
                    window[k++] = *(cameraDataPtr + width * v + u);
                }
            }

            // Find the value
            for(int i = 0; i < 9; ++i){
            	new_value += window[i] * filter[i];
            }
            // Assign the median to the processed image array
            *(processedDataPtr + width * y + x) = new_value;
        }
    }
}

void edgedetection_3x3(const uint8_t *cameraDataPtr, uint8_t *processedDataPtr, uint16_t width, uint16_t height) {
    uint8_t window[9];

    int8_t filter = {-1,-1,-1,-1,9,-1,-1,-1,-1};
    // Loop though entire image array
    // ignored the outer edge of the image to simplify the code
    for (uint16_t x=1; x<width-1; x++) {
        for (uint16_t y=1; y<height-1; y++) {
            // Fill the 3x3 window
	    uint8_t k = 0;
	    int16_t new_value = 0;
            for (int u=x-1; u<=x+1; u++) {
                for (int v=y-1; v<=y+1; v++) {
                    window[k++] = *(cameraDataPtr + width * v + u);
                }
            }
            // Find the value
            for(int i = 0; i < 9; ++i){
            	new_value += window[i] * filter[i];
            }
            // Assign the median to the processed image array
            *(processedDataPtr + width * y + x) = new_value;
        }
    }
}
