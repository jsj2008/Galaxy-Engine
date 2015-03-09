/*!
 * \file shader.h
 * \brief Compile and attach shader to a program
 * \author Antoine MORRIER
 * \version 1.0
 */

#ifndef SHADER_H
#define SHADER_H

#include "../include/include.h"
#include "../include/constant.h"
#include "../Debug/debug.h"

namespace GXY
{
    /**
      * @example Shader shaderExample.cpp
      *
      * @code{.cpp}
      * GXY::Shader shader("file.vert", "file.frag");
      * ....
      *
      * vao.bind();
      * shader.use();
      * Draw...();
      * @endcode
      */

    /**
     * @brief Provide a Shader Interface
     */
    class Shader
    {
    public :
        /**
         * @brief Shader Constructor
         */
        Shader(void);

        /**
         * @brief Shader Constructor
         * @param[in] shaders pair<Path, TypeOfShader> the type can be VERTEX, FRAGMENT, GEOMETRY or COMPUTE
         */
        Shader(std::vector<std::pair<std::string, ShaderType>> const &shaders);

        /**
         * @brief Shader Constructor
         * @param[in] vert : path to vertex Shader
         * @param[in] fragment : path to fragment Shader
         */
        Shader(std::string const &vert, std::string const &fragment);

        /**
         * @brief Shader Constructor
         * @param[in] vert : path to vertex Shader
         * @param[in] geometry : path to geometry Shader
         * @param[in] fragment : path to fragment Shader
         */
        Shader(std::string const &vert, std::string const &geometry, std::string const &fragment);

        /**
         * @brief Shader Constructor copy move semantic
         * @param[in] shader
         */
        Shader(Shader &&shader);

        /**
         * @brief operator = Move Semantic
         * @param[in] shader
         * @return
         */
        Shader &operator=(Shader &&shader);

        Shader(Shader const &shader) = delete;
        Shader &operator=(Shader const &shader) = delete;

        /**
         * @brief Compile a source
         * @param[in] src : source in GLSL
         * @param[in] type : ShaderType of Shader
         */
        void compileSource(std::string const &src, ShaderType type);

        /**
         * @brief Compile a file
         * @param[in] path : Path to a file to compile
         * @param[in] type : ShaderType of Shader
         */
        void compileFile(std::string const &path, ShaderType type);

        /**
         * @brief link the shader
         */
        void link(void);

        /**
         * @brief destroy ressources
         */
        void destroy(void);

        /**
         * @brief Use program
         */
        void use();

        /**
         * @brief Send 4 floats in uniform "name"
         * @param[in] vec : 4 floats
         * @param[in] name : name of uniform variable
         */
        void uniform4f(glm::vec4 const &vec, std::string const &name);

        /**
         * @brief Send 3 floats in uniform "name"
         * @param[in] vec : 3 floats
         * @param[in] name : name of uniform variable
         */
        void uniform3f(glm::vec3 const &vec, std::string const &name);

        /**
         * @brief Send 2 floats in uniform "name"
         * @param[in] vec : 2 floats
         * @param[in] name : name of uniform variable
         */
        void uniform2f(glm::vec2 const &vec, std::string const &name);

        /**
         * @brief Send one float in uniform "name"
         * @param[in] value : 1 float
         * @param[in] name : name of uniform variable
         */
        void uniform1f(float value, std::string const &name);

        /**
         * @brief Send 4 integers in uniform "name"
         * @param[in] vec : 4 integers
         * @param[in] name : name of uniform variable
         */
        void uniform4i(glm::ivec4 const &vec, std::string const &name);

        /**
         * @brief Send 3 integers in uniform "name"
         * @param[in] vec : 3 integers
         * @param[in] name : name of uniform variable
         */
        void uniform3i(glm::ivec3 const &vec, std::string const &name);

        /**
         * @brief Send 2 integers in uniform "name"
         * @param[in] vec : 2 integers
         * @param[in] name : name of uniform variable
         */
        void uniform2i(glm::ivec2 const &vec, std::string const &name);

        /**
         * @brief Send 1 integer in uniform "name"
         * @param[in] value : 1 integer
         * @param[in] name : name of uniform variable
         */
        void uniform1i(int value, std::string const &name);

        /**
          * @brief Shader Destructor
          */
        ~Shader(void);

    private:
        std::map<std::string, s32> mUniform; //!< Associative table between name of variable and uniform location
        std::map<ShaderType, u32> mShader; //!< Associative table between ShaderType and ID of this shader
        u32 mProgram; //!< Id for the program

        /**
         * @brief Get file localised in path and copy it into src
         * @param[in] path : path of file
         * @param[out] src : source to compile
         */
        void mGetSourceInFile(std::string const &path, std::string &src);

    protected :

        /**
         * @brief Get the localisation of uniform name
         * @param[in] name : name of the uniform variable
         * @return
         */
        s32 mGetUniformLocation(std::string const &name);
    };
}
#endif // SHADER_H
