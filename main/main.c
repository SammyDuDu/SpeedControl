#include <stdio.h>
#include "esp_timer.h"

#define LEFT_HALL 11
#define RIGHT_HALL 12
#define LEFT_PWM_PIN 15
#define RIGHT_PWM_PIN 16

#define KP 1.0f
#define KI 0.1f
#define KD 0.05f

#define WHEEL_CIRCUMFERENCE 0.025f  // meters
#define PULSES_PER_REVOLUTION 2
#define PWM_MAX_DUTY 100
#define PWM_MIN_DUTY 0

typedef struct {
    float desired_speed;
    float current_speed;
    float previous_error;
    float integral;
    int64_t last_pulse_time;
    uint8_t pwm_channel;
    uint8_t hall_pin;
} motor_state_t;

static motor_state_t left_motor = {
    .desired_speed = 1.0f,
    .pwm_channel = LEFT_PWM_PIN,
    .hall_pin = LEFT_HALL
};

static motor_state_t right_motor = {
    .desired_speed = 1.0f,
    .pwm_channel = RIGHT_PWM_PIN,
    .hall_pin = RIGHT_HALL
};

void app_main(void)
{
    //test
}
