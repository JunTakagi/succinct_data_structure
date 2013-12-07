#ifndef __FLEXIBLE_BYTE_ARRAY__
#define __FLEXIBLE_BYTE_ARRAY__

#include <stdint.h>

namespace succinct {

class FlexibleArray {
  public:
    FlexibleArray(uint64_t size, uint64_t num);
    FlexibleArray(uint64_t size, uint64_t num, uint64_t *array);
    ~FlexibleArray();

    uint64_t get(uint64_t i);
    bool write(uint64_t i, uint64_t num);

    void dump();

  private:
    uint64_t *array;
    uint64_t size;
    uint64_t maxnum;
    bool flg;
    static const uint64_t BLOCK_SIZE = sizeof(uint64_t) * 8;
};

} // namespace succinct

#endif //__FLEXIBLE_BYTE_ARRAY__
