#include "Color32.h"

#include <math.h>

Color32::Color32(int red, int green, int blue)
{
    this->red = red;
    this->green = green;
    this->blue = blue;
}

Color32::Color32(int red, int green, int blue, int alpha)
{
    this->red = red;
    this->green = green;
    this->blue = blue;
    this->alpha = alpha;
}

float Color32::CalculateColorDistance(const Color32 otherColor) const
{
    return sqrtf(
        (red - otherColor.red) * (red - otherColor.red) +
        (green - otherColor.green) * (green - otherColor.green) +
        (blue - otherColor.blue) * (blue - otherColor.blue)
    );
}

float Color32::CalculateColorDistanceRelative(const Color32 otherColor) const
{
    return
        (red - otherColor.red) * (red - otherColor.red) +
        (green - otherColor.green) * (green - otherColor.green) +
        (blue - otherColor.blue) * (blue - otherColor.blue)
    ;
}

unsigned short Color32::GetRGB565() const
{
    unsigned short B = (blue >> 3);
    unsigned short G = ((green >> 2) << 5);
    unsigned short R = ((red >> 3) << 11);

    return static_cast<unsigned short int> (R | G | B);
}
