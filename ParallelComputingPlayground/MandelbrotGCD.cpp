//
//  MandelbrotGrandCentralDispatchThread.cpp
//  ParallelComputingPlayground
//
//  The MIT License (MIT)
//  Copyright (c) 2013 Morten Nobel-Joergensen. All rights reserved.
//

#include "MandelbrotGCD.h"
#ifndef NO_CPP11
#include "dispatch/dispatch.h"
#endif
using namespace std;

#ifndef NO_CPP11

void *MandelbrotGCD::createData(int width, int height){
    RGB * res = new RGB[width * height];
    
    // create a block that computes a subsection of the mandelbrot
    void (^computeRow)(int, int) = ^(int yFrom, int yTo) {
        for (int y=yFrom; y<yTo; y++){
            for (int x = 0; x < width; x++) {
                RGB color = computePixel(x,y, width, height);
                res[y*width+x] = color;
            }
        }
    };
    
    // get handle to default queue with thread pool
    dispatch_queue_t aQueue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    
    // launch a number of threads
    int numberOfThreads = (int)sysconf( _SC_NPROCESSORS_ONLN );
    int numberOfRowsPerThread = (height-1)/numberOfThreads + 1;
    dispatch_apply(numberOfThreads, aQueue, ^(size_t index) {
        int yFrom = (int)index*numberOfRowsPerThread;
        int yTo = min(height, yFrom+numberOfRowsPerThread);
        computeRow(yFrom, yTo);
    });
    return res;
}
#endif
