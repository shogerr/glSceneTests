#pragma once

#include <gl00/common.hpp>
#include <string>
#include <vector>
#include <filesystem>

namespace gl00
{
    class Shader
    {
    public:
        Shader();
        Shader(std::vector<std::pair<GLenum, std::filesystem::path>>& shader_filenames);
        Shader(std::vector<std::pair<GLenum, std::string>>& shader_filenames);
        ~Shader();
        GLuint Program() { return program_; }
    private:
        GLuint program_;
        // Returns the string read from a file.
        std::string LoadShader(const std::string& filename);

        // Methods for returning a compiled & linked shader program.
        GLuint CreateProgram(std::pair<GLenum, std::string>& shaders);
        GLuint CreateProgram(std::vector<std::pair<GLenum, std::string>>& shaders);

        GLuint CompileShader(const std::string& text, GLenum shaderType);
    };

    bool CheckGlError(const char* function_name);
    static const void _PrintProgramLog(GLuint program);
    static const void _PrintShaderLog(GLuint shader);
}
