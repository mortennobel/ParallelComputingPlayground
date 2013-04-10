//
//  ResultWindow.cpp
//  ParallelComputingPlayground
//
//  The MIT License (MIT)
//  Copyright (c) 2013 Morten Nobel-Joergensen. All rights reserved.
//

#include "ResultWindow.h"

#include <iostream>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>

using namespace std;

#define check_gl_error() _check_gl_error(__FILE__,__LINE__)


GLuint textureId;

void _check_gl_error(const char *file, int line) {
    GLenum err (glGetError());
    
    while(err!=GL_NO_ERROR) {
        string error;
        
        switch(err) {
            case GL_INVALID_OPERATION:      error="INVALID_OPERATION";      break;
            case GL_INVALID_ENUM:           error="INVALID_ENUM";           break;
            case GL_INVALID_VALUE:          error="INVALID_VALUE";          break;
            case GL_OUT_OF_MEMORY:          error="OUT_OF_MEMORY";          break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
        }
        
        cerr << "GL_" << error.c_str() <<" - "<<file<<":"<<line<<endl<<flush;
        err=glGetError();
    }
}

void displayCall(){
    glBegin (GL_QUADS);
    glTexCoord2d(0.0,0.0);
    glVertex2d(-1.0,-1.0);
    glTexCoord2d(1.0,0.0);
    glVertex2d(+1.0,-1.0);
    glTexCoord2d(1.0,1.0);
    glVertex2d(+1.0,+1.0);
    glTexCoord2d(0.0,1.0);
    glVertex2d(-1.0,+1.0);
    glEnd();
    glutSwapBuffers();
    check_gl_error();
}

void createTexture(void *data, int width, int height){
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    check_gl_error();
}

void reshape (int w, int h) {
    glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glutPostRedisplay();
}



ResultWindow::ResultWindow(int &argc, char **argv, void *data, int width, int height){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    glutCreateWindow("Parallel Computing Playground!");
    glutDisplayFunc(displayCall);
    glutReshapeFunc (reshape);
    createTexture(data, width, height);
    glutMainLoop();
}
