#include "TextureBlock.h"

#include "ImageTexel.h"

#include <fstream>

void BC1Block::Serialize(std::ofstream& fileStream)
{
    fileStream.write(reinterpret_cast<char*>(&m_firstColor), sizeof(m_firstColor));
    fileStream.write(reinterpret_cast<char*>(&m_secondColor), sizeof(m_secondColor));
    fileStream.write(reinterpret_cast<char*>(&m_indices), sizeof(m_indices));
}

void BC1Block::ConvertDataFromTexel(const ImageTexel& texel)
{
    Color32 colors[4];
    GetMainColors(texel, colors[0], colors[1]);

    m_firstColor = colors[0].GetRGB565();
    m_secondColor = colors[1].GetRGB565();

    if (HasAlpha(texel))
    {
        //if (m_firstColor > m_secondColor)
        if (colors[0] > colors[1])
        {
            std::swap(m_firstColor, m_secondColor);
            std::swap(colors[0], colors[1]);
        }
        colors[2] = Color32(colors[0].red * 1 / 2 + colors[1].red * 1 / 2, colors[0].green * 1 / 2
            + colors[1].green * 1 / 2, colors[0].blue * 1 / 2 + colors[1].blue * 1 / 2);
        colors[3] = Color32(0, 0, 0, 0);

        m_indices = CalculateColorIndicesBC1a(texel, colors);
    }
    else
    {
        if (m_firstColor < m_secondColor)
        {
            std::swap(m_firstColor, m_secondColor);
            std::swap(colors[0], colors[1]);
        }
        colors[2] = Color32(colors[0].red * 2 / 3 + colors[1].red * 1 / 3, colors[0].green * 2 / 3
            + colors[1].green * 1 / 3, colors[0].blue * 2 / 3 + colors[1].blue * 1 / 3);
        colors[3] = Color32(colors[0].red * 1 / 3 + colors[1].red * 2 / 3, colors[0].green * 1 / 3
            + colors[1].green * 2 / 3, colors[0].blue * 1 / 3 + colors[1].blue * 2 / 3);

        m_indices = CalculateColorIndices(texel, colors);
    }
}

bool BC1Block::HasAlpha(const ImageTexel& texel)
{
    for (int i = 0; i < 16; i++)
    {
        if (texel.m_pixelData[i].alpha == 0)
        {
            return true;
        }
    }
    return false;
}

uint32_t BC1Block::CalculateColorIndicesBC1a(const ImageTexel &texel, const Color32 (&mainColors)[4]) const
{
    uint32_t indices = 0;
    for (int i = 15; i >= 0; i--)
    {
        uint32_t pixelColorIndex = 0;
        if (texel.m_pixelData[i].alpha == 0)
        {
            pixelColorIndex = 3;
        }
        else
        {
            int minDistance = texel.m_pixelData[i].CalculateColorDistanceRelative(mainColors[0]);
            for (uint32_t colorIndex = 1; colorIndex < 3; colorIndex++)
            {
                int distance = texel.m_pixelData[i].CalculateColorDistanceRelative(mainColors[colorIndex]);
                if (distance < minDistance)
                {
                    pixelColorIndex = colorIndex;
                    minDistance = distance;
                }
            }
        }

        indices = indices << 2;
        indices += static_cast<uint32_t>(pixelColorIndex);
    }

    return indices;
}

void TextureBlock::GetMainColors(const ImageTexel& texel, Color32& firstColor, Color32& secondColor) const
{
    float max = 0;
    firstColor = texel.m_pixelData[0];
    secondColor = texel.m_pixelData[0];

    for (int i = 0; i < 16; i++)
    {
        for (int j = i + 1; j < 16; j++)
        {
            if (i != j)
            {
                float res = texel.m_pixelData[i].CalculateColorDistanceRelative(texel.m_pixelData[j]);
                if (res > max)
                {
                    max = res;
                    firstColor = texel.m_pixelData[i];
                    secondColor = texel.m_pixelData[j];
                }
            }
        }
    }
}

uint32_t TextureBlock::CalculateColorIndices(const ImageTexel& texel, const Color32(&mainColors)[4]) const
{
    uint32_t indices = 0;
    for (int i = 15; i >= 0; i--)
    {
        uint32_t pixelColorIndex = 0;
        float minDistance = texel.m_pixelData[i].CalculateColorDistanceRelative(mainColors[0]);

        for (uint32_t colorIndex = 1; colorIndex < 4; colorIndex++)
        {
            int distance = texel.m_pixelData[i].CalculateColorDistanceRelative(mainColors[colorIndex]);
            if (distance < minDistance)
            {
                pixelColorIndex = colorIndex;
                minDistance = distance;
            }
        }

        indices = indices << 2;
        indices += static_cast<uint32_t>(pixelColorIndex);
    }

    return indices;
}
