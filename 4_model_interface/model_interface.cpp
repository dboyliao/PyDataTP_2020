#include "model_interface.hpp"

using namespace uTensor;

static localCircularArenaAllocator<128> meta_alloc;
static localCircularArenaAllocator<128> ram_alloc;

int main(int argc, const char **argv) {
  Context::get_default_context()->set_metadata_allocator(&meta_alloc);
  Context::get_default_context()->set_ram_data_allocator(&ram_alloc);
  Tensor msg = new RomTensor({12}, u8, msg_codes);
  Tensor output = new RamTensor({12}, u8);
  MyModel mymodel(true);
  mymodel.set_inputs({{MyModel::msg, msg}})
      .set_outputs({{MyModel::output, output}})
      .eval();
  return 0;
}