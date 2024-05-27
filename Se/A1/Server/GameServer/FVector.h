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

    bool operator==(const FVector& other)
    {
        return y == other.y && x == other.x && z == other.z;
    }

    bool operator!=(const FVector& other)
    {
        return !(*this == other);
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
};

