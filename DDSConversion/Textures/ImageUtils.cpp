#include "ImageUtils.h"

Color32 CreateColor32FromQColor(const QColor& source)
{
    return Color32(source.red(), source.green(), source.blue(), source.alpha());
}

void ConvertQImageToTexelArray(const QImage& source, std::vector<ImageTexel>& texelArray)
{
    for (int i = 0; i < source.height(); i+=4)
    {
        for (int j = 0; j < source.width(); j+=4)
        {
            int pixelCount = 0;
            texelArray.push_back(ImageTexel());

            for (int y = i; y < i + 4; y++)
            {
                for (int x = j; x < j + 4; x++)
                {
                    if (y >= source.height() || x >= source.width())
                    {
                        texelArray.back().m_pixelData[pixelCount] = Color32(0, 0, 0, 0);
                    }
                    else
                    {
                        texelArray.back().m_pixelData[pixelCount] = CreateColor32FromQColor(source.pixelColor(x, y));
                    }
                    pixelCount++;
                }
            }
        }
    }
}

void FillDDSHeader(DDS_HEADER &header, CompressionType type, int width, int height)
{
    header.dwMagic = ('D' << 0) | ('D' << 8) | ('S' << 16) | (' ' << 24);
    header.dwSize = 124;
    header.dwFlags = (DDSD_CAPS) | (DDSD_HEIGHT) | (DDSD_WIDTH) | (DDSD_PIXELFORMAT) | (DDSD_LINEARSIZE);
    header.dwHeight = static_cast<DWORD>(height);
    header.dwWidth = static_cast<DWORD>(width);

    switch (type)
    {
        case (CompressionType::DXT1):
        {
            header.dwPitchOrLinearSize = static_cast<DWORD>(std::max(1, ((width + 3) / 4)) * 8);
            header.ddspf.dwFourCC = ('D' << 0) | ('X' << 8) | ('T' << 16) | ('1' << 24);
            break;
        }
        case (CompressionType::DXT3):
        {
            header.dwPitchOrLinearSize = static_cast<DWORD>(std::max(1, ((width + 3) / 4)) * 16);
            header.ddspf.dwFourCC = ('D' << 0) | ('X' << 8) | ('T' << 16) | ('3' << 24);
            break;
        }
        case (CompressionType::DXT5):
        {
            header.dwPitchOrLinearSize = static_cast<DWORD>(std::max(1, ((width + 3) / 4)) * 16);
            header.ddspf.dwFourCC = ('D' << 0) | ('X' << 8) | ('T' << 16) | ('5' << 24);
            break;
        }
    }

    header.ddspf.dwSize = 32;
    header.ddspf.dwFlags = DDPF_FOURCC;
    header.dwCaps = DDSD_CAPS;
}
