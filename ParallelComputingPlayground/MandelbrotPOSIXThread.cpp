//
//  MandelbrotPOSIXThread.cpp
//  ParallelComputingPlayground
//
//  The MIT License (MIT)
//  Copyright (c) 2013 Morten Nobel-Joergensen. All rights reserved.
//

#include "MandelbrotPOSIXThread.h"
#include <pthread.h>
#include <unistd.h>
#include <cassert>

using namespace std;

// thread arguments is wrapped in this struct
struct ThreadArgument {
    int yFrom;
    int yTo;
    int width;
    int height;
    RGB * res;
    MandelbrotPOSIXThread *instance;
};

// the thread function
void *ThreadFunction(void *argument){
    ThreadArgument arg = *((ThreadArgument *) argument);
    
    for (int y=arg.yFrom; y<arg.yTo; y++){
        for (int x = 0; x < arg.width; x++) {
            RGB color = arg.instance->computePixelPublic(x,y, arg.width, arg.height);
            arg.res[y*arg.width+x] = color;
        }
    }
    return NULL;
}

// We need to create a public method in order to call the compute pixel function from outside the class
RGB MandelbrotPOSIXThread::computePixelPublic(int x, int y, int width, int height) {
    return computePixel(x, y, width, height);
}

void *MandelbrotPOSIXThread::createData(int width, int height){
    
    int numberOfThreads = (int)sysconf( _SC_NPROCESSORS_ONLN );
    pthread_t *threads = new pthread_t[numberOfThreads];
    ThreadArgument *thread_args = new ThreadArgument[numberOfThreads];
    
    RGB * res = new RGB[width * height];
    
    int numberOfRowsPerThread = (height-1)/numberOfThreads + 1;
    for (int i=0;i<numberOfThreads;i++){
        // init thread arguments
        thread_args[i].yFrom = i*numberOfRowsPerThread;
        thread_args[i].yTo = min(height, thread_args[i].yFrom + numberOfRowsPerThread);
        thread_args[i].width = width;
        thread_args[i].height = height;
        thread_args[i].res = res;
        thread_args[i].instance = this;
        // create and start thread
        int returnCode = pthread_create(&threads[i], NULL, ThreadFunction, (void *) &thread_args[i]);
        assert(returnCode == 0);
    }

    // join threads
    for (int i=0; i<numberOfThreads; ++i) {
        int returnCode = pthread_join(threads[i], NULL);
        assert(returnCode == 0);
    }

    // cleanup
    delete [] threads;
    delete [] thread_args;
    return res;
}
