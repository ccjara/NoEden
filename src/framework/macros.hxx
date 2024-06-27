#ifndef NOEDEN_MACROS_HXX
#define NOEDEN_MACROS_HXX

#ifdef NOEDEN_RELEASE
    #define DEBUG_TRAP() ((void)0)
#else
    #if defined(_WIN32) || defined(_WIN64)
        #include <intrin.h>

        #define DEBUG_TRAP() __debugbreak();
    #elif defined(__unix__) || defined(__APPLE__)
        #include <csignal>

        #define DEBUG_TRAP() raise(SIGTRAP)
    #else
        #define DEBUG_TRAP() ((void)0)
    #endif

#endif

#define GL_CHECK(message)                                                                                              \
    {                                                                                                                  \
        auto gl_err = glGetError();                                                                                    \
        if (gl_err != GL_NO_ERROR) {                                                                                   \
            LOG_ERROR("{}: {}", message, gl_err);                                                                      \
            return false;                                                                                              \
        }                                                                                                              \
    }

#endif
