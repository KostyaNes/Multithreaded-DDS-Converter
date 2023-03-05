#include "TextureData.h"

#include <DDSConversion/Textures/ImageUtils.h>

TextureData::TextureData(CompressionType type, int height, int width)
    : m_compressionType(type)
    , m_height(height)
    , m_width(width)
{

}

void TextureData::ConvertDataCPU(std::vector<ImageTexel>& texelArray)
{
    //assertm(texelArray.size() == m_blockData.size(), "Converted data is not allocated or invalid. Data should be prealocated in client's code!");
    m_blockData.resize(texelArray.size());

    for (size_t index = 0; index < texelArray.size(); index++)
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

    for (size_t i = 0; i < texelArray.size(); i++)
    {
        m_blockData[i]->ConvertDataFromTexel(texelArray[i]);
    }

    m_bDataReady = true;
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


