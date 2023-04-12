#pragma once

#include <QImage>
#include <QColor>

#include <DDSConversion/Textures/ImageTexel.h>

Color32 CreateColor32FromQColor(const QColor& source);
void ConvertQImageToTexelArray(const QImage& source, std::vector<ImageTexel>& texelArray, TextureOrientation textureOrientation);

void FillDDSHeader(DDS_HEADER& header, CompressionType type, int width, int height);
