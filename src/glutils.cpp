#include "glutils.hpp"

bool checkGlError(const char* functionName) {
    GLint err = glGetError();

    if (err != GL_NO_ERROR) {
        return true;
    }
    return false;
}

void printProgramLog(GLuint program) {
    if (glIsProgram(program)) {
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        char* infoLog = new char[maxLength];

        glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
        if (infoLogLength > 0)
            printf("%s\n", infoLog);

        delete[] infoLog;
    }
    else
        printf("Not a program");
}

void printShaderLog(GLuint shader) {
    if (glIsShader(shader)) {
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        char* infoLog = new char[maxLength];

        glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);

        if (infoLogLength > 0)
            printf("%s\n", infoLog);
        else
            printf("Not a shader\n");
    }
}

std::string loadShader(const std::string& fileName) {
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

GLuint createProgram(std::vector<std::pair<GLenum, std::string>>* shaderSource) {
    GLuint program = 0;
    std::vector<GLuint> shaders;
    GLint linked = GL_FALSE;

    for (auto s : *shaderSource) {
        shaders.push_back(createShader(s.first, s.second.c_str()));
        if (shaders.back())
            goto exit;
    }

    program = glCreateProgram();
    if (!program) {
        checkGlError("glCreateProgram");
        goto exit;
    }

    for (auto s : shaders)
        glAttachShader(program, s);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    if (linked == GL_FALSE) {
        printProgramLog(program);
        program = 0;
    }

exit:
    for (auto s : shaders)
        glDeleteShader(s);
    return program;
}

GLuint createShader(GLenum shaderType, const std::string &text) {
    GLint shaderCompiled = GL_FALSE;

    GLuint shader = 0;
    shader = glCreateShader(shaderType);

    if (!shader) {
        checkGlError("glCreateShader");
        return 0;
    }

    const GLchar* src;
    src = text.c_str();

    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompiled);

    if (shaderCompiled == GL_FALSE) {
        printShaderLog(shader);
    }

    return shader;
}
