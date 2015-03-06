/*!
 * \file debug.h
 * \brief forgive tools for debuging
 * \author Antoine MORRIER
 * \version 1.0
 */

#ifndef DEBUG_H
#define DEBUG_H

#include "../include/include.h"
#include "../System/buffer.h"

/**
 * @brief print vec4
 * @param[in] stream
 * @param[in] vec
 * @return stream
 */
inline std::ostream &operator<<(std::ostream &stream, glm::vec4 const &vec)
{
    stream << vec.x << " " << vec.y << " " << vec.z << " " << vec.w;
    return stream;
}

/**
 * @brief print vec3
 * @param[in] stream
 * @param[in] vec
 * @return stream
 */
inline std::ostream &operator<<(std::ostream &stream, glm::vec3 const &vec)
{
    stream << vec.x << " " << vec.y << " " << vec.z;
    return stream;
}

/**
 * @brief print vec2
 * @param[in] stream
 * @param[in] vec
 * @return stream
 */
inline std::ostream &operator<<(std::ostream &stream, glm::vec2 const &vec)
{
    stream << vec.x << " " << vec.y;
    return stream;
}

namespace GXY
{
    template<typename func, typename ...Args>
    /**
     * @brief function which return time taken by function
     * @param[in] function : the function send to "time"
     * @param[in] args : argument for function
     */
    inline void time(func &function, Args&& ...args)
    {
        GXY::u32 query;
        GXY::u64 resultQuery = 0;

        glGenQueries(1, &query);
        glBeginQuery(GL_TIME_ELAPSED, query);
            function(std::forward<Args>(args)...);
            GXY::synchronize();

        glEndQuery(GL_TIME_ELAPSED);

        while(!resultQuery)
            glGetQueryObjectui64v(query, GL_QUERY_RESULT_AVAILABLE, &resultQuery);

        glGetQueryObjectui64v(query, GL_QUERY_RESULT, &resultQuery);

        std::cout << "This function take : " << (double)resultQuery / 1000000 << " ms to be executed." << std::endl;
    }

    /**
     * @brief The Exception class
     *
     * Class to provide info if error
     */
    class Except : public std::exception
    {
    public:

        /**
         * @brief Except Constructor
         * @param[in] error : string with error
         */
        Except(std::string const &error) : mError(error){}

        /**
         * @brief Return the error
         * @return Return the string with error
         */
        virtual const char *what(void) const noexcept
        {
            return mError.c_str();
        }

        /**
         * @brief Except Destructor
         */
        virtual ~Except(void) noexcept
        {

        }

    private:
        std::string mError; //!< The error

    };
}

#endif // DEBUG_H

