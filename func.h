#ifndef FUNC_H_
#define FUNC_H_

class Func {
  public:
    Func() {}
    virtual ~Func() {}
    virtual void operator()(unsigned x, unsigned y) = 0;
};

#endif
