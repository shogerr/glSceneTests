#ifndef __GLUTILS_HPP_
#define __GLUTILS_HPP_

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <gl/gl.h>

#define LOGE(...) (std::cout << __VA_ARGS__);

GLuint createProgram(std::vector<std::pair<GLenum, std::string>>* shaderSource);
GLuint createShader(GLenum shaderType, const std::string &text);
std::string loadShader(const std::string& fileName);

#endif // __GLUTILS_HPP_