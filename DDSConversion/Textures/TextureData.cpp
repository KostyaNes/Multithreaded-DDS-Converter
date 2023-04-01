#include "TextureData.h"

#include <DDSConversion/Textures/ImageUtils.h>

TextureData::TextureData(CompressionType type, int height, int width, int texelCount)
    : m_compressionType(type)
    , m_height(height)
    , m_width(width)
{
    m_blockData.resize(texelCount);

    for (int index = 0; index < texelCount; index++)
    {
        switch (m_compressionType)
        {
        case CompressionType::DXT1:
            m_blockData[index] = new BC1Block();
            break;
        case CompressionType::DXT3:
            m_blockData[index] = new BC2Block();
            break;
        case CompressionType::DXT5:
            m_blockData[index] = new BC3Block();
            break;
        default:
            break;
        }
    }
}

TextureData::~TextureData()
{
    for (TextureBlock* pBlock : m_blockData)
    {
        delete pBlock;
    }
}

void TextureData::ConvertDataCPU(std::vector<ImageTexel>& texelArray)
{
    for (size_t i = 0; i < texelArray.size(); i++)
    {
        m_blockData[i]->ConvertDataFromTexel(texelArray[i]);
    }
}

void TextureData::ConvertDataCPU(std::vector<ImageTexel> &texelArray, int texelBegin, int texelEnd)
{
    for (int i = texelBegin; i < texelEnd; i++)
    {
        m_blockData[i]->ConvertDataFromTexel(texelArray[i]);
    }
}

void TextureData::Serialize(std::ofstream& fileStream)
{
    DDS_HEADER header;
    FillDDSHeader(header, m_compressionType, m_width, m_height);
    fileStream.write(reinterpret_cast<char*>(&header), sizeof(DDS_HEADER));

    for (TextureBlock* pTextureBlock : m_blockData)
    {
        pTextureBlock->Serialize(fileStream);
    }
}


