#ifndef __SHADER_HPP
#define __SHADER_HPP 

#include <common.hpp>
#include <string>
#include <vector>

class Shader
{
public:
    Shader();
    Shader(std::vector<std::pair<GLenum, std::string>> shaderFilenames);
    virtual ~Shader();
    GLuint program_;
private:
    std::string loadShader(const std::string& filename);
    GLuint createProgram(std::vector<std::pair<GLenum, std::string>>* shaders);
    GLuint compileShader(const std::string& text, GLenum shaderType);
};

#endif // __SHADER_HPP
