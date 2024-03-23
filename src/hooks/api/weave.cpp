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

loom::weave::WeaveHookStatus loom::weave::RemoveHook(WeaveFuncPtr targetFunc, WeaveFuncPtr originalFunc) {
    if (DetourTransactionBegin() != NO_ERROR) {
        return WeaveHookStatus::WeaveErrorUnknown;
    }

    if (DetourUpdateThread(GetCurrentThread()) != NO_ERROR) {
        DetourTransactionAbort();
        return WeaveHookStatus::WeaveErrorUnknown;
    }

    if (DetourDetach((PVOID*)&targetFunc, originalFunc) != NO_ERROR) {
        DetourTransactionAbort();
        return WeaveHookStatus::WeaveErrorUnknown;
    }

    if (DetourTransactionCommit() != NO_ERROR) {
        return WeaveHookStatus::WeaveErrorUnknown;
    }

    return WeaveHookStatus::WeaveSuccess;
}

#elif defined(__linux__) || defined(__APPLE__)

#include <dlfcn.h>
#include <unistd.h>

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

loom::weave::WeaveHookStatus loom::weave::RemoveHook(WeaveFuncPtr targetFunc, WeaveFuncPtr originalFunc) {
    uintptr_t targetFuncAddress = reinterpret_cast<uintptr_t>(targetFunc);

    size_t pageSize = sysconf(_SC_PAGESIZE);
    uintptr_t pageStart = targetFuncAddress & ~(pageSize - 1);
    if (mprotect(reinterpret_cast<void*>(pageStart), pageSize, PROT_READ | PROT_WRITE) != 0) {
        return WeaveHookStatus::WeaveErrorUnknown;
    }

    std::memcpy(reinterpret_cast<void*>(targetFuncAddress), reinterpret_cast<const void*>(originalFunc), sizeof(targetFunc));

    if (mprotect(reinterpret_cast<void*>(pageStart), pageSize, PROT_READ) != 0) {
        return WeaveHookStatus::WeaveErrorUnknown;
    }

    return WeaveHookStatus::WeaveSuccess;
}

#else
#error "Unsupported platform for weave"
#endif
