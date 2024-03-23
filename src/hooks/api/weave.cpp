#include "weave.h"

#ifdef _WIN32

#include <detours.h>

loom::weave::WeaveHookStatus loom::weave::CreateHook(WeaveFuncPtr targetFunc, WeaveFuncPtr detourFunc, WeaveFuncPtr *originalFuncOut)
{
    if (DetourTransactionBegin() != NO_ERROR)
    {
        return WeaveHookStatus::WeaveErrorUnknown;
    }

    if (DetourFindFirst(NULL, (PVOID)targetFunc, (PVOID *)originalFuncOut) != NO_ERROR)
    {
        DetourTransactionCancel();
        return WeaveHookStatus::WeaveErrorTargetNotFound;
    }

    if (DetourAttach((PVOID *)originalFuncOut, detourFunc) != NO_ERROR)
    {
        DetourTransactionCancel();
        return WeaveHookStatus::WeaveErrorUnknown;
    }

    if (DetourTransactionCommit() != NO_ERROR)
    {
        DetourDetach((PVOID *)originalFuncOut, NULL);
        return WeaveHookStatus::WeaveErrorUnknown;
    }

    return WeaveHookStatus::WeaveSuccess;
}

#elif defined(__linux__) || defined(__APPLE__)

#include <dlfcn.h>

loom::weave::WeaveHookStatus loom::weave::CreateHook(WeaveFuncPtr targetFunc, WeaveFuncPtr detourFunc, WeaveFuncPtr *originalFuncOut)
{
    void *handle = dlopen(NULL, RTLD_LAZY);
    if (!handle)
    {
        return WeaveHookStatus::WeaveErrorUnknown;
    }

    *originalFuncOut = (loom::weave::WeaveFuncPtr)dlsym(handle, (const char *)targetFunc);
    
    if (!(*originalFuncOut))
    {
        dlclose(handle);
        return WeaveHookStatus::WeaveErrorTargetNotFound;
    }

    dlclose(handle);
    return WeaveHookStatus::WeaveSuccess;
}

#else
#error "Unsupported platform for weave"
#endif
