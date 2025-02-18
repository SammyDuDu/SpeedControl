#include <stdio.h>
#include "esp_timer.h"

#define LEFT_HALL 11
#define RIGHT_HALL 12
#define LEFT_PWM_PIN 15
#define RIGHT_PWM_PIN 16

#define KP 1.0f
#define KI 0.1f
#define KD 0.05f

#define WHEEL_CIRCUMFERENCE 0.094f  // meters
#define PULSES_PER_REVOLUTION 2
#define PWM_MAX_DUTY 1.0f
#define PWM_MIN_DUTY 0.0f

typedef struct {
    float desired_speed, current_speed;
    float previous_error, integral;
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

float calculate_pid(motor_state_t* motor, float error, float dt);
static void update_pwm(uint8_t channel, float pid_output);

float calculate_pid(motor_state_t* motor, float error, float dt) {

    float P = KP * error;
    motor->integral += error * dt;
    float I = KI * motor->integral;
    float D = KD * ((error - motor->previous_error) / dt);

    motor->previous_error = error;

    float pwm_output = P + I + D;
    if (pwm_output < 0.0f) pwm_output = 0.0f;
    if (pwm_output > 1.0f) pwm_output = 1.0f;

    return pwm_output;
}

void app_main(void)
{
    //test
}


