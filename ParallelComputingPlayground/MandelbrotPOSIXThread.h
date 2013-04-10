//
//  MandelbrotPOSIXThread.h
//  ParallelComputingPlayground
//
//  The MIT License (MIT)
//  Copyright (c) 2013 Morten Nobel-Joergensen. All rights reserved.
//

#ifndef __ParallelComputingPlayground__MandelbrotPOSIXThread__
#define __ParallelComputingPlayground__MandelbrotPOSIXThread__

#include <iostream>

#include "AbstractMandelbrot.h"

class MandelbrotPOSIXThread : public AbstractMandelbrot {
public:
    virtual RGB *createData(int width, int height);
    
    RGB computePixelPublic(int x, int y, int width, int height);
};


#endif /* defined(__ParallelComputingPlayground__MandelbrotPOSIXThread__) */
