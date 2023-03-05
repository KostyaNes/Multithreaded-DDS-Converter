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

    if (HasAlpha(texel) || m_firstColor == m_secondColor)
    {
        if (m_firstColor > m_secondColor)
        {
            std::swap(m_firstColor, m_secondColor);
            std::swap(colors[0], colors[1]);
        }
        colors[2] = Color32(colors[0].red * 1 / 2 + colors[1].red * 1 / 2, colors[0].green * 1 / 2
            + colors[1].green * 1 / 2, colors[0].blue * 1 / 2 + colors[1].blue * 1 / 2, 255);
        colors[3] = Color32(0, 0, 0, 0);

        CalculateContentIndicesBC1a(m_indices, texel, colors);
    }
    else
    {
        if (m_firstColor < m_secondColor)
        {
            std::swap(m_firstColor, m_secondColor);
            std::swap(colors[0], colors[1]);
        }
        colors[2] = Color32(colors[0].red * 2 / 3 + colors[1].red * 1 / 3, colors[0].green * 2 / 3
            + colors[1].green * 1 / 3, colors[0].blue * 2 / 3 + colors[1].blue * 1 / 3, 255);
        colors[3] = Color32(colors[0].red * 1 / 3 + colors[1].red * 2 / 3, colors[0].green * 1 / 3
            + colors[1].green * 2 / 3, colors[0].blue * 1 / 3 + colors[1].blue * 2 / 3, 255);

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

void BC1Block::CalculateContentIndicesBC1a(uint32_t& indices, const ImageTexel &texel, const Color32 (&mainColors)[4]) const
{
    indices = 0;
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

void BC2Block::Serialize(std::ofstream &fileStream)
{
    fileStream.write(reinterpret_cast<char*>(&m_alphaContent), sizeof(m_alphaContent));
    fileStream.write(reinterpret_cast<char*>(&m_firstColor), sizeof(m_firstColor));
    fileStream.write(reinterpret_cast<char*>(&m_secondColor), sizeof(m_secondColor));
    fileStream.write(reinterpret_cast<char*>(&m_indices), sizeof(m_indices));
}

void BC2Block::ConvertDataFromTexel(const ImageTexel &texel)
{
    Color32 colors[4];
    GetMainColors(texel, colors[0], colors[1]);
    colors[2] = Color32(colors[0].red * 2 / 3 + colors[1].red * 1 / 3, colors[0].green * 2 / 3
        + colors[1].green * 1 / 3, colors[0].blue * 2 / 3 + colors[1].blue * 1 / 3, 255);
    colors[3] = Color32(colors[0].red * 1 / 3 + colors[1].red * 2 / 3, colors[0].green * 1 / 3
        + colors[1].green * 2 / 3, colors[0].blue * 1 / 3 + colors[1].blue * 2 / 3, 255);

    m_firstColor = colors[0].GetRGB565();
    m_secondColor = colors[1].GetRGB565();

    CalculateContentIndicesBC2(m_indices, m_alphaContent, texel, colors);
}

void BC2Block::CalculateContentIndicesBC2(uint32_t &indices, uint64_t& alphaContent, const ImageTexel &texel, const Color32 (&mainColors)[4]) const
{
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

        alphaContent = alphaContent << 4;
        alphaContent += static_cast<uint64_t>(texel.m_pixelData[i].alpha >> 4);
    }
}

void BC3Block::Serialize(std::ofstream &fileStream)
{
    fileStream.write(reinterpret_cast<char*>(&m_alphaContent), sizeof(m_alphaContent));
    fileStream.write(reinterpret_cast<char*>(&m_firstColor), sizeof(m_firstColor));
    fileStream.write(reinterpret_cast<char*>(&m_secondColor), sizeof(m_secondColor));
    fileStream.write(reinterpret_cast<char*>(&m_indices), sizeof(m_indices));
}

void BC3Block::ConvertDataFromTexel(const ImageTexel &texel)
{
    Color32 colors[4];
    int alpha[8];

    GetMainColorsAndAlpha(texel, colors[0], colors[1], alpha[0], alpha[1]);

    colors[2] = Color32(colors[0].red * 2 / 3 + colors[1].red * 1 / 3, colors[0].green * 2 / 3
        + colors[1].green * 1 / 3, colors[0].blue * 2 / 3 + colors[1].blue * 1 / 3, 255);
    colors[3] = Color32(colors[0].red * 1 / 3 + colors[1].red * 2 / 3, colors[0].green * 1 / 3
        + colors[1].green * 2 / 3, colors[0].blue * 1 / 3 + colors[1].blue * 2 / 3, 255);

    m_firstColor = colors[0].GetRGB565();
    m_secondColor = colors[1].GetRGB565();

    if (alpha[0] < alpha[1])
    {
        std::swap(alpha[0], alpha[1]);
    }

    alpha[2] = (6 * alpha[0] + 1 * alpha[1]) / 7;
    alpha[3] = (5 * alpha[0] + 2 * alpha[1]) / 7;
    alpha[4] = (4 * alpha[0] + 3 * alpha[1]) / 7;
    alpha[5] = (3 * alpha[0] + 4 * alpha[1]) / 7;
    alpha[6] = (2 * alpha[0] + 5 * alpha[1]) / 7;
    alpha[7] = (1 * alpha[0] + 6 * alpha[1]) / 7;

    CalculateContentIndicesBC3(m_indices, m_alphaContent, texel, colors, alpha);

    m_alphaContent = m_alphaContent << 8;
    m_alphaContent += alpha[1];
    m_alphaContent = m_alphaContent << 8;
    m_alphaContent += alpha[0];
}

void BC3Block::CalculateContentIndicesBC3(uint32_t &indices, uint64_t& alphaContent, const ImageTexel &texel, const Color32 (&mainColors)[4], const int (&mainAlpha)[8]) const
{
    for (int i = 15; i >= 0; i--)
    {
        uint32_t pixelColorIndex = 0;
        float minColorDistance = texel.m_pixelData[i].CalculateColorDistanceRelative(mainColors[0]);

        for (uint32_t colorIndex = 1; colorIndex < 4; colorIndex++)
        {
            int distance = texel.m_pixelData[i].CalculateColorDistanceRelative(mainColors[colorIndex]);
            if (distance < minColorDistance)
            {
                pixelColorIndex = colorIndex;
                minColorDistance = distance;
            }
        }

        indices = indices << 2;
        indices += static_cast<uint32_t>(pixelColorIndex);

        uint64_t pixelAlphaIndex = 0;
        int minAlphaDistance = std::abs(texel.m_pixelData[i].alpha - mainAlpha[0]);

        for (uint64_t alphaIndex = 1; alphaIndex < 8; alphaIndex++)
        {
            int distance = std::abs(texel.m_pixelData[i].alpha - mainAlpha[alphaIndex]);
            if (distance < minAlphaDistance)
            {
                pixelAlphaIndex = alphaIndex;
                minAlphaDistance = distance;
            }
        }

        alphaContent = alphaContent << 3;
        alphaContent += static_cast<uint64_t>(pixelAlphaIndex);
    }
}

void BC3Block::GetMainColorsAndAlpha(const ImageTexel &texel, Color32 &firstColor, Color32 &secondColor, int &firstAlpha, int &secondAlpha) const
{
    float maxColorDif = 0.0f;
    firstColor = texel.m_pixelData[0];
    secondColor = texel.m_pixelData[0];

    int maxAlphaDif = 0;
    firstAlpha = texel.m_pixelData[0].alpha;
    secondAlpha = texel.m_pixelData[0].alpha;

    for (int i = 0; i < 16; i++)
    {
        for (int j = i + 1; j < 16; j++)
        {
            if (i != j)
            {
                float colorDif = texel.m_pixelData[i].CalculateColorDistanceRelative(texel.m_pixelData[j]);
                if (colorDif > maxColorDif)
                {
                    maxColorDif = colorDif;
                    firstColor = texel.m_pixelData[i];
                    secondColor = texel.m_pixelData[j];
                }

                int alphaDif = std::abs(texel.m_pixelData[i].alpha - texel.m_pixelData[j].alpha);
                if (alphaDif > maxAlphaDif)
                {
                    maxAlphaDif = alphaDif;
                    firstAlpha = texel.m_pixelData[i].alpha;
                    secondAlpha = texel.m_pixelData[j].alpha;
                }
            }
        }
    }
}
