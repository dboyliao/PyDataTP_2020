#include <stdio.h>

#include "uTensor.h"

using namespace uTensor;

const uint8_t msg_codes[] = {72, 101, 108, 108, 111, 44,
                             32, 119, 111, 114, 108, 100};
const uint16_t num_codes = sizeof(msg_codes) / sizeof(msg_codes[0]);
static localCircularArenaAllocator<512> meta_alloc;
static localCircularArenaAllocator<sizeof(float) * num_codes> ram_alloc;

int main(int argc, const char **argv) {
  Context::get_default_context()->set_metadata_allocator(&meta_alloc);
  Context::get_default_context()->set_ram_data_allocator(&ram_alloc);
  Tensor msg_tensor = new RomTensor({num_codes}, u8, msg_codes);
  printf("Reading rom tensor: ");
  for (int i = 0; i < num_codes; ++i) {
    uint8_t code = static_cast<uint8_t>(msg_tensor(i));
    char c = static_cast<char>(code);
    printf("%c", c);
  }
  printf("\n");
  // writing to tensor
  Tensor ram_msg_tensor = new RamTensor({num_codes}, flt);
  for (int i = 0; i < num_codes; ++i) {
    ram_msg_tensor(i) = static_cast<float>(msg_tensor(i));
  }
  printf("Reading ram tensor: ");
  for (int i = 0; i < num_codes; ++i) {
    uint8_t code = static_cast<uint8_t>(ram_msg_tensor(i));
    char c = static_cast<char>(code);
    printf("%c", c);
  }
  printf("\n");
  return 0;
}