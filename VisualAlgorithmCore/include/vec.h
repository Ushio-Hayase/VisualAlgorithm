//
// Created by UshioHayase on 2025-10-15.
//

#ifndef VISUALALGORITHMCORE_MATH_H
#define VISUALALGORITHMCORE_MATH_H

#include <cmath>

struct Vector2
{
    float x;
    float y;

    inline Vector2& operator+=(const Vector2& vec)
    {
        this->x += vec.x;
        this->y += vec.y;

        return *this;
    }

    inline Vector2& operator-=(const Vector2& vec)
    {
        this->x -= vec.x;
        this->y -= vec.y;

        return *this;
    }

    inline Vector2& operator*=(const float rhs)
    {
        this->x *= rhs;
        this->y *= rhs;

        return *this;
    }

    inline Vector2& operator/=(const float rhs)
    {
        this->x /= rhs;
        this->y /= rhs;

        return *this;
    }
};

struct Vector3
{
    float x;
    float y;
    float z;

    inline Vector3& operator+=(const Vector3& vec)
    {
        this->x += vec.x;
        this->y += vec.y;
        this->z += vec.z;

        return *this;
    }

    inline Vector3& operator-=(const Vector3& vec)
    {
        this->x -= vec.x;
        this->y -= vec.y;
        this->z -= vec.z;

        return *this;
    }

    inline Vector3& operator*=(const float rhs)
    {
        this->x *= rhs;
        this->y *= rhs;
        this->z *= rhs;

        return *this;
    }

    inline Vector3& operator/=(const float rhs)
    {
        this->x /= rhs;
        this->y /= rhs;
        this->z /= rhs;

        return *this;
    }
};

inline Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

inline Vector2 operator-(const Vector2& lhs, const Vector2& rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

inline Vector2 operator*(const float lhs, const Vector2& rhs)
{
    return {rhs.x * lhs, rhs.y * lhs};
}

inline Vector2 operator*(const Vector2& lhs, const float rhs)
{
    return {lhs.x * rhs, lhs.y * rhs};
}

inline Vector2 operator/(const Vector2& lhs, const float rhs)
{
    return {lhs.x / rhs, lhs.y / rhs};
}

inline Vector3 operator+(const Vector3& lhs, const Vector3& rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

inline Vector3 operator-(const Vector3& lhs, const Vector3& rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

inline Vector3 operator*(const float lhs, const Vector3& rhs)
{
    return {lhs * rhs.x, lhs * rhs.y, lhs * rhs.z};
}

inline Vector3 operator*(const Vector3& lhs, const float rhs)
{
    return {lhs.x * rhs, lhs.y * rhs, lhs.z * rhs};
}

inline Vector3 operator/(const Vector3& lhs, const float rhs)
{
    return {lhs.x / rhs, lhs.y / rhs, lhs.z / rhs};
}

inline float dot(const Vector2& lhs, const Vector2& rhs)
{
    return lhs.x * rhs.x + lhs.y + rhs.y;
}

inline float dot(const Vector3& lhs, const Vector3& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

inline float cross(const Vector2& lhs, const Vector2& rhs)
{
    return lhs.x * rhs.y - lhs.y * rhs.x;
}

inline Vector3 cross(const Vector3& lhs, const Vector3& rhs)
{
    return {lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x};
}

inline float norm(const Vector2& vec)
{
    return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}

inline float norm(const Vector3& vec)
{
    return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

#endif  // VISUALALGORITHMCORE_MATH_H
