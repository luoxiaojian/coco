#ifndef PAGERANK_H_
#define PAGERANK_H_

#include "func.h"

class CalcDegree : public Func {
  public:
    CalcDegree(float *deg) 
      : deg_(deg) {}

    inline void operator()(unsigned x, unsigned y) {
      deg_[x] += 1.0;
    }
  private:
    float *deg_;
};

class Pagerank : public Func {
  public:
    Pagerank(float *src, float *dst)
      : src_(src), dst_(dst) {}
    inline void operator()(unsigned x, unsigned y) {
      dst_[y] += src_[x];
    }
  private:
    float *src_, *dst_;
};

#endif
