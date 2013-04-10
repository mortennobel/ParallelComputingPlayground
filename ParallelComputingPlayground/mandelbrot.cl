__constant int mandel_iterations = 1024;

__constant float colorResolution = 8.0f;

uchar3 getColorByIndex(float index){
	float i = fmod(index,4.0f);
	if (i < 0.5) {
		return make_uchar3(0,0,255);
	}
	if (i < 1.5) {
		return make_uchar3(255,255,255);
	}
	if (i<2.5){
		return make_uchar3(255,255,0);
	}
	return make_uchar3(255,0,0);
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

uchar3 mixRGB(uchar3 x, uchar3 y, float a){
    float3 xF = convert_float3(x);
    float3 yF = convert_float3(y);
    float3 tmp = xF*(1.0f-a)+yF*a;
    return convert_uchar3_sat_rte(tmp);
}

uchar3 computePixelXY(float x, float y){
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
	uchar3 from = getColorByIndex(colorIndex);
	uchar3 to = getColorByIndex(colorIndex+1.0);
	return mixRGB(from,to,fraction);
}


uchar3 computePixel(x,y, width, height){
    float offsetX = -2;
    float scaleX = 4;
    float offsetY = -2;
    float scaleY = 4;
    return computePixelXY(x/(width-1.0)*scaleX + offsetX, y/(height-1.0) * scaleY + offsetY);
}

__kernel void mandelbrot_kernel(__global uchar *res, const int width, const int height){
    int gid = get_global_id(0);

    int x = gid%width;
    int y = gid/width;
    
    uchar3 color = computePixel(x,y, width, height);
    
    res[gid*3] = color.x;
    res[gid*3+1] = color.y;
    res[gid*3+2] = color.z;
}