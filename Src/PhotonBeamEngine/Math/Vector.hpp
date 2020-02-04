#pragma once

#include <cmath>
#include <iostream>

namespace Pht {
    const float Pi = 4 * std::atan(1.0f);
    const float TwoPi = 2 * Pi;

    template <typename T>
    struct Vector2 {
        Vector2() {}
        Vector2(T x, T y) : x(x), y(y) {}

        T Dot(const Vector2& v) const {
            return x * v.x + y * v.y;
        }

        Vector2 operator+(const Vector2& v) const {
            return Vector2(x + v.x, y + v.y);
        }

        Vector2 operator-(const Vector2& v) const {
            return Vector2(x - v.x, y - v.y);
        }

        Vector2 operator-() const {
            return Vector2(-x, -y);
        }

        void operator+=(const Vector2& v) {
            *this = Vector2(x + v.x, y + v.y);
        }

        void operator-=(const Vector2& v) {
            *this = Vector2(x - v.x, y - v.y);
        }

        Vector2 operator/(float s) const {
            return Vector2(x / s, y / s);
        }

        Vector2 operator*(float s) const {
            return Vector2(x * s, y * s);
        }

        void operator/=(float s) {
            *this = Vector2(x / s, y / s);
        }

        void operator*=(float s) {
            *this = Vector2(x * s, y * s);
        }

        void Normalize() {
            float s = 1.0f / Length();
            x *= s;
            y *= s;
        }

        Vector2 Normalized() const {
            Vector2 v = *this;
            v.Normalize();
            return v;
        }

        T LengthSquared() const {
            return x * x + y * y;
        }

        T Length() const {
            return sqrt(LengthSquared());
        }

        operator Vector2<float>() const {
            return Vector2<float>(x, y);
        }

        bool operator==(const Vector2& v) const {
            return x == v.x && y == v.y;
        }
        
        bool operator!=(const Vector2& v) const {
            return !(*this == v);
        }

        Vector2 Lerp(float t, const Vector2& v) const {
            return Vector2(x * (1 - t) + v.x * t,
                           y * (1 - t) + v.y * t);
        }

        T* Write(T* ptr) const {
            *ptr++ = x;
            *ptr++ = y;
            return ptr;
        }

        T x;
        T y;
    };

    template <typename T>
    struct Vector3 {
        Vector3() {}
        Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

        void Normalize() {
            float s = 1.0f / std::sqrt(x * x + y * y + z * z);
            x *= s;
            y *= s;
            z *= s;
        }

        Vector3 Normalized() const {
            Vector3 v = *this;
            v.Normalize();
            return v;
        }
        
        T LengthSquared() const {
            return x * x + y * y + z * z;
        }
        
        T Length() const {
            return sqrt(LengthSquared());
        }

        Vector3 Cross(const Vector3& v) const {
            return Vector3(y * v.z - z * v.y,
                           z * v.x - x * v.z,
                           x * v.y - y * v.x);
        }

        T Dot(const Vector3& v) const {
            return x * v.x + y * v.y + z * v.z;
        }

        Vector3 operator+(const Vector3& v) const {
            return Vector3(x + v.x, y + v.y, z + v.z);
        }

        Vector3 operator*(const Vector3& v) const {
            return Vector3(x * v.x, y * v.y, z * v.z);
        }

        void operator*=(float s) {
            *this = Vector3(x * s, y * s, z * s);
        }

        void operator+=(const Vector3& v) {
            x += v.x;
            y += v.y;
            z += v.z;
        }

        void operator-=(const Vector3& v) {
            x -= v.x;
            y -= v.y;
            z -= v.z;
        }
        
        void operator*=(const Vector3& v) {
            x *= v.x;
            y *= v.y;
            z *= v.z;
        }

        void operator/=(T s) {
            x /= s;
            y /= s;
            z /= s;
        }

        Vector3 operator-(const Vector3& v) const {
            return Vector3(x - v.x, y - v.y, z - v.z);
        }

        Vector3 operator-() const {
            return Vector3(-x, -y, -z);
        }

        Vector3 operator*(T s) const {
            return Vector3(x * s, y * s, z * s);
        }

        Vector3 operator/(T s) const {
            return Vector3(x / s, y / s, z / s);
        }

        bool operator==(const Vector3& v) const {
            return x == v.x && y == v.y && z == v.z;
        }

        bool operator!=(const Vector3& v) const {
            return !(*this == v);
        }

        Vector3 Lerp(float t, const Vector3& v) const {
            return Vector3(x * (1 - t) + v.x * t,
                           y * (1 - t) + v.y * t,
                           z * (1 - t) + v.z * t);
        }

        const T* Pointer() const {
            return &x;
        }
        
        T* Write(T* ptr) const {
            *ptr++ = x;
            *ptr++ = y;
            *ptr++ = z;
            return ptr;
        }

        T x;
        T y;
        T z;
    };

    template <typename T>
    struct Vector4 {
        Vector4() {}
        Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
        Vector4(const Vector3<T>& v, T w) : x(v.x), y(v.y), z(v.z), w(w) {}
        
        Vector3<T> ToVec3() const {
            return Vector3<T> {x, y, z};
        }

        T Dot(const Vector4& v) const {
            return x * v.x + y * v.y + z * v.z + w * v.w;
        }

        Vector4 operator-() const {
            return Vector4(-x, -y, -z, -w);
        }

        Vector4 Lerp(float t, const Vector4& v) const {
            return Vector4(x * (1 - t) + v.x * t,
                           y * (1 - t) + v.y * t,
                           z * (1 - t) + v.z * t,
                           w * (1 - t) + v.w * t);
        }

        Vector4 operator+(const Vector4& v) const {
            return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
        }
        
        Vector4 operator-(const Vector4& v) const {
            return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
        }

        Vector4 operator*(T s) const {
            return Vector4(x * s, y * s, z * s, w * s);
        }
        
        Vector4 operator/(T s) const {
            return Vector4(x / s, y / s, z / s, w / s);
        }
        
        bool operator==(const Vector4& v) const {
            return x == v.x && y == v.y && z == v.z && w == v.w;
        }
        
        bool operator!=(const Vector4& v) const {
            return !(*this == v);
        }

        const T* Pointer() const {
            return &x;
        }
        
        T* Write(T* ptr) const {
            *ptr++ = x;
            *ptr++ = y;
            *ptr++ = z;
            *ptr++ = w;
            return ptr;
        }
        
        void Print() const {
            std::cout << x << ", " << y << ", " << z << ", " << w << std::endl;
        }
        
        T x;
        T y;
        T z;
        T w;
    };

    using IVec2 = Vector2<int>;
    using IVec3 = Vector3<int>;
    using IVec4 = Vector4<int>;

    using Vec2 = Vector2<float>;
    using Vec3 = Vector3<float>;
    using Vec4 = Vector4<float>;
}
