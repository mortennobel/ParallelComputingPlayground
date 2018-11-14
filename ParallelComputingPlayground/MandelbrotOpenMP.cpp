//
//  MandelbrotOpenMP.cpp
//  ParallelComputingPlayground
//
//  The MIT License (MIT)
//  Copyright (c) 2013 Morten Nobel-Joergensen. All rights reserved.
//

#include "MandelbrotOpenMP.h"

using namespace std;

void *MandelbrotOpenMP::createData(int width, int height){
    RGB * res = new RGB[width * height];
   
    #pragma omp parallel for
    for (int y=0;y<height;y++){
        for (int x=0;x<width;x++){
            RGB color = computePixel(x,y, width, height);
            res[y*width+x] = color;
        }
    }
    return res;
}
