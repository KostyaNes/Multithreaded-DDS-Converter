#pragma once

#include <ProjectDefines.h>

#include <DDSConversion/DDSDefines.h>
#include <DDSConversion/Color32.h>

#include <vector>


class ImageTexel
{
public:
    ImageTexel() = default;

public:
    Color32 m_pixelData[16];
};
