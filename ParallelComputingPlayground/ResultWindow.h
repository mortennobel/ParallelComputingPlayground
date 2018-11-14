//
//  ResultWindow.h
//  ParallelComputingPlayground
//
//  The MIT License (MIT)
//  Copyright (c) 2013 Morten Nobel-Joergensen. All rights reserved.
//

#ifndef __ParallelComputingPlayground__ResultWindow__
#define __ParallelComputingPlayground__ResultWindow__

#include <iostream>

class ResultWindow {
public:
    ResultWindow(int &argc, char ** argv, void *data, int width, int height, bool rgba);
};

#endif /* defined(__ParallelComputingPlayground__ResultWindow__) */
