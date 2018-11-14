//
//  MandelbrotCpp11Thread.cpp
//  ParallelComputingPlayground
//
//  The MIT License (MIT)
//  Copyright (c) 2013 Morten Nobel-Joergensen. All rights reserved.
//

#include "MandelbrotCpp11Thread.h"
#ifndef NO_CPP11
#include <functional>
#include <thread>
#endif
#include <vector>

using namespace std;

#ifndef NO_CPP11
void *MandelbrotCpp11Thread::createData(int width, int height){
    RGB * res = new RGB[width * height];
    
    // create a function object that computes a subsection of the mandelbrot
    // the function uses a clojure of this, res, width and height
    function<void (int, int)> computeRow = [this,res,width,height] (int yFrom, int yTo) {
        for (int y=yFrom; y<yTo; y++){
            for (int x = 0; x < width; x++) {
                RGB color = computePixel(x,y, width, height);
                res[y*width+x] = color;
            }
        }
    };
    
    // launch a number of threads
    vector<thread> threads;
    int numberOfThreads = thread::hardware_concurrency();
    int numberOfRowsPerThread = (height-1)/numberOfThreads + 1;
    for (int i=0;i<numberOfThreads;i++){
        int yFrom = i*numberOfRowsPerThread;
        int yTo = min(height, yFrom+numberOfRowsPerThread);
        threads.push_back(thread(computeRow, yFrom, yTo));
    }

    // wait for threads to complete
    for (int i=0;i<numberOfThreads;i++){
        threads[i].join();
    }
    return res;
}
#endif
