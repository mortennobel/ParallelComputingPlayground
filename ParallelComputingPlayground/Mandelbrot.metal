//
//  Mandelbrot.metal
//  ParallelComputingPlayground
//
//  Created by Morten Nobel Jørgensen on 12/11/2018.
//  Copyright © 2018 morten. All rights reserved.
//

#include <metal_stdlib>
using namespace metal;

constant int mandel_iterations = 1024;

constant float colorResolution = 8.0f; // how many iterations the first color band should use (2nd use the double amount)

half3 getColorByIndex(float index){
    float i = fmod(index,4.0f);
    if (i < 0.5) {
        return half3(0,0,1);
    }
    if (i < 1.5) {
        return half3(1,1,1);
    }
    if (i<2.5){
        return half3(1,1,0);
    }
    return half3(1,0,0);
}



int calculateMandelbrotIterations(float x, float y) {
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

half3 computePixel(float x, float y){
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
    half3 from = getColorByIndex(colorIndex);
    half3 to = getColorByIndex(colorIndex+1.0);
    return mix(from,to,fraction);
}

half3 computePixel(int x, int y, int width, int height){
    float offsetX = -2;
    float scaleX = 4;
    float offsetY = -2;
    float scaleY = 4;
    return computePixel(x/(width-1.0)*scaleX + offsetX, y/(height-1.0) * scaleY + offsetY);
    
}

constant int textureId = 0;

// Mandelbrot compute kernel
kernel void
mandelbrotKernel(texture2d<half, access::write> outTexture [[texture(textureId)]],
                uint2                          gid         [[thread_position_in_grid]])
{
    // Check if the pixel is within the bounds of the output texture
    if((gid.x >= outTexture.get_width()) || (gid.y >= outTexture.get_height()))
    {
        // Return early if the pixel is out of bounds
        return;
    }
    outTexture.write(half4(computePixel(gid.x, gid.y, outTexture.get_width(), outTexture.get_height()), 1.0), gid);
}
