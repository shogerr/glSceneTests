#pragma once

#include <gl00/common.hpp>
#include <string>
#include <vector>
#include <filesystem>

namespace gl00
{
    using shader_source = std::pair <GLenum, std::string>;
    using shader_path = std::pair <GLenum, std::filesystem::path>;
    using shader_sources = std::vector<std::pair <GLenum, std::string>>;
    using shader_paths = std::vector<std::pair <GLenum, std::filesystem::path>>;

    class Shader
    {
    public:

        Shader();
        //Shader(GLenum type, std::filesystem::path& path);
        Shader(shader_paths& shader_filenames);
        Shader(shader_sources& shader_filenames);
        ~Shader();
        GLuint Program() { return program_; }
    private:
        GLuint program_;
        // Returns the string read from a file.
        std::string LoadShader(const std::string& filename);

        // Methods for returning a compiled & linked shader program.
        GLuint CreateProgram(shader_source& shaders);
        GLuint CreateProgram(shader_sources& shaders);

        GLuint CompileShader(const std::string& text, GLenum shaderType);
    };

    bool CheckGlError(const char* function_name);
    static const void _PrintProgramLog(GLuint program);
    static const void _PrintShaderLog(GLuint shader);
}
