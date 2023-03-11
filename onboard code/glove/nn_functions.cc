#include "pico/stdio.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "model.h"

#define MAX_TIMESTAMPS 11

namespace {
    const tflite::Model* model = nullptr;
    tflite::MicroInterpreter* interpreter = nullptr;
    TfLiteTensor* input = nullptr;
    TfLiteTensor* output = nullptr;

    const int tensor_arena_size = 60*1024;
    uint8_t tensor_arena[tensor_arena_size];
}

void setup_nn() {
    tflite::InitializeTarget();

    model = tflite::GetModel(model_LSTM_tflite);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        printf("Model provided is schema version %lu not equal to supported version %d.\n",
                             model->version(), TFLITE_SCHEMA_VERSION);
        return;
    }

    static tflite::MicroMutableOpResolver<4> micro_op_resolver;
    micro_op_resolver.AddUnidirectionalSequenceLSTM();
    micro_op_resolver.AddStridedSlice();
    micro_op_resolver.AddFullyConnected();
    micro_op_resolver.AddSoftmax();

    static tflite::MicroInterpreter static_interperter(
            model, micro_op_resolver, tensor_arena, tensor_arena_size);
    interpreter = &static_interperter;
    interpreter->AllocateTensors();

    input = interpreter->input(0);
    if ((input->dims->size != 3) || (input->dims->data[0] != 1) ||
        (input->dims->data[1] != MAX_TIMESTAMPS) || (input->type != kTfLiteFloat32)) {
        printf("Bad input tensor parameters in model");
        return;
    }
}

float get_output(float* imu_input) {
    input->data.f = imu_input;

    TfLiteStatus invoke_status = interpreter->Invoke();
    if (invoke_status != kTfLiteOk) {
        printf("Invoke failed");
        return -1.0;
    }
    return *interpreter->output(0)->data.f;
}