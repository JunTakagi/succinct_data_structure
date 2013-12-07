#include "ptable.hpp"
#include <iostream>

namespace succinct {
namespace dictionary {

// debug
inline void print_bits(uint64_t size, uint64_t i) {
  uint64_t mask = ~(0xffffffffffffffff << size);
  for (uint64_t j=0; j<size; ++j) {
    i &= mask;
    std::cout << (i >> (size-1));
    i <<= 1;
  }
}

const unsigned char Ptable::MODE_NUMS;
const unsigned char Ptable::MODE_BITS;
const unsigned char Ptable::MODE_DEBUG;

Ptable::Ptable(uint64_t size) {
  bitsize = size;
  num_case = (1 << size);
  bits = new FlexibleArray(size, num_case);
  init();
}

Ptable::Ptable(uint64_t size, uint64_t *b) {
  bitsize = size;
  num_case = (2 << size);
  bits = new FlexibleArray(size, num_case, b);
  init();
}

Ptable::~Ptable() {
  delete bits;
}

void Ptable::init() {
  for (uint64_t i=0; i<num_case; i++) {
    bits->write(i, count_bit(i));
  }
}

uint64_t Ptable::count_bit(uint64_t i) {
  i = (i & 0x5555555555555555) + ((i >> 1) & 0x5555555555555555);
  i = (i & 0x3333333333333333) + ((i >> 2) & 0x3333333333333333);
  i = (i & 0x0f0f0f0f0f0f0f0f) + ((i >> 4) & 0x0f0f0f0f0f0f0f0f);
  i = (i & 0x00ff00ff00ff00ff) + ((i >> 8) & 0x00ff00ff00ff00ff);
  i = (i & 0x0000ffff0000ffff) + ((i >> 16) & 0x0000ffff0000ffff);
  i = (i & 0x00000000ffffffff) + ((i >> 32) & 0x00000000ffffffff);
  return i;
}

uint64_t Ptable::rank(uint64_t i) {
  return bits->get(i);
}

void Ptable::dump(unsigned char mode) {
  switch(mode) {
  case MODE_NUMS:
    for (uint64_t i=0; i<num_case; i++) {
      std::cout << "bits[" << i << "] = " << bits->get(i) << std::endl;
    }
    break;
  case MODE_BITS:
    bits->dump();
    break;
  case MODE_DEBUG:
    for (uint64_t i=0; i<num_case; i++) {
      std::cout << "bits[";
      print_bits(bitsize, i);
      std::cout << "] = " << bits->get(i) << std::endl;
    }
    break;
  default:
    ;
 }
}

} // dictionary
} // succinct
