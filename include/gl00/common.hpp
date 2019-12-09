#pragma once

#if defined(_WIN32)
#include <windows.h>
#endif

#if defined(__linux__)
 #include <GL/glew.h>
#else
 #include <glad/glad.h>
#endif

#define GLM_ENABLE_EXPIREMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define GLM_FORCE_RADIANS
#include <stdio.h>

#include <chrono>

#define EPSILON 0.0000001f

#ifdef NDEBUG
#define LOGE(...) ( printf(__VA_ARGS__) )
#define LOGI(...)
#define LOGD(...)
#else
#define LOGE(...) ( printf(__VA_ARGS__) )
#define LOGI(...) ( printf(__VA_ARGS__) )
#define LOGD(...) ( printf(__VA_ARGS__) )
#endif

// Clean up a resource (delete and set to null).
template<typename T> void CleanUp(T** pptr) {
    if (*pptr) {
        delete *pptr;
        *pptr = NULL;
    }
}

template<typename T> bool Comparef(T a, T b)
{
    T c = a - b;
    if (c < EPSILON && -c < EPSILON)
        return true;
    return false;
}

static uint64_t TimeNs()
{
    using std::chrono::nanoseconds;
    using std::chrono::steady_clock;
    return std::chrono::time_point_cast<nanoseconds>(steady_clock::now()).time_since_epoch().count();
}
