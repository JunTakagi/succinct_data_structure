#include "flexibleArray.hpp"
#include <stdint.h>
#include <math.h>
#include <iostream>

namespace implicit {

// debug
inline void dump_uint64(std::string s, uint64_t num) {
  uint64_t mask = 0x8000000000000000;

  std::cout << s << ":";

  int count = 0;
  for (int i = 0; i < 64; ++i) {
    uint64_t out = (num & mask) >> 63;
    std::cout << out;
    if (++count == 8) {
      std::cout << " ";
      count = 0;
    }
    num <<= 1;
  }

  std::cout << std::endl;
}

const uint64_t FlexibleArray::BLOCK_SIZE;

// safety devide
inline uint64_t dev_ceil(uint64_t numer, uint64_t denom) {
  return (uint64_t) ceil((float) numer / (float) denom);
}

inline uint64_t dev_floor(uint64_t numer, uint64_t denom) {
  return (uint64_t) floor((float) numer / (float) denom);
}

FlexibleArray::FlexibleArray(uint64_t size, uint64_t num) {
  this->size = size;
  this->maxnum = num;
  this->flg = true;

  uint64_t datanum = dev_ceil(size * num, BLOCK_SIZE);
  this->array = new uint64_t[datanum];
}

FlexibleArray::FlexibleArray(uint64_t size, uint64_t num, uint64_t *array) {
  this->size = size;
  this->maxnum = num;
  this->flg = false;
  this->array = array;
}

FlexibleArray::~FlexibleArray() {
  if (flg) {
    delete array;
  }
}

uint64_t FlexibleArray::get(uint64_t i) {
  uint64_t ret = 0;
  uint64_t mask = 0xffffffffffffffff;

  if (maxnum < i) return 0;

  uint64_t bitsstart = i * size;
  uint64_t bitsend = bitsstart + size;
  uint64_t block_head = dev_floor(bitsstart, BLOCK_SIZE);
  uint64_t block_tail = dev_ceil(bitsend + size, BLOCK_SIZE) - 1;

  uint64_t head_mask = mask >> (bitsstart % BLOCK_SIZE);
  uint64_t head_shift = (bitsend % BLOCK_SIZE);
  uint64_t tail_shift = BLOCK_SIZE - head_shift;

  if ( block_head == block_tail ) {
    ret = (array[block_head] & head_mask) >> tail_shift;
  } else {
    uint64_t head = (array[block_head] & head_mask) << head_shift;
    uint64_t tail = array[block_tail] >> tail_shift;

    ret = head | tail;
  }
  return ret;
}

bool FlexibleArray::write(uint64_t i, uint64_t num) {
  if (maxnum < i) return false;

  uint64_t mask = 0xffffffffffffffff;

  // check bit size of num
  uint64_t input_mask = ~(mask << size);
  num &= input_mask;

  uint64_t bitsstart = i * size;
  uint64_t bitsend = bitsstart + size;
  uint64_t block_head = dev_floor(bitsstart, BLOCK_SIZE);
  uint64_t block_tail = dev_ceil(bitsend + size, BLOCK_SIZE) - 1;

  uint64_t head_shift = (bitsstart % BLOCK_SIZE);
  uint64_t tail_shift = BLOCK_SIZE - (bitsend % BLOCK_SIZE);

  uint64_t head_mask = mask >> head_shift;
  uint64_t tail_mask = mask << tail_shift;

  if (block_head == block_tail) {
    uint64_t mask = ~(head_mask & tail_mask);

    uint64_t put = array[block_head] & mask;
    put |= num << tail_shift;
    array[block_head] = put;
  } else {
    uint64_t put_head = array[block_head];
    put_head &= ~head_mask;
    put_head |= num >> (BLOCK_SIZE - tail_shift);
    array[block_head] = put_head;

    uint64_t put_tail = array[block_tail];
    put_tail &= ~tail_mask;
    put_tail |= num << tail_shift;
    array[block_tail] = put_tail;
  }

 return true;
}

void FlexibleArray::dump() {
  uint64_t mask = 0x8000000000000000;
  uint64_t shift_num = BLOCK_SIZE - 1;
  uint64_t line_block_num = 8;
  uint64_t datanum = dev_ceil(size * maxnum, BLOCK_SIZE);
  uint64_t bit_count = 0;
  uint64_t block_count = 0;
  std::cout << "=========================================" << std::endl;
  for (uint64_t i=0; i < datanum; ++i) {
    uint64_t target = array[i];

    for (uint64_t j = 0; j < BLOCK_SIZE; ++j) {
      std::cout << ((target & mask) >> shift_num);
      if (++bit_count == size) {
        if (++block_count == line_block_num) {
          std::cout << std::endl;
          block_count = 0;
        } else {
          std::cout << " ";
        }
        bit_count = 0;
      }
      target <<= 1;
    }
    std::cout << "'";
  }
  std::cout << std::endl << "=========================================" << std::endl;
}

} // namespace implicit

//debug
/*
int main() {
  implicit::FlexibleArray array(7, 64);
  array.write(2, 255);
  array.dump();
  array.write(0, 8);
  array.dump();
  array.write(0, 127);
  array.dump();
  uint64_t ret;
  ret = array.get(0);
  std::cout << "get(0) = " << ret << std::endl;
  ret = array.get(2);
  std::cout << "get(2) = " << ret << std::endl;
  array.write(9, 65);
  array.dump();
  ret = array.get(9);
  std::cout << "get(9) = " << ret << std::endl;
  array.write(64, 127);
  array.dump();
  ret = array.get(64);
  std::cout << "get(64) = " << ret << std::endl;
  array.write(64, 0);
  array.dump();
  array.write(65, 127);
  ret = array.get(65);
  std::cout << "get(65) = " << ret << std::endl;
  array.dump();
}
*/
