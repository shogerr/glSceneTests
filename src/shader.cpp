#include "shader.hpp"

#include <fstream>

Shader::Shader()
{
}

Shader::Shader(std::vector<std::pair<GLenum, std::string>> shaderFilenames)
{
    std::string shaderSource;
    for (auto &s : shaderFilenames)
    {
        shaderSource = loadShader(s.second);
        s.second = shaderSource;
    }

    program_ = createProgram(&shaderFilenames);
}

Shader::~Shader()
{
}

bool checkGlError(const char* functionName)
{
    GLint err = glGetError();

    if (err != GL_NO_ERROR) {
        LOGE("%d\n", err);
        return true;
    }
    return false;
}

static const void _printProgramLog(GLuint program)
{
    if (glIsProgram(program)) {
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

static const void _printShaderLog(GLuint shader)
{
    if (glIsShader(shader)) {
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        char* infoLog = new char[maxLength];

        glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);

        if (infoLogLength > 0)
        {
            LOGE("%s\n", infoLog);
        }
        else
        {
            LOGE("Not a shader\n");
        }
    }
}

std::string Shader::loadShader(const std::string& fileName) {
    std::ifstream file;
    file.open((fileName).c_str());

    std::string output;
    std::string line;

    if (file.is_open()) {
        while (file.good()) {
            getline(file, line);
            output.append(line + "\n");
        }
    }
    else
    {
        LOGE("ERROR : Failed to load shader");
    }

    file.close();
    return output;
}

GLuint Shader::createProgram(std::vector<std::pair<GLenum, std::string>>* shaderSources) {
    GLuint program = 0;
    std::vector<GLuint> shaders;
    GLint linked = GL_FALSE;

    for (auto s : *shaderSources) {
        shaders.push_back(compileShader(s.second.c_str(), s.first));
        //if (shaders.back())
        //    goto exit;
    }

    LOGI("Finished loading shaders.\n");

    program = glCreateProgram();
    if (!program) {
        LOGI("no program");
        checkGlError("glCreateProgram");
        goto exit;
    }

    for (auto s : shaders)
        glAttachShader(program, s);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    if (linked == GL_FALSE)
    {
        _printProgramLog(program);
        program = 0;
    }

exit:
    for (auto s : shaders)
        glDeleteShader(s);
    return program;
}

GLuint Shader::compileShader(const std::string& text, GLenum shaderType)
{
    GLint shaderCompiled = GL_FALSE;

    GLuint shader = 0;
    shader = glCreateShader(shaderType);

    if (!shader)
    {
        checkGlError("glCreateShader");
        return 0;
    }

    const GLchar* src;
    src = text.c_str();

    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompiled);

    if (shaderCompiled == GL_FALSE) {
        _printShaderLog(shader);
    }

    return shader;
}