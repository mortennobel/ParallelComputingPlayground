//
//  MandelbrotMetal.h
//  ParallelComputingPlayground
//
//  Created by Morten Nobel Jørgensen on 12/11/2018.
//  Copyright © 2018 morten. All rights reserved.
//

#pragma once

#include "AbstractMandelbrot.h"

class MandelbrotMetal : public AbstractMandelbrot {
public:
    void *createData(int width, int height) override;
    int alignmentInBytes() override;
};
