#ifndef PICO_TFLMICRO_NN_FUNCTIONS_H
#define PICO_TFLMICRO_NN_FUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

void setup_nn();

float* get_nn_output(float* imu_input, int size);

#ifdef __cplusplus
}
#endif

#endif //PICO_TFLMICRO_NN_FUNCTIONS_H
