#include <stdio.h>

#include "uTensor.h"

using namespace uTensor;
static localCircularArenaAllocator<512> meta_alloc;
static localCircularArenaAllocator<512> ram_alloc;
const uint8_t msg_codes[] = {72, 101, 108, 108, 111, 44,
                             32, 119, 111, 114, 108, 100};
const uint16_t num_codes = sizeof(msg_codes) / sizeof(msg_codes[0]);

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
  ReferenceOperators::AddOperator<uint8_t> add_op;

  Tensor msg = new RomTensor({num_codes}, u8, msg_codes);
  op.set_inputs({{ASCIIPrintOperator::msg, msg}}).eval();

  Tensor ones = new RamTensor({num_codes}, u8);
  Tensor outputs = new RamTensor({num_codes}, u8);
  for (int i = 0; i < num_codes; ++i) ones(i) = static_cast<uint8_t>(1);
  add_op
      .set_inputs({
          {ReferenceOperators::AddOperator<uint8_t>::a, msg},
          {ReferenceOperators::AddOperator<uint8_t>::b, ones},
      })
      .set_outputs({{ReferenceOperators::AddOperator<uint8_t>::c, outputs}})
      .eval();
  op.set_inputs({{ASCIIPrintOperator::msg, outputs}}).eval();
  return 0;
}