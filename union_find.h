#ifndef UNIONFIND_H_
#define UNIONFIND_H_

#include "func.h"

class UnionFind : public Func {
  public:
    UnionFind(unsigned *roots, uint8_t* ranks)
      : roots_(roots), ranks_(ranks) {}
    inline void operator()(unsigned x, unsigned y) {
      x = roots_[x];
      y = roots_[y];

      while(x != roots_[x]) { x = roots_[x]; }
      while(y != roots_[y]) { y = roots_[y]; }

      if(x != y) {
        uint8_t rx = ranks_[x];
        uint8_t ry = ranks_[y];
        if(rx < ry) {
          roots_[x] = y;
        } else if(rx > ry) {
          roots_[y] = x;
        } else {
          roots_[y] = x;
          ranks_[x]++;
        }
      }
    }

  private:
    unsigned *roots_;
    uint8_t *ranks_;
};

#endif
