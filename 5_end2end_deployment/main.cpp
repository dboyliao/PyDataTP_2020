#include <stdio.h>

#include "constants/simple_cnn/params_simple_cnn.hpp"
#include "input_images.hpp"
#include "models/simple_cnn/simple_cnn.hpp"
#include "uTensor/src/uTensor.h"

int argmax(const Tensor &logits) {
  uint32_t num_elems = logits->num_elems();
  float max_value = static_cast<float>(logits(0));
  int max_index = 0;
  for (int i = 1; i < num_elems; ++i) {
    float value = static_cast<float>(logits(i));
    if (value >= max_value) {
      max_value = value;
      max_index = i;
    }
  }
  return max_index;
}

SimpleCnn model;

int main(int argc, const char **argv) {
  printf("Simple CNN Demo\n");
  for (uint32_t i = 0; i < 2; ++i) {
    Tensor in_img = new RomTensor({1, 32, 32, 3}, flt, arr_images[i]);
    Tensor logits = new RamTensor({1, 10}, flt);
    model.set_inputs({{SimpleCnn::input_0, in_img}})
        .set_outputs({{SimpleCnn::output_0, logits}})
        .eval();
    int pred_idx = argmax(logits);
    printf("It's a %s\n", labels[pred_idx]);
  }
  printf("No, it's SUPERMAN!!\n");
  return 0;
}