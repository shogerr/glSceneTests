#include <gl00/shader.hpp>

#include <fstream>

gl00::Shader::Shader() : program_(0) {}

gl00::Shader::Shader(std::vector<std::pair<GLenum, std::filesystem::path>>& shader_source_paths)
{
    std::vector<std::pair<GLenum, std::string>> shaders;
    for (auto& s : shader_source_paths)
    {
        std::string source = LoadShader(std::filesystem::absolute(s.second).string());
        shaders.push_back(std::pair<GLenum, std::string>{s.first, source});
    }

    program_ = CreateProgram(shaders);
}

gl00::Shader::Shader(std::vector<std::pair<GLenum, std::string>>& shaders)
{
#pragma message("This constructor will no longer open files in the future.")
    for (auto &s : shaders)
    {
        std::string source = LoadShader(s.second);
        s.second = source;
    }

    program_ = CreateProgram(shaders);
}

gl00::Shader::~Shader() {
    LOGD("Destroying Shader.\n");
    glDeleteProgram(program_);
}

bool gl00::CheckGlError(const char* function_name)
{
    GLint err = glGetError();

    if (err != GL_NO_ERROR)
    {
        LOGE("%d\n", err);
        return true;
    }

    return false;
}

static const void gl00::_PrintProgramLog(GLuint program)
{
    if (glIsProgram(program))
    {
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        char* infoLog = new char[maxLength];

        glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
        if (infoLogLength > 0)
        {
            LOGE("%s\n", infoLog);
        }

        delete[] infoLog;
    }
    else
    {
        LOGE("Not a program");
    }
}

static const void gl00::_PrintShaderLog(GLuint shader)
{
    if (glIsShader(shader))
    {
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        char* infoLog = new char[maxLength];

        glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);

        if (infoLogLength > 0)
        {
            LOGI("%s\n", infoLog);
        }
        else
        {
            LOGE("Not a shader.\n");
        }

        delete[] infoLog;
    }
}

std::string gl00::Shader::LoadShader(const std::string& file_name) {
    std::ifstream file;
    file.open((file_name).c_str());

    std::string output;
    std::string line;

    if (file.is_open())
    {
        while (file.good())
        {
            getline(file, line);
            output.append(line + "\n");
        }
    }
    else
    {
        LOGE("Failed to load shader: %s\n", file_name.c_str());
    }

    file.close();
    return output;
}

GLuint gl00::Shader::CreateProgram(std::pair<GLenum, std::string> & source)
{
    GLuint program = 0;
    GLint linked = GL_FALSE;
    GLuint shader = CompileShader(source.second.c_str(), source.first);
    program = glCreateProgram();
    if (!program)
    {
        LOGE("Program failed to create.");
        CheckGlError("glCreateProgram");
        goto exit;
    }
    glAttachShader(program, shader);

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    if (linked == GL_FALSE)
    {
        gl00::_PrintProgramLog(program);
        program = 0;
    }
exit:
    glDeleteShader(shader);

    return program;
}

GLuint gl00::Shader::CreateProgram(std::vector<std::pair<GLenum, std::string>> & shaderSources) {
    GLuint program = 0;
    std::vector<GLuint> shaders;
    GLint linked = GL_FALSE;

    // Compile the list of shaders.
    for (auto s : shaderSources)
        shaders.push_back(CompileShader(s.second.c_str(), s.first));

    LOGD("Finished loading shaders.\n");

    // Create the shader program.
    program = glCreateProgram();

    if (!program)
    {
        LOGE("Program failed to create.");
        CheckGlError("glCreateProgram");
        goto exit;
    }

    // Attach each of the compiled shaders to the program.
    for (auto s : shaders)
        glAttachShader(program, s);

    // Link the program.
    glLinkProgram(program);

    // Check the program status.
    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    if (linked == GL_FALSE)
    {
        gl00::_PrintProgramLog(program);
        program = 0;
    }

// Skip linking the program if there is an error.
exit:
    for (auto s : shaders)
        glDeleteShader(s);

    return program;
}

GLuint gl00::Shader::CompileShader(const std::string& text, GLenum shaderType)
{
    GLint shaderCompiled = GL_FALSE;

    GLuint shader = 0;
    shader = glCreateShader(shaderType);

    if (!shader)
    {
        gl00::CheckGlError("glCreateShader");
        return 0;
    }

    const GLchar* src = text.c_str();

    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompiled);

    if (shaderCompiled == GL_FALSE)
        gl00::_PrintShaderLog(shader);

    return shader;
}