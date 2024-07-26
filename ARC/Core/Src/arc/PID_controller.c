#include <stdio.h>
#include <math.h>

float target = 0;
float accumulated_error = 0;
float last_error = 0;

/*
input(distance from the line) -> PID controller -> output(steering angle)
*/
float PID(float input){
    float delta_time = 0.001;
    float k_p = 2; // tune the gain value manually
    float k_i = 0; // tune the gain value manually
    float k_d = 0; // tune the gain value manually

    float current_error = target - input; // find out error

    float slope_of_change = (current_error - last_error)/delta_time; // differentiation D

    accumulated_error += current_error * delta_time; // integration I

    float out = current_error*k_p + k_i*accumulated_error + k_d*slope_of_change; // P + I + D
    last_error = current_error; // update last error

    return out;
}


int main(){

    return 0;
}