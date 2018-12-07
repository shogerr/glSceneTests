#ifndef __COMMON_HPP
#define __COMMON_HPP

//#if defined(_WIN32)
#include <windows.h>
//#endif

#include <GL/glew.h>
#include <GL/GL.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define GLM_FORCE_RADIANS

#include <stdio.h>

#define EPSILON 0.0000001f

#define LOGE(...) ( printf(__VA_ARGS__) )
#define LOGI(...) ( printf(__VA_ARGS__) )
#define LOGD(...) ( printf(__VA_ARGS__) )

// Clean up a resource (delete and set to null).
template<typename T> void CleanUp(T** pptr) {
    if (*pptr) {
        delete *pptr;
        *pptr = NULL;
    }
}

template<typename T> bool Comparef(T a, T b)
{
    float c = a - b;
    if (c < EPSILON && -c < EPSILON)
        return true;
    return false;
}

#endif //__COMMON_HPP
