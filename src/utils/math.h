#pragma once

template <typename T>
class Vec2 {
public:
    T x = 0, y = 0;
public:
    Vec2(T x = 0, T y = 0) {
        this->x = x;
        this->y = y;
    };
};

template <typename T>
class Vec3 : public Vec2<T> {
public:
    T z = 0;
public:
    Vec3(T x = 0, T y = 0, T z = 0) : Vec2<T>(x, y) {
        this->z = z;
    };
};