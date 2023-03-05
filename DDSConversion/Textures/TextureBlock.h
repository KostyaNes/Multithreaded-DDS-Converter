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
    void CalculateContentIndicesBC1a(uint32_t& indices, const ImageTexel& texel, const Color32(&mainColors)[4]) const;
};

class BC2Block
    : public TextureBlock
{
public:
    virtual void Serialize(std::ofstream& fileStream) override;
    virtual void ConvertDataFromTexel(const ImageTexel& texel) override;

private:
    void CalculateContentIndicesBC2(uint32_t& indices, uint64_t& alphaContent, const ImageTexel& texel, const Color32(&mainColors)[4]) const;

private:
    uint64_t m_alphaContent { 0 };
};

class BC3Block
    : public TextureBlock
{
public:
    virtual void Serialize(std::ofstream& fileStream) override;
    virtual void ConvertDataFromTexel(const ImageTexel& texel) override;

private:
    void CalculateContentIndicesBC3(uint32_t& indices, uint64_t& alphaContent, const ImageTexel& texel, const Color32(&mainColors)[4], const int(&mainAlpha)[8]) const;
    void GetMainColorsAndAlpha(const ImageTexel& texel, Color32& firstColor, Color32& secondColor, int& firstAlpha, int& secondAlpha) const;

private:
    uint64_t m_alphaContent { 0 };
};

