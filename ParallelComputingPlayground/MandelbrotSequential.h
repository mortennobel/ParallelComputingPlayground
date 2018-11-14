//
//  MandelbrotSequential.hpp
//  ParallelComputingPlayground
//
//  Created by Morten Nobel Jørgensen on 12/11/2018.
//  Copyright © 2018 morten. All rights reserved.
//

#pragma once

#include <iostream>

#include "AbstractMandelbrot.h"

class MandelbrotSequential : public AbstractMandelbrot {
public:
#ifndef NO_OPENCL
    void *createData(int width, int height) override;
#endif
};


