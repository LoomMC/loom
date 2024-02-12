#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <type_traits>
#include <typeinfo>

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