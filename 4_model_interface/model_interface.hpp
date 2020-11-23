#include "modelBase.hpp"
#include "uTensor.h"

using namespace uTensor;

const uint8_t msg_codes[] = {72, 101, 108, 108, 111, 44,
                             32, 119, 111, 114, 108, 100};

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

class MyModel : public ModelInterface<1, 1> {
 public:
  enum names_in : uint8_t { msg };
  enum names_out : uint8_t { output };
  MyModel(bool newline = true) : _print_op(newline) {}

 private:
  ASCIIPrintOperator _print_op;
  ReferenceOperators::AddOperator<uint8_t> _add_op;
  // allocators which control the total memory consumption of a model
  localCircularArenaAllocator<256> meta_alloc;
  localCircularArenaAllocator<256> ram_alloc;

 protected:
  void compute() {
    Context::get_default_context()->set_metadata_allocator(&meta_alloc);
    Context::get_default_context()->set_ram_data_allocator(&ram_alloc);
    Tensor &msg_tensor = inputs[msg].tensor();
    _print_op.set_inputs({{ASCIIPrintOperator::msg, msg_tensor}}).eval();
    uint16_t num_codes = msg_tensor->num_elems();
    Tensor &output_tensor = outputs[output].tensor();
    Tensor ones = new RamTensor({num_codes}, u8);
    for (int i = 0; i < num_codes; ++i) ones(i) = static_cast<uint8_t>(1);
    _add_op
        .set_inputs({
            {ReferenceOperators::AddOperator<uint8_t>::a, msg_tensor},
            {ReferenceOperators::AddOperator<uint8_t>::b, ones},
        })
        .set_outputs(
            {{ReferenceOperators::AddOperator<uint8_t>::c, output_tensor}})
        .eval();
    _print_op.set_inputs({{ASCIIPrintOperator::msg, output_tensor}}).eval();
    return;
  }
};