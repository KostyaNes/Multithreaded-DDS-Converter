#pragma once

#include <windows.h>

#define DDSD_CAPS           0x1
#define DDSD_HEIGHT         0x2
#define DDSD_WIDTH          0x4
#define DDSD_PIXELFORMAT    0x1000
#define DDSD_LINEARSIZE     0x80000
#define DDSCAPS_TEXTURE     0x1000
#define DDPF_FOURCC         0x4

enum class CompressionType
{
    DXT1,
    DXT3,
    DXT5
};

struct DDS_PIXELFORMAT
{
    DWORD dwSize{ 0 };
    DWORD dwFlags{ 0 };
    DWORD dwFourCC{ 0 };
    DWORD dwRGBBitCount{ 0 };
    DWORD dwRBitMask{ 0 };
    DWORD dwGBitMask{ 0 };
    DWORD dwBBitMask{ 0 };
    DWORD dwABitMask{ 0 };
};

struct DDS_HEADER
{
    DWORD           dwMagic{ 0 };
    DWORD           dwSize{ 0 };
    DWORD           dwFlags{ 0 };
    DWORD           dwHeight{ 0 };
    DWORD           dwWidth{ 0 };
    DWORD           dwPitchOrLinearSize{ 0 };
    DWORD           dwDepth{ 0 };
    DWORD           dwMipMapCount{ 0 };
    DWORD           dwReserved1[11];
    DDS_PIXELFORMAT ddspf;
    DWORD           dwCaps{ 0 };
    DWORD           dwCaps2{ 0 };
    DWORD           dwCaps3{ 0 };
    DWORD           dwCaps4{ 0 };
    DWORD           dwReserved2{ 0 };
};
