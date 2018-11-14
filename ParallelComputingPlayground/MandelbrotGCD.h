//
//  MandelbrotGrandCentralDispatchThread.h
//  ParallelComputingPlayground
//
//  The MIT License (MIT)
//  Copyright (c) 2013 Morten Nobel-Joergensen. All rights reserved.
//

#ifndef __ParallelComputingPlayground__MandelbrotGrandCentralDispatchThread__
#define __ParallelComputingPlayground__MandelbrotGrandCentralDispatchThread__

#include <iostream>

#include "AbstractMandelbrot.h"

class MandelbrotGCD : public AbstractMandelbrot {
public:
#ifndef NO_CPP11
    void *createData(int width, int height) override;
#endif
};

#endif /* defined(__ParallelComputingPlayground__MandelbrotGrandCentralDispatchThread__) */
