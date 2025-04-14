#pragma once
#include <GL/gl.h>

namespace OpenGLUtils {
    GLuint LoadTexture(const char* path, int* width, int* height);
    void SetupBasicRenderState();
}