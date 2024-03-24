#ifndef WEAVE_H
#define WEAVE_H

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

#ifdef _WIN32
#include <windows.h> // For Windows memory management and function pointers
#elif defined(__linux__) || defined(__APPLE__)
#include <sys/mman.h> // For memory management (Linux/macOS)
#include <dlfcn.h>    // For function pointers (Linux/macOS)
#else
#error "Unsupported platform for weave"
#endif

namespace loom
{
    namespace weave
    {
        typedef void (*WeaveFuncPtr)(void *, ...);

        enum class WeaveHookStatus
        {
            WeaveSuccess,             // hook created successfully
            WeaveErrorUnknown,        // uknown error occurred
            WeaveErrorTargetNotFound, // target function not found
            WeaveErrorUnsupported,    // operation not supported on current platform
        };

        template <typename R, typename... Args>
        R CallFunc(void* func, Args... args)
        {
            return ((R(*)(Args...))func)(args...);
        }

        WeaveHookStatus CreateHook(WeaveFuncPtr targetFunc, WeaveFuncPtr detourFunc, WeaveFuncPtr* originalFuncOut);
        WeaveHookStatus RemoveHook(WeaveFuncPtr targetFunc, WeaveFuncPtr originalFunc);
        WeaveFuncPtr FindSignature(std::string signature);
    }
}

#endif