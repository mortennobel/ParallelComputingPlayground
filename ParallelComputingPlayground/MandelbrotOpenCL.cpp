//
//  MandelbrotOpenCL.cpp
//  ParallelComputingPlayground
//
//  The MIT License (MIT)
//  Copyright (c) 2013 Morten Nobel-Joergensen. All rights reserved.
//

#include "MandelbrotOpenCL.h"

#include <fstream>
#include <sstream>
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#ifndef NO_OPENCL
using namespace std;

// forward declaration - implementation in the end of this file
cl_context CreateContext();
cl_command_queue CreateCommandQueue(cl_context, cl_device_id*);
cl_program CreateProgram(cl_context context, cl_device_id device, const char* fileName);

void *MandelbrotOpenCL::createData(int width, int height){
    cl_context context = nullptr;
    cl_command_queue commandQueue = nullptr;
    cl_program program = nullptr;
    cl_device_id device = nullptr;
    cl_kernel kernel = nullptr;
    cl_mem memObject = 0;
    cl_int errNum;
    
    // create OpenCL context on first available platform
    context = CreateContext();
    if (context == nullptr){
        cerr << "Failed to create OpenCL Context"<<endl<< flush;
        exit(0);
    }
    
    // create command queue on first device available in context
    commandQueue = CreateCommandQueue(context, &device);
    if (commandQueue == nullptr){
        cerr << "Failed to create command queue" << endl<< flush;
        exit(1);
    }
    program = CreateProgram(context, device, "mandelbrot.cl");
    kernel = clCreateKernel(program, "mandelbrot_kernel", nullptr);
    
    unsigned int arraySize = width*height;
    RGB *res = new RGB[arraySize];
    memObject = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(RGB)*arraySize, nullptr, nullptr);
    
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &memObject);
    clSetKernelArg(kernel, 1, sizeof(cl_int), &width);
    clSetKernelArg(kernel, 2, sizeof(cl_int), &height);
    
    size_t globalWorkSize[1] = {arraySize};
    size_t localWorkSize[1] = {1};
    
    errNum = clEnqueueNDRangeKernel(commandQueue, kernel, 1, nullptr, globalWorkSize, localWorkSize, 0, nullptr, nullptr);
    
    if (errNum != CL_SUCCESS){
        cerr << "Error queing kernel for execution" << endl<< flush;
        exit(10);
    }
    
    // read the output buffer back to the host
    errNum = clEnqueueReadBuffer(commandQueue, memObject, CL_TRUE, 0, sizeof(RGB)*arraySize, res, 0, nullptr, nullptr);
    if (errNum != CL_SUCCESS){
        cerr << "Error reading result buffer" << endl << flush;
        exit(11);
    }
    
    // cleanup
    clReleaseMemObject(memObject);
    clReleaseCommandQueue(commandQueue);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseContext(context);
    
    return res;
}

cl_context CreateContext(){
    cl_int errNum;
    cl_uint numPlatforms;
    cl_platform_id firstPlatformId;
    cl_context context = nullptr;
    errNum = clGetPlatformIDs(1, &firstPlatformId, &numPlatforms);
    if (errNum != CL_SUCCESS || numPlatforms <= 0){
        cerr << "Failed to find any OpenCL platforms"<<endl<< flush;
        exit(2);
    }
    cl_context_properties contextProperties[] = {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties)firstPlatformId,
        0
    };
    context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_GPU, nullptr, nullptr, &errNum);
    if (errNum != CL_SUCCESS){
        cerr << "Could not create GPU context" << endl<< flush;
        exit(3);
    }
    return context;
}

cl_command_queue CreateCommandQueue(cl_context context, cl_device_id *device){
    cl_int errNum;
    cl_device_id *devices;
    cl_command_queue commandQueue = nullptr;
    size_t deviceBufferSize = -1;
    
    // get size of devices buffer
    errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, nullptr, &deviceBufferSize);
    if (errNum != CL_SUCCESS){
        cerr << "Failed to call clGenContextInfo(...,GL_CONTEXT_DEVICES,...)" << endl<< flush;
        exit(4);
    }
    if (deviceBufferSize <= 0){
        cerr << "No devices available"<<endl<< flush;
        exit(5);
    }
    // allocate memory for devices buffer
    devices = new cl_device_id[deviceBufferSize / sizeof(cl_device_id)];
    errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, deviceBufferSize, devices, nullptr);
    if (errNum != CL_SUCCESS){
        cerr << "Failed to get device IDs";
        exit(6);
    }
    commandQueue = clCreateCommandQueue(context, devices[0], 0, nullptr);
    *device = devices[0]; // copy device to parameter
    delete [] devices;
    return commandQueue;
}

cl_program CreateProgram(cl_context context, cl_device_id device, const char* fileName){
    cl_int errNum;
    cl_program program;
    
    ifstream kernelFile(fileName, ios::in);
    if (!kernelFile.is_open()){
        cerr << "Failed to open file for reading "<<fileName<<endl<< flush;
        exit(7);
    }
    ostringstream oss;
    oss << kernelFile.rdbuf();
    string srcStdStr = oss.str();
    const char* srcStr = srcStdStr.c_str();
    program = clCreateProgramWithSource(context, 1, (const char**)&srcStr, nullptr, nullptr);
    if (program == nullptr){
        cerr << "Failed to create CL program from source"<<endl<< flush;
        exit(8);
    }
    errNum = clBuildProgram(program, 0, nullptr, nullptr, nullptr, nullptr);
    if (errNum != CL_SUCCESS){
        char buildLog[16384];
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buildLog), buildLog, nullptr);
        cerr << "Error in kernel:"<<endl<< flush;
        cerr << buildLog<<endl;
        exit(9);
    }
    return program;
}
#endif
