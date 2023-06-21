#include "mbed.h"
#include "STSServoDriver.h"

EventQueue event_queue;
// Note on wiring: URT-1 TX <-> MCU TX, URT-1 RX <-> MCU RX
BufferedSerial serial4(PA_12, PA_11);
SCSServoDriver servo;

bool has_servo_1 = false;
bool has_servo_2 = false;
constexpr int min_position = 24;
constexpr int max_position = 1000;
int current_position = min_position;
int current_step = 2;

void update_servo()
{
    current_position += current_step;
    if(current_position >= max_position) current_step = -2;
    else if(current_position <= min_position) current_step = 2;
    if(has_servo_1) servo.setTargetPosition(1, current_position);
    if(has_servo_2) servo.setTargetPosition(2, current_position);
    if(has_servo_1) printf("Servo1: %d -> %d\n", servo.getCurrentPosition(1), current_position);
    if(has_servo_2) printf("Servo2: %d -> %d\n", servo.getCurrentPosition(2), current_position);
}

int main()
{
    printf("Application Started.\n");
    if(servo.init(PA_0, &serial4))
    {
        if(servo.ping(1))
        {
            has_servo_1 = true;
            servo.setTargetPosition(1, current_position);
            printf("Servo 1 found!\n");
        }
        else
        {
            printf("Servo 1 not found!\n");
        }
        if(servo.ping(2))
        {
            has_servo_2 = true;
            servo.setTargetPosition(2, current_position);
            printf("Servo 2 found!\n");
        }
        else
        {
            printf("Servo 2 not found!\n");
        }
    }
    else
    {
        printf("Servo not found!\n");
        sleep();
    }
    event_queue.call_every(1s, &update_servo);
    event_queue.dispatch_forever();
}
