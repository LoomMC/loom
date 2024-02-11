# Loom API Guidelines

This refers to the guidelines that any future contributor should follow when working on the Loom API. This document is always subject to change, meaning older code may not follow these standards. Therefore, you should check back always to make sure your code is the best it can be and fits seamlessly with the rest of the Loom code. We are trying to support the latest update FIRST, and then worry about backwards compatibility.

## Important!

Before coding you should obviously know C++ and Lua proficiently. However, just because you know every single way to do something in a language doesn't mean you should go for the most advanced route. Use simple, yet efficient functions that get the job done.

## How to handle functions

Functions should not be explicity used to get variables through the parameters, when you can easily get it through a structure. But when getting a function, you should always setup an event. This is so the user can create a listener, making it easier to use this function. Also make the return type explicit when possible, do not rely on the ```template``` or ```auto``` keywords in C++. This is for easier compilation and for the user to expect what they are going to get. For example:

✅ Good
```cpp
float GetTime_Hook(void* a1, void* a2, void* a3, void* a4) {
    float time = Original_Time(a1, a2, a3, a4);

    // Expose the time to the Loom API
    return time;
}
```

❌ Bad
```cpp
template <typename T>
T GetTime_Hook(void* a1, void* a2, void* a3, void* a4) {
    auto time = Original_Time(a1, a2, a3, a4);

    // Expose the time to the Loom API
    return time;
}
```

Additionally, when naming a function, base it on the actual name of the function and not on the purpose of it, as some functions can be used for multiple things.