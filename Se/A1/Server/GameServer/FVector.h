#pragma once

struct FVector
{
	float x, y, z;

    FVector operator+(const FVector& other) const {
        return { x + other.x, y + other.y, z + other.z };
    }
    FVector operator-(const FVector& other) const {
        return { x - other.x, y - other.y, z - other.z };
    }
    FVector operator*(float scalar) const {
        return { x * scalar, y * scalar, z * scalar };
    }

    FVector operator/(float scalar) const {
        return { x / scalar, y / scalar, z / scalar };
    }

    bool operator==(const FVector& other)
    {
        return y == other.y && x == other.x && z == other.z;
    }

    bool operator==(const FVector& other) const
    {
        return y == other.y && x == other.x && z == other.z;
    }

    bool operator!=(const FVector& other)
    {
        return !(*this == other);
    }

    bool operator < (const FVector& vec) const
    {
        if (x != vec.x)
            return x < vec.x;
        else if (y != vec.y)
            return y < vec.y;
        else if (z != vec.z)
            return z < vec.z;
        else
            return false;
    }

    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    float lengthXY() const {
        return std::sqrt(x * x + y * y);
    }

    FVector normalized() const {
        float len = length();
        if (len != 0) {
            return { x / len, y / len, z / len};
        }
        return { 0, 0, 0 };
    }

    float DistanceTo2D(const FVector& v)
    {
        float _x = x - v.x;
        float _y = y - v.y;
        return std::sqrt(_x * _x + _y * _y);

    }
};

struct FVectorHash {
    std::size_t operator()(const FVector& v) const {
        std::size_t hx = std::hash<float>()(v.x);
        std::size_t hy = std::hash<float>()(v.y);
        std::size_t hz = std::hash<float>()(v.z);
        return hx ^ (hy << 1) ^ (hz << 2); // 간단한 해시 조합
    }
};