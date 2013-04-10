//
//  main.cpp
//  ParallelComputingPlayground
//
//  The MIT License (MIT)
//  Copyright (c) 2013 Morten Nobel-Joergensen. All rights reserved.
//

#include <iostream>

#include "ResultWindow.h"
#include "AbstractMandelbrot.h"
#include "MandelbrotCpp11Thread.h"
#include "MandelbrotGCD.h"
#include "MandelbrotOpenMP.h"
#include "MandelbrotPOSIXThread.h"
#include "MandelbrotOpenCL.h"


using namespace std;

int main(int argc, char * argv[])
{
    
    cout << "Choose method:" << endl;
    cout << "1. Sequential" << endl;
    cout << "2. C++11 Thread" << endl;
    cout << "3. Grand Central Dispatch" << endl;
    cout << "4. OpenMP" << endl;
    cout << "5. POSIX Threads" << endl;
    cout << "6. OpenCL" << endl;
    unsigned char s;
    cin >> s;
    
    AbstractMandelbrot *res;
    switch (s) {
        case '2':
            res = new MandelbrotCpp11Thread();
            cout << "C++11 Thread"<<endl;
            break;
        case '3':
            res = new MandelbrotGCD();
            cout << "Grand Central Dispatch"<<endl;
            break;
        case '4':
            res = new MandelbrotOpenMP();
            cout << "OpenMP"<<endl;
            break;
        case '5':
            res = new MandelbrotPOSIXThread();
            cout << "POSIX Threads"<<endl;
            break;
        case '6':
            res = new MandelbrotOpenCL();
            cout << "OpenCL"<<endl;
            break;
        default:
            res = new AbstractMandelbrot();
            cout << "Sequential"<<endl;
            break;
    }
    
    int size = 500;
    void *data = res->createData(size, size);
    
    ResultWindow(argc, argv, data, size, size);
    

    return 0;
}

