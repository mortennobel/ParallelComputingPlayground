//
//  MandelbrotSequential.cpp
//  ParallelComputingPlayground
//
//  Created by Morten Nobel Jørgensen on 12/11/2018.
//  Copyright © 2018 morten. All rights reserved.
//

#include "MandelbrotSequential.h"

void *MandelbrotSequential::createData(int width, int height){
     RGB * res = new RGB[width * height];
     for (int y=0;y<height;y++){
         for (int x=0;x<width;x++){
             RGB color = computePixel(x,y, width, height);
             res[y*width+x] = color;
         }
     }
     return res;
 }
