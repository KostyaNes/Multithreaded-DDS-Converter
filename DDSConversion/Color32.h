#pragma once

#include <stdint.h>

class Color32
{
public:
    Color32() = default;
    Color32(int red, int green, int blue);
    Color32(int red, int green, int blue, int alpha);

    float CalculateColorDistance(const Color32 otherColor) const;
    float CalculateColorDistanceRelative(const Color32 otherColor) const; // Faster but can be used only when comparing two colors
    uint16_t GetRGB565() const;

public:
    int red { 0 };
    int green { 0 };
    int blue { 0 };
    int alpha { 0 };
};

inline bool operator< (const Color32& lhs, const Color32& rhs)
{
    return (lhs.red * lhs.red + lhs.green * lhs.green + lhs.blue * lhs.blue) <
        (rhs.red * rhs.red + rhs.green * rhs.green + rhs.blue * rhs.blue);
}
inline bool operator> (const Color32& lhs, const Color32& rhs) { return rhs < lhs; }
inline bool operator<=(const Color32& lhs, const Color32& rhs) { return !(lhs > rhs); }
inline bool operator>=(const Color32& lhs, const Color32& rhs) { return !(lhs < rhs); }

inline bool operator==(const Color32& lhs, const Color32& rhs)
{
    return (lhs.red * lhs.red + lhs.green * lhs.green + lhs.blue * lhs.blue) ==
        (rhs.red * rhs.red + rhs.green * rhs.green + rhs.blue * rhs.blue);
}
inline bool operator!=(const Color32& lhs, const Color32& rhs) { return !(lhs == rhs); }

