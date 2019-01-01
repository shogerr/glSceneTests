#ifndef __SHADER_HPP
#define __SHADER_HPP 

#include <gl00/common.hpp>
#include <string>
#include <vector>

namespace gl00
{
    class Shader
    {
    public:
        Shader();
        Shader(std::vector<std::pair<GLenum, std::string>> shaderFilenames);
        virtual ~Shader();
        GLuint program_;
    private:
        std::string LoadShader(const std::string& filename);
        GLuint CreateProgram(std::vector<std::pair<GLenum, std::string>>* shaders);
        GLuint CompileShader(const std::string& text, GLenum shaderType);
    };

    bool CheckGlError(const char* function_name);
    static const void _PrintProgramLog(GLuint program);
    static const void _PrintShaderLog(GLuint shader);
}

#endif // __SHADER_HPP
