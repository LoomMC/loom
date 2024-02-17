#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <Shlobj.h>
#include <stdint.h>
#include <iostream>
#include <type_traits>
#include <typeinfo>

#include "../include/minhook/MinHook.h"

#define INRANGE(x, a, b) (x >= a && x <= b)
#define GET_BYTE(x) (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))
#define GET_BITS(x)                                                            \
  (INRANGE((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xa)              \
                                    : (INRANGE(x, '0', '9') ? x - '0' : 0))

template <typename Ret, typename Type>
Ret& DirectAccess(Type* type, size_t offset) {
    union {
        size_t raw;
        Type* source;
        Ret* target;
    } u;
    u.source = type;
    u.raw += offset;
    return *u.target;
}

#define AS_FIELD(type, name, fn) __declspec(property(get = fn, put = set##name)) type name
#define DEF_FIELD_RW(type, name) __declspec(property(get = get##name, put = set##name)) type name

#define FAKE_FIELD(type, name)       \
	AS_FIELD(type, name, get##name); \
	type& get##name()

#define BUILD_ACCESS(ptr, type, name, offset)                           \
	AS_FIELD(type, name, get##name);                                    \
	type& get##name() const { return DirectAccess<type>(ptr, offset); } \
	void set##name(type v) const { DirectAccess<type>(ptr, offset) = std::move(v); }


template <unsigned int FunctionIndex, typename ReturnType, typename... TArgs>
static inline ReturnType CallVFunc(void* objectPtr, TArgs... args) {
    using fn = ReturnType(__thiscall*)(void*, decltype(args)...);
    return (*static_cast<fn**>(objectPtr))[FunctionIndex](objectPtr, args...);
}

static uintptr_t FindSignature(const char* szSignature)
{
    const char* pattern = szSignature;
    uintptr_t firstMatch = 0;

    static const auto rangeStart = (uintptr_t) GetModuleHandleA("Minecraft.Windows.exe");
    static MODULEINFO miModInfo;
    static bool init = false;

    if (!init)
    {
        init = true;
        GetModuleInformation(GetCurrentProcess(), (HMODULE) rangeStart, &miModInfo, sizeof(MODULEINFO));
    }

    static const uintptr_t rangeEnd = rangeStart + miModInfo.SizeOfImage;

    BYTE patByte = GET_BYTE(pattern);
    const char* oldPat = pattern;

    for (uintptr_t pCur = rangeStart; pCur < rangeEnd; pCur++)
    {
        if (!*pattern)
            return firstMatch;

        while (*(PBYTE) pattern == ' ')
            pattern++;

        if (!*pattern)
            return firstMatch;

        if (oldPat != pattern)
        {
            oldPat = pattern;
            if (*(PBYTE) pattern != '\?')
                patByte = GET_BYTE(pattern);
        }

        if (*(PBYTE) pattern == '\?' || *(BYTE *) pCur == patByte)
        {
            if (!firstMatch)
                firstMatch = pCur;

            if (!pattern[2] || !pattern[1])
                return firstMatch;

            pattern += 2;
        }
        else
        {
            pattern = szSignature;
            firstMatch = 0;
        }
    }

    return 0;
}

void ReplaceVtable(void* _vptr, size_t index, void** outOldFunc, void* newFunc) {
    void** vptr = (void**)_vptr;
    void* oldFunc = vptr[index];
    if (oldFunc == newFunc) {
        return;
    }
    if (outOldFunc != nullptr) {
	    *outOldFunc = oldFunc;
    }

    DWORD oldProtect, tmp;
    VirtualProtect(vptr + index, sizeof(void*), PAGE_READWRITE, &oldProtect);
    vptr[index] = newFunc;
    VirtualProtect(vptr + index, sizeof(void*), oldProtect, &tmp);
}