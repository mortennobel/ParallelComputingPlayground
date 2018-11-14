//
//  MandelbrotCpp11Thread.h
//  ParallelComputingPlayground
//
//  The MIT License (MIT)
//  Copyright (c) 2013 Morten Nobel-Joergensen. All rights reserved.
//

#ifndef __ParallelComputingPlayground__MandelbrotCpp11Thread__
#define __ParallelComputingPlayground__MandelbrotCpp11Thread__

#include <iostream>

#include "AbstractMandelbrot.h"

class MandelbrotCpp11Thread : public AbstractMandelbrot {
public:
#ifndef NO_CPP11
    void *createData(int width, int height) override;
#endif
};

#endif /* defined(__ParallelComputingPlayground__MandelbrotCpp11Thread__) */
