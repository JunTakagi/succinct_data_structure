#ifndef __IMPLICIT_TABLE_P_TABLE__
#define __IMPLICIT_TABLE_P_TABLE__

#include <stdint.h>
#include "flexibleArray.hpp"

namespace implicit {

namespace dictionary {

class Ptable {
  public:
    Ptable(uint64_t size);
    Ptable(uint64_t size, uint64_t *b);
    ~Ptable();
    uint64_t rank(uint64_t i);

    void dump(unsigned char mode);

    static const unsigned char MODE_NUMS = 0;
    static const unsigned char MODE_BITS = 1;
    static const unsigned char MODE_DEBUG = 2;

  private:
    uint64_t bitsize;
    uint64_t num_case;
    FlexibleArray *bits;
    bool flg;

    uint64_t count_bit(uint64_t i);
    void init();
};

} // namespace dictionary
} // namespace implicit

#endif //__IMPLICIT_TABLE_P_TABLE__
