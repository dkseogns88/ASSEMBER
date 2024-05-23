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
    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }
    FVector normalized() const {
        float len = length();
        if (len != 0) {
            return { x / len, y / len, z / len};
        }
        return { 0, 0, 0 };
    }
};

