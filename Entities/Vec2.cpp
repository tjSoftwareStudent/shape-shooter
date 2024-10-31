#include "Vec2.h"

#include <cmath>

Vec2::Vec2()
{
}

Vec2::Vec2(float xin, float yin) : x(xin), y(yin)
{
}

bool Vec2::operator==(const Vec2 &rhs) const
{
    return x == rhs.x && y == rhs.y;
}

bool Vec2::operator!=(const Vec2 &rhs) const
{
    return x != rhs.x || y != rhs.y;
}

Vec2 Vec2::operator+(const Vec2 &rhs) const
{
    return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator-(const Vec2 &rhs) const
{
    return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator*(const float &value) const
{
    return Vec2(x * value, y * value);
}

Vec2 Vec2::operator/(const float &value) const
{
    return Vec2(x / value, y / value);
}

void Vec2::operator+=(const Vec2 &rhs)
{
    x += rhs.x;
    y += rhs.y;
}
void Vec2::operator-=(const Vec2 &rhs)
{
    x -= rhs.x;
    y -= rhs.y;
}

void Vec2::operator*=(const Vec2 &rhs)
{
    x *= rhs.x;
    y *= rhs.y;
}

void Vec2::operator/=(const Vec2 &rhs)
{
    x /= rhs.x;
    y /= rhs.y;
}

float Vec2::dist(const Vec2 &rhs) const
{
    auto difference = rhs - *this;

    return std::sqrt(difference.x * difference.x + difference.y * difference.y);
}

void Vec2::normalize()
{
    auto l = length();
    x /= l;
    y /= l;
}

float Vec2::length() const
{
    return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
}