#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void setup(void);
void loop(void);
float pid(float error);
float read_input(void);
void cleanup(void);
unsigned long get_time_ms(void);
float generate_noise(float max_noise);
void simulate_process(void);

float dt, last_time = 0;
float integral, previous, output = 0;
float setpoint = 75.00;

#define KP 0.7067
#define KI 0.22
#define KD 0.01

FILE *output_file;
FILE *input_file;

unsigned long get_time_ms(void) {
    return (unsigned long)(clock() * 1000.0 / CLOCKS_PER_SEC);
}

void setup(void) {
    srand(time(NULL));

    input_file = fopen("input.txt", "r");
    if (input_file == NULL) {
        printf("Error: Could not open input file\n");
        exit(1);
    }

    output_file = fopen("datafile.txt", "w");
    if (output_file == NULL) {
        printf("Error: Could not open output file\n");
        fclose(input_file);
        exit(1);
    } 
}

float read_input(void) {
    float value;

    // Close and reopen input_file to ensure it reads the latest data
    fclose(input_file);
    input_file = fopen("input.txt", "r");
    if (input_file == NULL) {
        printf("Error: Could not reopen input file\n");
        exit(1);
    }

    if (fscanf(input_file, "%f", &value) != 1) {
        printf("Error: Could not read input value or end of file reached\n");
        exit(1);
    }
    return value;
}

void loop(void) {
    float now = (float) get_time_ms();
    dt = (now - last_time) / 1000.00;   // div by 1000 to get value in seconds
    if (dt < 0.0001) dt = 0.0001;
    last_time = now;

    float actual = read_input();
    float error = setpoint - actual;
    output = pid(error);

    fprintf(output_file, "%.0f,%.2f\n", now, output);
    fflush(output_file);
}

float pid(float error) {
    float proportional = error;

    integral += error * dt;
    if (integral > 100.0) integral = 100.0;
    if (integral < -100.0) integral = -100.0;

    float derivative = (error - previous) / dt;

    previous = error;
    float output = (KP * proportional) + (KI * integral) + (KD * derivative);

    if (output < 0.0f) output = 0.0f;
    if (output > 250.0f) output = 250.0f;
    return output;
}

float generate_noise(float max_noise) {
    float random = (float)rand() / RAND_MAX;
    return (random * 2 * max_noise) - max_noise;
}

void simulate_process(void) {
    FILE *p_output_file, *p_input_file;
    float t, value;
    char line[100];
    
    // Open datafile.txt for reading
    p_output_file = fopen("datafile.txt", "r");
    if (p_output_file == NULL) {
        printf("Error: Could not open datafile.txt\n");
        return;
    }
    
    // Open input.txt in append mode
    p_input_file = fopen("input.txt", "a");
    if (p_input_file == NULL) {
        printf("Error: Could not open input.txt\n");
        fclose(p_output_file);
        return;
    }

    // Read the last line from datafile.txt
    float last_t = 0, last_value = 0;
    while (fgets(line, sizeof(line), p_output_file)) {
        if (sscanf(line, "%f,%f", &last_t, &last_value) == 2) {
            // Keep reading until the last line
        }
    }

    // Add noise to the last output value
    float noise = generate_noise(5.0);
    float noisy_value = last_value + noise;
    
    // Clamp the noisy value to a valid range
    if (noisy_value < 0) noisy_value = 0;
    if (noisy_value > 255) noisy_value = 255;
    
    // Append the noisy value to input.txt
    fprintf(p_input_file, "%.2f\n", noisy_value);
    fflush(p_input_file); // Ensure the data is written immediately

    printf("Last Value: %.2f, Noise: %.2f, Noisy Value: %.2f\n", last_value, noise, noisy_value);

    // Close files
    fclose(p_output_file);
    fclose(p_input_file);
}

int main(void) {
    input_file = fopen("input.txt", "w");
    fprintf(input_file, "0.0\n");
    fclose(input_file);
    setup();
    atexit(cleanup);
    
    for (int i = 0; i < 99; i++) {
        loop();
        simulate_process();
    }
    
    return 0;
}

void cleanup(void) {
    if (output_file != NULL) {
        fclose(output_file);
    }
    if (input_file != NULL) {
        fclose(input_file);
    }
}