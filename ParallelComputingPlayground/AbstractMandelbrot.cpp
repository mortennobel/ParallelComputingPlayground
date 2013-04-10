//
//  AbstractMandelbrot.cpp
//  ParallelComputingPlayground
//
//  The MIT License (MIT)
//  Copyright (c) 2013 Morten Nobel-Joergensen. All rights reserved.
//

#include "AbstractMandelbrot.h"
#include <cmath>

int AbstractMandelbrot::mandel_iterations = 1024;

const float colorResolution = 8.0f; // how many iterations the first color band should use (2nd use the double amount)

RGB *AbstractMandelbrot::createData(int width, int height){
    RGB * res = new RGB[width * height];
    for (int y=0;y<height;y++){
        for (int x=0;x<width;x++){
            RGB color = computePixel(x,y, width, height);
            res[y*width+x] = color;
        }
    }
    return res;
}

RGB getColorByIndex(float index){
	float i = fmod(index,4.0f);
	if (i < 0.5) {
		return RGB(0,0,255);
	}
	if (i < 1.5) {
		return RGB(255,255,255);
	}
	if (i<2.5){
		return RGB(255,255,0);
	}
	return RGB(255,0,0);
}

RGB mix(RGB x, RGB y, float a){
    return RGB(
        (unsigned char)round(x.r*(1-a)+y.r*a),
        (unsigned char)round(x.g*(1-a)+y.g*a),
        (unsigned char)round(x.b*(1-a)+y.b*a)
    );
}

RGB AbstractMandelbrot::computePixel(int x, int y, int width, int height){
    float offsetX = -2;
    float scaleX = 4;
    float offsetY = -2;
    float scaleY = 4;
    return computePixel(x/(width-1.0)*scaleX + offsetX, y/(height-1.0) * scaleY + offsetY);
    
}

RGB AbstractMandelbrot::computePixel(float x, float y){
    int iterations = calculateMandelbrotIterations(x, y);
    float colorIndex = 0.0;
	float iterationsFloat = iterations;
	float colorRes = colorResolution;
	while (iterationsFloat > colorRes){
		iterationsFloat -= colorRes;
		colorRes = colorRes*2.0;
		colorIndex ++;
	}
	float fraction = iterationsFloat/colorRes;
	RGB from = getColorByIndex(colorIndex);
	RGB to = getColorByIndex(colorIndex+1.0);
	return mix(from,to,fraction);
}

int AbstractMandelbrot::calculateMandelbrotIterations(float x, float y) {
    float xx = 0.0;
	float yy = 0.0;
    int count = 0;
	for (int iter = 0;iter < mandel_iterations;iter++){
        float xx2 = xx*xx;
        float yy2 = yy*yy;
        if (xx2 + yy2 <=4.0){
    		float temp = xx2 - yy2 + x;
     		yy = 2.0*xx*yy + y;
    		xx = temp;
     		count ++;
        }
 	}
 	return count;
}