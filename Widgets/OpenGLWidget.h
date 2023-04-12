#pragma once

#include <DDSConversion/DDSDefines.h>

#include <QtWidgets>
#include <QOpenGLFunctions_4_0_Core>

#include <QDebug>

struct Vertex
{
   Vertex(qreal x, qreal y, qreal z, qreal s, qreal t):
     coord(x, y, z), texCoord(s, t)
   {

   }

   QVector3D coord;
   QVector2D texCoord;
};

class OpenGLWidget
        : public QOpenGLWidget
        , public QOpenGLFunctions_4_0_Core
{
public:
    explicit OpenGLWidget(QWidget* pParent):
      QOpenGLWidget(pParent),
      _qGLBufArray(QOpenGLBuffer::VertexBuffer),
      _qGLBufIndex(QOpenGLBuffer::IndexBuffer),
      _pQGLTex(nullptr)
    { }

    virtual ~OpenGLWidget()
    {
      makeCurrent();
      delete _pQGLTex;
      _qGLBufArray.destroy();
      _qGLBufIndex.destroy();
      doneCurrent();
    }

    OpenGLWidget(const OpenGLWidget&) = delete;
    OpenGLWidget& operator=(const OpenGLWidget&) = delete;

    void SetTexture(Texture* pTexture);
    void SetTextureOrientation(TextureOrientation textureOrientation);

protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;

private:
    void initShaders();
    void initGeometry();

    void initTexture();

private:

    Texture* m_pTexture { nullptr };
    TextureOrientation m_textureOrientation { TextureOrientation::DirectX };

    QOpenGLShaderProgram _qGLSProg;
    QOpenGLBuffer _qGLBufArray;
    QOpenGLBuffer _qGLBufIndex;
    QOpenGLTexture *_pQGLTex;
};
