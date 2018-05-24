#ifndef STATS_H_
#define STATS_H_

#include "func.h"

class Stats : public Func {
  public:
    Stats(unsigned &max_x, unsigned &max_y, size_t &edges)
      : max_x_(max_x), max_y_(max_y), edges_(edges) {}

    inline void operator()(unsigned x, unsigned y) {
      if(max_x_ < x) {
        max_x_ = x;
      }
      if(max_y_ < y) {
        max_y_ = y;
      }
      edges_++;
    }
  private:
    unsigned &max_x_;
    unsigned &max_y_;
    size_t &edges_;
};

#endif
