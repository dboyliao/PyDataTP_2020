#include <stdio.h>

#include "uTensor.h"

using namespace uTensor;
static localCircularArenaAllocator<512> meta_alloc;
static localCircularArenaAllocator<512> ram_alloc;
const uint8_t msg_codes[] = {72, 101, 108, 108, 111, 44,
                             32, 119, 111, 114, 108, 100};

// It's a operator with 1 input and 0 output
class ASCIIPrintOperator : public OperatorInterface<1, 0> {
 public:
  enum names_in : uint8_t { msg };
  ASCIIPrintOperator(bool newline = true) : _with_newline(newline) {}

 protected:
  void compute() {
    Tensor &msg_tensor = inputs[msg].tensor();
    if (msg_tensor->get_type() != u8) {
      uTensor_printf("Only accepting tensor with data of type uint8_t\n");
      Context::get_default_context()->throwError(
          new InvalidTensorDataTypeError);
      return;
    }
    for (int i = 0; i < msg_tensor->get_shape().num_elems(); ++i) {
      uint8_t code = static_cast<uint8_t>(msg_tensor(i));
      char chr = static_cast<char>(code);
      printf("%c", chr);
    }
    if (_with_newline) printf("\n");
  }

 private:
  bool _with_newline;
};

int main(int argc, const char **argv) {
  Context::get_default_context()->set_metadata_allocator(&meta_alloc);
  Context::get_default_context()->set_ram_data_allocator(&ram_alloc);
  ASCIIPrintOperator op;
  Tensor msg = new RomTensor({12}, u8, msg_codes);
  op.set_inputs({{ASCIIPrintOperator::msg, msg}}).eval();
  return 0;
}