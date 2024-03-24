#include "weave.h"

bool isPatternMatch(const char *pattern, const char *memory)
{
    while (*pattern)
    {
        if (*pattern == '?')
        {
            pattern++;
            memory++;
        }
        else if (*pattern != *memory)
        {
            return false;
        }
        else
        {
            pattern += 2; // skip two chars for every byte
            memory++;
        }
    }

    return true;
}

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

loom::weave::WeaveHookStatus loom::weave::RemoveHook(WeaveFuncPtr targetFunc, WeaveFuncPtr originalFunc)
{
    if (DetourTransactionBegin() != NO_ERROR)
    {
        return WeaveHookStatus::WeaveErrorUnknown;
    }

    if (DetourUpdateThread(GetCurrentThread()) != NO_ERROR)
    {
        DetourTransactionAbort();
        return WeaveHookStatus::WeaveErrorUnknown;
    }

    if (DetourDetach((PVOID *)&targetFunc, originalFunc) != NO_ERROR)
    {
        DetourTransactionAbort();
        return WeaveHookStatus::WeaveErrorUnknown;
    }

    if (DetourTransactionCommit() != NO_ERROR)
    {
        return WeaveHookStatus::WeaveErrorUnknown;
    }

    return WeaveHookStatus::WeaveSuccess;
}

loom::weave::WeaveFuncPtr loom::weave::FindSignature(std::string signature)
{
    HMODULE target_module = GetModuleHandleA("Minecraft.Windows.exe");

    if (!target_module)
    {
        return nullptr;
    }

    MODULEINFO module_info;
    if (!GetModuleInformation(GetCurrentProcess(), target_module, &module_info, sizeof(MODULEINFO)))
    {
        return nullptr;
    }

    uintptr_t base_address = reinterpret_cast<uintptr_t>(target_module);
    size_t image_size = module_info.SizeOfImage;

    for (uintptr_t current_address = base_address; current_address < base_address + image_size; current_address++)
    {
        if (isPatternMatch(signature.c_str(), reinterpret_cast<const char *>(current_address)))
        {
            return reinterpret_cast<WeaveFuncPtr>(current_address);
        }
    }

    return nullptr;
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

loom::weave::WeaveHookStatus loom::weave::RemoveHook(WeaveFuncPtr targetFunc, WeaveFuncPtr originalFunc)
{
    uintptr_t targetFuncAddress = reinterpret_cast<uintptr_t>(targetFunc);

    size_t pageSize = sysconf(_SC_PAGESIZE);
    uintptr_t pageStart = targetFuncAddress & ~(pageSize - 1);
    if (mprotect(reinterpret_cast<void *>(pageStart), pageSize, PROT_READ | PROT_WRITE) != 0)
    {
        return WeaveHookStatus::WeaveErrorUnknown;
    }

    std::memcpy(reinterpret_cast<void *>(targetFuncAddress), reinterpret_cast<const void *>(originalFunc), sizeof(targetFunc));

    if (mprotect(reinterpret_cast<void *>(pageStart), pageSize, PROT_READ) != 0)
    {
        return WeaveHookStatus::WeaveErrorUnknown;
    }

    return WeaveHookStatus::WeaveSuccess;
}

loom::weave::WeaveFuncPtr loom::weave::FindSignature(std::string signature)
{
    void *handle = dlopen(NULL, RTLD_LAZY);
    if (!handle)
    {
        return nullptr;
    }

    const char *target_module = "libminecraftpe.so";
    void *target_address = dlsym(handle, target_module);
    dlclose(handle);

    if (!target_address)
    {
        return nullptr;
    }

    int page_size = sysconf(_SC_PAGESIZE);
    uintptr_t base_address = reinterpret_cast<uintptr_t>(target_address);
    uintptr_t page_start = base_address & ~(page_size - 1);
    size_t map_size = page_size;

    int prot = PROT_READ;

    void *mapped_address = mmap(nullptr, map_size, prot, MAP_SHARED, fileno(stdin), page_start);
    if (mapped_address == MAP_FAILED)
    {
        return nullptr;
    }

    if (isPatternMatch(signature.c_str(), reinterpret_cast<const char *>(mapped_address)))
    {
        WeaveFuncPtr target_func = reinterpret_cast<WeaveFuncPtr>(mapped_address);
        munmap(mapped_address, map_size);
        return target_func;
    }

    munmap(mapped_address, map_size);

    return nullptr;
}

#else
#error "Unsupported platform for weave"
#endif
