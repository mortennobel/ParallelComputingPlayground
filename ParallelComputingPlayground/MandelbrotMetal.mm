//
//  MandelbrotMetal.m
//  ParallelComputingPlayground
//
//  Created by Morten Nobel Jørgensen on 12/11/2018.
//  Copyright © 2018 morten. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "MandelbrotMetal.h"

#include <Metal/Metal.h>
#include <MetalKit/MetalKit.h>
#include <simd/simd.h>

@interface Solution:NSObject
+ (NSNumber *) createData:(simd_uchar4 *)data withWidth: (int) width andHeight: (int) height;
@end

@implementation Solution

// Complete the countingValleys function below.
+ (NSNumber *) createData:(simd_uchar4 *) data withWidth: (int) width andHeight: (int) height {
    
    NSError *error = NULL;
    MTLSize _threadgroupSize;
    MTLSize _threadgroupCount;
    
    id<MTLDevice> _device = MTLCreateSystemDefaultDevice();
    
    // Load all the shader files with a .metal file extension in the project
    id<MTLLibrary> defaultLibrary = [_device newDefaultLibrary];
    
    // Load the kernel function from the library
    id<MTLFunction> kernelFunction = [defaultLibrary newFunctionWithName:@"mandelbrotKernel"];
    
    // Create a compute pipeline state
    id<MTLComputePipelineState> _computePipelineState = [_device newComputePipelineStateWithFunction:kernelFunction
                                                                   error:&error];
    
    if(!_computePipelineState)
    {
        // Compute pipeline State creation could fail if kernelFunction failed to load from the
        //   library.  If the Metal API validation is enabled, we automatically be given more
        //   information about what went wrong.  (Metal API validation is enabled by default
        //   when a debug build is run from Xcode)
        NSLog(@"Failed to create compute pipeline state, error %@", error);
        return nil;
    }
    
    // Set the compute kernel's threadgroup size of 16x16
    _threadgroupSize = MTLSizeMake(16, 16, 1);
    
    // Calculate the number of rows and columns of threadgroups given the width of the input image
    // Ensure that you cover the entire image (or more) so you process every pixel
    _threadgroupCount.width  = ( width + _threadgroupSize.width -  1) / _threadgroupSize.width;
    _threadgroupCount.height = ( height + _threadgroupSize.height - 1) / _threadgroupSize.height;
    
    // Since we're only dealing with a 2D data set, set depth to 1
    _threadgroupCount.depth = 1;
    
    // Create the command queue
    id<MTLCommandQueue>  _commandQueue = [_device newCommandQueue];
    
    MTLTextureDescriptor *textureDescriptor = [[MTLTextureDescriptor alloc] init];
    
    // Indicate we're creating a 2D texture.
    textureDescriptor.textureType = MTLTextureType2D;
    
    // Indicate that each pixel has a Blue, Green, Red, and Alpha channel,
    //    each in an 8 bit unnormalized value (0 maps 0.0 while 255 maps to 1.0)
    textureDescriptor.pixelFormat = MTLPixelFormatRGBA8Unorm;
    textureDescriptor.width = width;
    textureDescriptor.height = height;
    textureDescriptor.usage = MTLTextureUsageShaderWrite | MTLTextureUsageShaderRead ;
    
    id<MTLTexture> outputTexture = [_device newTextureWithDescriptor:textureDescriptor];
    
    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"MyCommandCompute";
    
    id<MTLComputeCommandEncoder> computeEncoder = [commandBuffer computeCommandEncoder];
    
    [computeEncoder setComputePipelineState:_computePipelineState];
    
    [computeEncoder setTexture: outputTexture
                       atIndex:0];
    
    [computeEncoder dispatchThreadgroups:_threadgroupCount
                   threadsPerThreadgroup:_threadgroupSize];
    
    [computeEncoder endEncoding];
    // Finalize rendering here & push the command buffer to the GPU
    [commandBuffer commit];
    [commandBuffer waitUntilCompleted];
    
    [outputTexture getBytes: data
                bytesPerRow: sizeof(simd_uchar4)*width
                 fromRegion:  MTLRegionMake2D(0, 0, width, height)
                mipmapLevel: 0];
    
    return nil;
}

@end

void * MandelbrotMetal::createData(int width, int height){
    simd_uchar4 * res = new simd_uchar4[width * height];
    @autoreleasepool {
        [Solution createData:res withWidth: width andHeight: height];
    }
    return res;
}

int MandelbrotMetal::alignmentInBytes() {
    return 4;
}
