#pragma once

#include <ProjectDefines.h>

#include <DDSConversion/Textures/ImageTexel.h>

#include <fstream>

class TextureBlock
{
public:
    virtual ~TextureBlock() = default;

    virtual void Serialize(std::ofstream& fileStream) = 0;
    virtual void ConvertDataFromTexel(const ImageTexel& texel) = 0;

protected:
    void GetMainColors(const ImageTexel& texel, Color32& firstColor, Color32& secondColor) const;
    uint32_t CalculateColorIndices(const ImageTexel& texel, const Color32(&mainColors)[4]) const;

protected:
    uint16_t m_firstColor { 0 };
    uint16_t m_secondColor { 0 };
    uint32_t m_indices { 0 };
};

class BC1Block
    : public TextureBlock
{
public:
    virtual void Serialize(std::ofstream& fileStream) override;
    virtual void ConvertDataFromTexel(const ImageTexel& texel) override;

private:
    bool HasAlpha(const ImageTexel& texel);
    uint32_t CalculateColorIndicesBC1a(const ImageTexel& texel, const Color32(&mainColors)[4]) const; // special case for 1-bit alpha
};

