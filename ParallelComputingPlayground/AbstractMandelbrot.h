//
//  AbstractMandelbrot.h
//  ParallelComputingPlayground
//
//  The MIT License (MIT)
//  Copyright (c) 2013 Morten Nobel-Joergensen. All rights reserved.
//

#ifndef __ParallelComputingPlayground__AbstractMandelbrot__
#define __ParallelComputingPlayground__AbstractMandelbrot__

#include <iostream>

struct RGB {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    RGB(){}
    RGB(unsigned char r, unsigned char g, unsigned char b):r(r),g(g),b(b){}

};

class AbstractMandelbrot {
public:
    AbstractMandelbrot(){}
    virtual ~AbstractMandelbrot(){}
    
    virtual void *createData(int width, int height) = 0;
    
    virtual int alignmentInBytes();
    
    static int mandel_iterations;
protected:
    RGB computePixel(float x, float y);
    RGB computePixel(int x, int y, int width, int height);
    int calculateMandelbrotIterations(float x, float y);
    RGB getColor(int iterations);
    
};

#endif /* defined(__ParallelComputingPlayground__AbstractMandelbrot__) */
