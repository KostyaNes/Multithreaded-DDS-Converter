#pragma once

#include <ProjectDefines.h>

#include <DDSConversion/Textures/TextureBlock.h>
#include <atomic>

class TextureData
{
public:
    TextureData(CompressionType type,  int height, int width, int texelCount);
    ~TextureData();

    void ConvertDataCPU(std::vector<ImageTexel>& texelArray);
    void ConvertDataCPU(std::vector<ImageTexel>& texelArray, int texelBegin, int texelEnd);
    void Serialize(std::ofstream& fileStream);

private:
    std::vector<TextureBlock*> m_blockData;

    // Required to generate DDS header
    CompressionType m_compressionType { CompressionType::DXT1 };
    int m_height { 0 };
    int m_width { 0 };
};
