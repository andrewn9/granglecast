/*
 * vector2.h 
 * Provides the Vector2 struct and basic arithmetic operations for vector addition, subtraction, multiplication, and division.
 */

#pragma once

struct Vector2 {
    float x;
    float y;

    /*
     * Operator overloads for vector addition, subtraction, multiplication, and division.
     */

    // Vector addition
    Vector2 operator+(const Vector2& other) const {
        return {x + other.x, y + other.y};
    }

    // Vector subtraction
    Vector2 operator-(const Vector2& other) const {
        return {x - other.x, y - other.y};
    }

    // Scalar multiplication
    Vector2 operator*(float scalar) const {
        return {x * scalar, y * scalar};
    }

    // Scalar multiplication
    Vector2 operator*(const Vector2& other) const {
        return {x * other.x, y * other.y};
    }

    // Scalar division
    Vector2 operator/(float scalar) const {
        return {x / scalar, y / scalar};
    }

    // Compound assignment for vector addition
    Vector2& operator+=(const Vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    // Compound assignment for vector subtraction
    Vector2& operator-=(const Vector2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    // Compound assignment for scalar multiplication
    Vector2& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    // Compound assignment for scalar division
    Vector2& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    Vector2 Lerp(const Vector2& other, const float factor) {
        return Vector2{x + (other.x - x) * factor, y + (other.y - y) * factor};
    }
};
