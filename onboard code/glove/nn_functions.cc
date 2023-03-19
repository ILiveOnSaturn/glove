#include "pico/stdio.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "model.h"
#include "nn_functions.h"
#include "constants.h"

namespace {
    //globals in arduino fashion.
    const tflite::Model* model = nullptr;
    tflite::MicroInterpreter* interpreter = nullptr;
    TfLiteTensor* input = nullptr;
    TfLiteTensor* output = nullptr;

    const int tensor_arena_size = 60*1024;
    uint8_t tensor_arena[tensor_arena_size];
}

void setup_nn() {
    //set up the model and make it ready to run.
    tflite::InitializeTarget();

    model = tflite::GetModel(model_LSTM_tflite);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        printf("Model provided is schema version %lu not equal to supported version %d.\n",
                             model->version(), TFLITE_SCHEMA_VERSION);
        return;
    }

    //instead of calling allops which would take a lot of memory, we only call the ops we need for our model.
    static tflite::MicroMutableOpResolver<4> micro_op_resolver;
    micro_op_resolver.AddUnidirectionalSequenceLSTM();
    micro_op_resolver.AddStridedSlice();
    micro_op_resolver.AddFullyConnected();
    micro_op_resolver.AddSoftmax();

    //set up the interperter
    static tflite::MicroInterpreter static_interperter(
            model, micro_op_resolver, tensor_arena, tensor_arena_size);
    interpreter = &static_interperter;
    interpreter->AllocateTensors();

    //set up input tensors and check to see if its a valid model.
    input = interpreter->input(0);
    if ((input->dims->size != 3) || (input->dims->data[0] != 1) ||
        (input->dims->data[1] != max_timestamp) || (input->type != kTfLiteFloat32)) {
        printf("Bad input tensor parameters in model");
        return;
    }
}

float* get_nn_output(float* imu_input, int size) {
    //run the model with the imu data and return the output pointer.

    //set input buffer values to input
    for (int i=0; i<size; i++) {
        input->data.f[i] = imu_input[i];
    }
    //call model to run
    TfLiteStatus invoke_status = interpreter->Invoke();
    if (invoke_status != kTfLiteOk) {
        printf("Invoke failed");
        return nullptr;
    }
    //return output pointer
    output = interpreter->output(0);
    return output->data.f;
}
