#include "OpenGLWidget.h"

#include <DDSConversion/DDSDefines.h>

#include <QtWidgets>
#include <QOpenGLFunctions_4_0_Core>

#include <QDebug>

namespace Shaders
{
    const char *VertexShader =
      "#ifdef GL_ES\n"
      "// Set default precision to medium\n"
      "precision mediump int;\n"
      "precision mediump float;\n"
      "#endif\n"
      "\n"
      "uniform mat4 mvp_matrix;\n"
      "\n"
      "attribute vec4 a_position;\n"
      "attribute vec2 a_texcoord;\n"
      "\n"
      "varying vec2 v_texcoord;\n"
      "\n"
      "void main()\n"
      "{\n"
      "  // Calculate vertex position in screen space\n"
      "  gl_Position = mvp_matrix * a_position;\n"
      "\n"
      "  // Pass texture coordinate to fragment shader\n"
      "  // Value will be automatically interpolated to fragments inside polygon faces\n"
      "  v_texcoord = a_texcoord;\n"
      "}\n";

    const char *DirectXFragmentShader =
      "#ifdef GL_ES\n"
      "// Set default precision to medium\n"
      "precision mediump int;\n"
      "precision mediump float;\n"
      "#endif\n"
      "\n"
      "uniform sampler2D texture;\n"
      "\n"
      "varying vec2 v_texcoord;\n"
      "\n"
      "void main()\n"
      "{\n"
      "  // Set fragment color from texture\n"
      "#if 0 // test check tex coords\n"
      "  gl_FragColor = vec4(1, v_texcoord.x, v_texcoord.y, 1);\n"
      "#else // (not) 0;\n"
      "  vec4 color = texture2D(texture, vec2(v_texcoord.x, 1 - v_texcoord.y));\n"
      "  //if (color.a < 0.01) discard;\n"
      "  gl_FragColor = color;\n"
      "#endif // 0\n"
      "}\n";

    const char *OpenGLFragmentShader =
      "#ifdef GL_ES\n"
      "// Set default precision to medium\n"
      "precision mediump int;\n"
      "precision mediump float;\n"
      "#endif\n"
      "\n"
      "uniform sampler2D texture;\n"
      "\n"
      "varying vec2 v_texcoord;\n"
      "\n"
      "void main()\n"
      "{\n"
      "  // Set fragment color from texture\n"
      "#if 0 // test check tex coords\n"
      "  gl_FragColor = vec4(1, v_texcoord.x, v_texcoord.y, 1);\n"
      "#else // (not) 0;\n"
      "  vec4 color = texture2D(texture, vec2(v_texcoord.x, v_texcoord.y));\n"
      "  //if (color.a < 0.01) discard;\n"
      "  gl_FragColor = color;\n"
      "#endif // 0\n"
      "}\n";
}

void OpenGLWidget::SetTexture(Texture* pTexture)
{
    m_pTexture = pTexture;
}

void OpenGLWidget::SetTextureOrientation(TextureOrientation textureOrientation)
{
    m_textureOrientation = textureOrientation;
    hide();
    show();
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.5, 0.5, 0.5, 0);

    initShaders();
    initGeometry();
    initTexture();
}
void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _pQGLTex->bind();
    QMatrix4x4 mat; mat.setToIdentity();
    _qGLSProg.setUniformValue("mvp_matrix", mat);
    _qGLSProg.setUniformValue("texture", 0);
    // draw geometry
    _qGLBufArray.bind();
    _qGLBufIndex.bind();
    quintptr offset = 0;
    int coordLocation = _qGLSProg.attributeLocation("a_position");
    _qGLSProg.enableAttributeArray(coordLocation);
    _qGLSProg.setAttributeBuffer(coordLocation, GL_FLOAT, offset, 3, sizeof(Vertex));
    offset += sizeof(QVector3D);
    int texCoordLocation = _qGLSProg.attributeLocation("a_texcoord");
    _qGLSProg.enableAttributeArray(texCoordLocation);
    _qGLSProg.setAttributeBuffer(texCoordLocation, GL_FLOAT, offset, 2, sizeof(Vertex));
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);
}

void OpenGLWidget::initShaders()
{
    if (!_qGLSProg.addShaderFromSourceCode(QOpenGLShader::Vertex, QString::fromLatin1(Shaders::VertexShader)))
    {
      parentWidget()->hide();
    }

    if (m_textureOrientation == TextureOrientation::OpenGL)
    {
      if (!_qGLSProg.addShaderFromSourceCode(QOpenGLShader::Fragment, QString::fromLatin1(Shaders::OpenGLFragmentShader)))
      {
          parentWidget()->hide();
      }
    }
    else
    {
      if (!_qGLSProg.addShaderFromSourceCode(QOpenGLShader::Fragment, QString::fromLatin1(Shaders::DirectXFragmentShader)))
      {
          parentWidget()->hide();
      }
    }

    if (!_qGLSProg.link()) parentWidget()->hide();
    if (!_qGLSProg.bind()) parentWidget()->hide();
}

void OpenGLWidget::initGeometry()
{
    Vertex vertices[] = {
        //  x      y      z     s     t
        { -1.0f, -1.0f, 0.0f, 0.0f, 0.0f },
        { +1.0f, -1.0f, 0.0f, 1.0f, 0.0f },
        { +1.0f, +1.0f, 0.0f, 1.0f, 1.0f },
        { -1.0f, +1.0f, 0.0f, 0.0f, 1.0f }
    };
    enum { nVtcs = sizeof vertices / sizeof *vertices };
    GLushort indices[] = { 3, 0, 2, 1 };
    enum { nIdcs = sizeof indices / sizeof *indices };
    _qGLBufArray.create();
    _qGLBufArray.bind();
    _qGLBufArray.allocate(vertices, nVtcs * sizeof (Vertex));
    _qGLBufIndex.create();
    _qGLBufIndex.bind();
    _qGLBufIndex.allocate(indices, nIdcs * sizeof (GLushort));
}

void OpenGLWidget::initTexture()
{
    _pQGLTex = new QOpenGLTexture(QOpenGLTexture::Target2D);

    int dataSizeMultiplier = 0;
    const DWORD dxt1 = ('D' << 0) | ('X' << 8) | ('T' << 16) | ('1' << 24);
    const DWORD dxt3 = ('D' << 0) | ('X' << 8) | ('T' << 16) | ('3' << 24);
    const DWORD dxt5 = ('D' << 0) | ('X' << 8) | ('T' << 16) | ('5' << 24);

    _pQGLTex->setSize(m_pTexture->m_header.dwWidth, m_pTexture->m_header.dwHeight);

    switch (m_pTexture->m_header.ddspf.dwFourCC)
    {
        case (dxt1):
        {
            dataSizeMultiplier = 8;
            _pQGLTex->setFormat(QOpenGLTexture::RGBA_DXT1);
            _pQGLTex->allocateStorage(QOpenGLTexture::RGBA, QOpenGLTexture::UInt16_RGB5A1);
            break;
        }
        case(dxt3):
        {
            dataSizeMultiplier = 16;
            _pQGLTex->setFormat(QOpenGLTexture::RGBA_DXT3);
            _pQGLTex->allocateStorage(QOpenGLTexture::RGBA, QOpenGLTexture::UInt32);
            break;
        }
        case(dxt5):
        {
            dataSizeMultiplier = 16;
            _pQGLTex->setFormat(QOpenGLTexture::RGBA_DXT5);
            _pQGLTex->allocateStorage(QOpenGLTexture::RGBA, QOpenGLTexture::UInt32);
            break;
        }
    }

    int dataSize = (m_pTexture->m_header.dwWidth + (4 - m_pTexture->m_header.dwWidth % 4) % 4) * (m_pTexture->m_header.dwHeight + (4 - m_pTexture->m_header.dwHeight % 4) % 4) / 16 * dataSizeMultiplier;
    _pQGLTex->setCompressedData(dataSize, reinterpret_cast<const void*>(m_pTexture->m_data));

    _pQGLTex->setMinificationFilter(QOpenGLTexture::Nearest);
    _pQGLTex->setMagnificationFilter(QOpenGLTexture::Nearest);
    _pQGLTex->setWrapMode(QOpenGLTexture::ClampToEdge);
}
