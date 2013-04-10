//
//  MandelbrotOpenMP.h
//  ParallelComputingPlayground
//
//  The MIT License (MIT)
//  Copyright (c) 2013 Morten Nobel-Joergensen. All rights reserved.
//

#ifndef __ParallelComputingPlayground__MandelbrotOpenMP__
#define __ParallelComputingPlayground__MandelbrotOpenMP__

#include <iostream>

#include "AbstractMandelbrot.h"

class MandelbrotOpenMP : public AbstractMandelbrot {
public:
    virtual RGB *createData(int width, int height);
};

#endif /* defined(__ParallelComputingPlayground__MandelbrotOpenMP__) */
