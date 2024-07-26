void change_brightness(float* Image, int height, int width, float shift){
    for(int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int value = *(Image + width*i + j);
            if(0 <= (value+=shift) <= 255){
                *(Image + width*i + j) += shift;
            }
            
            if((value+=shift) >= 255){
                *(Image + width*i + j) = 255;
            }
            if(0 >= (value+= shift)){
                *(Image + width*i + j) = 0;
            }
            
        }
    }
}

void change_contrast(float* Image, int height, int width, float contrast){
    for(int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float value = (*(Image + width*i + j) - 128) * contrast + 128;
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
void autocorrect(float* Image, int height, int width){
    int shift =  128 - *(Image + height*height/2 + width/2); // the center pixel of the image 
    change_brightness(Image, height, width, shift); // change brightness to around 128
    float max = 0;
    for(int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if(*(Image + width*i + j) > max){
                max = *(Image + width*i + j);
            }
        }
    }
    float contrast = 127/(max-128); // maximize contrast
    change_contrast(Image, height, width, contrast); 
}
