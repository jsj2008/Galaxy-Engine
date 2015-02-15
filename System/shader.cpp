/*!
 * \file shader.cpp
 * \brief Compile and attach shader to a program
 * \author Antoine MORRIER
 * \version 1.0
 */

#include "shader.h"

namespace GXY
{
    using namespace std;
    using namespace glm;

    Shader::Shader(void) : mProgram(0)
    {

    }

    Shader::Shader(vector<pair<string, ShaderType>> const &shaders)
    {
        try
        {
            for(auto const &shader : shaders)
                compileFile(shader.first, shader.second);
            link();
        }

        catch(Except const &err)
        {
            destroy();
            throw err;
        }
    }

    Shader::Shader(const string &vert, const string &fragment)
    {
        try
        {
            compileFile(vert, ShaderType::VERTEX);
            compileFile(fragment, ShaderType::FRAGMENT);
            link();
        }

        catch(Except const &err)
        {
            destroy();
            throw err;
        }
    }

    Shader::Shader(const string &vert, const string &geometry, const string &fragment)
    {
        try
        {
            compileFile(vert, ShaderType::VERTEX);
            compileFile(geometry, ShaderType::GEOMETRY);
            compileFile(fragment, ShaderType::FRAGMENT);
            link();
        }

        catch(Except const &err)
        {
            destroy();
            throw err;
        }
    }

    Shader::Shader(Shader &&shader)
    {
        mShader = std::move(shader.mShader);
        mProgram = std::move(shader.mProgram);
        mUniform = std::move(shader.mUniform);

        shader.mProgram = 0;
        shader.mShader.clear();
        shader.mUniform.clear();
    }

    Shader &Shader::operator=(Shader &&shader)
    {
        mShader = std::move(shader.mShader);
        mProgram = std::move(shader.mProgram);

        shader.mProgram = 0;
        shader.mShader.clear();

        return *this;
    }

    void Shader::mGetSourceInFile(string const &path, string &src)
    {
        ifstream file(path.c_str());
        string line;

        if(!file)
            throw Except("Impossible to open : " + path);

        // On copie le fichier
        while(getline(file, line))
            src += line + '\n';
    }

    void Shader::compileFile(string const &path, ShaderType type)
    {
        string src;
        mGetSourceInFile(path, src);
        compileSource(src, type);
    }

    void Shader::compileSource(string const &src, ShaderType type)
    {
        u32 shader = 0;

        shader = glCreateShader(static_cast<u32>(type));

        char const *source = src.c_str();
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);

        s32 statCompilation;

        glGetShaderiv(shader, GL_COMPILE_STATUS, &statCompilation);

        // Si il y a erreur
        if(statCompilation != true)
        {
            s32 logSize;

            // Récupère la taille de l'erreur
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

            char *log = new char[logSize + 1];
            memset(log, 0, logSize + 1);

            // Récupère l'erreur
            glGetShaderInfoLog(shader, logSize, &logSize, log);
            string logStr(log);

            // Supprime
            delete[] log;
            glDeleteShader(shader);
            destroy();

            throw Except(logStr);
        }

        mShader[type] = shader;
    }

    void Shader::link(void)
    {
        u32 program;

        program = glCreateProgram();

        for(auto &shader : mShader)
            glAttachShader(program, shader.second);

        glLinkProgram(program);

        s32 statLink;

        glGetProgramiv(program, GL_LINK_STATUS, &statLink);

        // Si il y a erreur
        if(statLink != true)
        {
            s32 logSize;

            // Récupère la taille de l'erreur
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);

            char *log = new char[logSize + 1];
            memset(log, 0, logSize + 1);

            // Récupère l'erreur
            glGetProgramInfoLog(program, logSize, &logSize, log);
            string logStr(log);

            // Supprime
            delete[] log;
            glDeleteProgram(program);
            destroy();
            throw Except(logStr);
        }

        mProgram = program;
    }

    void Shader::use()
    {
        glUseProgram(mProgram);
    }

    void Shader::destroy(void)
    {
        glDeleteProgram(mProgram);
        mProgram = 0;

        for(auto &shader : mShader)
            glDeleteShader(shader.second);

        mShader.clear();
        mProgram = 0;
    }

    s32 Shader::mGetUniformLocation(const string &name)
    {
        auto it = mUniform.find(name);

        // find
        if(it != mUniform.end())
            return it->second;

        s32 location = glGetUniformLocation(mProgram, name.c_str());

        if(location == -1)
            throw Except("Do not find : " + name);

        mUniform[name] = location;

        return location;
    }

    void Shader::uniform1f(float value, const string &name)
    {
        glProgramUniform1fvEXT(mProgram, mGetUniformLocation(name), 1, &value);
    }

    void Shader::uniform2f(const vec2 &vec, const string &name)
    {
        glProgramUniform2fvEXT(mProgram, mGetUniformLocation(name), 1, value_ptr(vec));
    }

    void Shader::uniform3f(const vec3 &vec, const string &name)
    {
        glProgramUniform3fvEXT(mProgram, mGetUniformLocation(name), 1, value_ptr(vec));
    }

    void Shader::uniform4f(const vec4 &vec, const string &name)
    {
        glProgramUniform4fvEXT(mProgram, mGetUniformLocation(name), 1, value_ptr(vec));
    }

    void Shader::uniform1i(s32 value, const string &name)
    {
        glProgramUniform1ivEXT(mProgram, mGetUniformLocation(name), 1, &value);
    }

    void Shader::uniform2i(const ivec2 &vec, const string &name)
    {
        glProgramUniform2ivEXT(mProgram, mGetUniformLocation(name), 1, value_ptr(vec));
    }

    void Shader::uniform3i(const ivec3 &vec, const string &name)
    {
        glProgramUniform3ivEXT(mProgram, mGetUniformLocation(name), 1, value_ptr(vec));
    }

    void Shader::uniform4i(const ivec4 &vec, const string &name)
    {
        glProgramUniform4ivEXT(mProgram, mGetUniformLocation(name), 1, value_ptr(vec));
    }

    Shader::~Shader(void)
    {
        destroy();
    }
}
