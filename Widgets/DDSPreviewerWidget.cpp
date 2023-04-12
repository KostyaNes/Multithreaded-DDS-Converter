#include "DDSPreviewerWidget.h"
#include "ui_DDSPreviewerWidget.h"

#include <DDSConversion/DDSFunc.h>

#include <math.h>

namespace WidgetSizeDefs
{
    const int maxW = 1280;
    const int maxH = 720;
    const int minW = 640;
    const int minH = 360;

    const int xOffset = 50;
    const int yOffset = 50;
}

DDSPreviewerWidget::DDSPreviewerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DDSPreviewerWidget)
{
    ui->setupUi(this);
}

DDSPreviewerWidget::~DDSPreviewerWidget()
{
    delete m_pOpenGLWidget;

    delete ui;
}

void DDSPreviewerWidget::SetTexture(QString& texturePath, TextureOrientation textureOrientation)
{
    ReadDDSTexture(texturePath.toStdString(), m_texture);
    m_textureOrientation = textureOrientation;

    ui->textureOrientationComboBox->setCurrentIndex(static_cast<int>(m_textureOrientation));

    CreateOpenGLWidget();
}

void DDSPreviewerWidget::CreateOpenGLWidget()
{
    if (m_pOpenGLWidget)
    {
        delete m_pOpenGLWidget;
    }

    m_pOpenGLWidget = new OpenGLWidget(this);

    int widgetH = m_texture.m_header.dwHeight;
    int widgetW = m_texture.m_header.dwWidth;

    if (widgetH > WidgetSizeDefs::maxH)
    {
        float multiplier = widgetH / WidgetSizeDefs::maxH;
        widgetH = WidgetSizeDefs::maxH;
        widgetW = round(widgetW / multiplier);
    }

    if (widgetW > WidgetSizeDefs::maxW)
    {
        float multiplier = widgetW / WidgetSizeDefs::maxW;
        widgetW = WidgetSizeDefs::maxW;
        widgetH = round(widgetH / multiplier);
    }

    if (widgetH < WidgetSizeDefs::minH)
    {
        float multiplier = WidgetSizeDefs::minH / widgetH;
        widgetH = WidgetSizeDefs::minH;
        widgetW = round(widgetW * multiplier);
    }

    if (widgetW < WidgetSizeDefs::minW)
    {
        float multiplier = WidgetSizeDefs::minW / widgetW;
        widgetW = WidgetSizeDefs::minW;
        widgetH = round(widgetH * multiplier);
    }

    this->resize(widgetW + WidgetSizeDefs::xOffset * 2,  widgetH + WidgetSizeDefs::yOffset * 2);

    m_pOpenGLWidget->resize(widgetW,  widgetH);
    m_pOpenGLWidget->move(WidgetSizeDefs::xOffset, WidgetSizeDefs::yOffset);
    m_pOpenGLWidget->SetTexture(&m_texture);
    m_pOpenGLWidget->SetTextureOrientation(m_textureOrientation);
}

void DDSPreviewerWidget::on_textureOrientationComboBox_currentIndexChanged(int index)
{
    TextureOrientation newOrientation = static_cast<TextureOrientation>(index);
    if (m_textureOrientation != newOrientation)
    {
        m_textureOrientation = newOrientation;
        CreateOpenGLWidget();
        /*
        if (m_pOpenGLWidget)
        {
            m_pOpenGLWidget->SetTextureOrientation(m_textureOrientation);
        }
        */
    }
}
