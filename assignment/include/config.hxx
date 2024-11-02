#ifndef CONFIG_HXX
#define CONFIG_HXX

#if !defined(__cplusplus) || __cplusplus < 202302L
#   error "This project requires C++23 or later"
#endif

#if defined(__linux__) || defined(__unix__)
#   define TARGET_LINUX
#elif defined(_WIN32) || defined(_WIN64)
#   define TARGET_WINDOWS
#else
#   error "Unsupported platform"
#endif

#if defined(__clang__)
#   define COMPILER_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
#   define COMPILER_GCC
#elif defined(_MSC_VER)
#   define COMPILER_MSVC
#else
#   error "Unsupported compiler"
#endif

#endif
