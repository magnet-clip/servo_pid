# servo_pid
## Work still in progress
A servo motor from windshield wiper motor with Arduino

Task implementation is based on this: https://learn.adafruit.com/multi-tasking-the-arduino-part-1?view=all


### PID Settings:
1. Smaller overshoot and increased speed. Mostly due to the fact that the motor is no connected to E and +1 terminals. Rotation angle is a bit wider.    

```c++    
#define READ_PERIOD 2
#define UPDATE_PID 10
#define REPORT_PERIOD 500

#define KP 1
#define KI 0.01 
#define KD 0.25
#define MAX_I 16384
#define ACCURACY 20
#define MIN_PWM 10
#define FWD_COEFF 0.4
#define RWD_COEFF 2
#define MIN_DESIRED 60
#define MAX_DESIRED 940

```


2. Small overshoot over long rail-to-rail move, small oscillations in the middle positions:
    Motor is connected to E and +2 terminals. 

```c++    
    #define KP 1
    #define KI 0.01 
    #define KD 0.25
    
    // max value of integral part, when value is smaller integral is too weak to pick up differences in real and desired values
    #define MAX_I 16384       
    
    // update frequency in ms
    #define UPDATE_PID 10       
    
    // motor rotates forward too fast and backward too slow. Hence I slow him down on the FWD
    // and speed him up in rear direction
    #define FWD_COEFF 0.4
    #define RWD_COEFF 2  

    // minimum PWM value when motor would rotate. given current settings 
    #define MIN_PWM 15 
    
    // difference between actual and desired angle which is considered to be ok
    // 10 corresponds to 300 * (10/1024) approximately 3 degrees which is ok
    // I can increase it to 20 (6 degrees) and that
    #define ACCURACY 10 

    // i map desired angle to arbitrary interval given here. I can then adjust this interval so 
    // that to avoid overshoot which will result in motor trying to rotate further then it is possible and break steering
    #define MIN_DESIRED 0     
    #define MAX_DESIRED 1023  
```

There seems to be some non-linearity in my pots, so their relative rotation seems to be out of sync. Maybe i have to make a mapping. And then maybe FRD/RWD coeffs will be obsolete
