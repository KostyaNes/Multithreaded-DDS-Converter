#ifndef DDSPREVIEWERWIDGET_H
#define DDSPREVIEWERWIDGET_H

#include <Widgets/OpenGLWidget.h>
#include <DDSConversion/DDSDefines.h>

#include <QWidget>

namespace Ui {
class DDSPreviewerWidget;
}

class DDSPreviewerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DDSPreviewerWidget(QWidget *parent = nullptr);
    ~DDSPreviewerWidget();

    void SetTexture(QString& texturePath, TextureOrientation textureOrientation);

private slots:
    void on_textureOrientationComboBox_currentIndexChanged(int index);

private:
    void CreateOpenGLWidget();

private:
    Ui::DDSPreviewerWidget *ui;

    Texture m_texture;
    TextureOrientation m_textureOrientation { TextureOrientation::DirectX };
    OpenGLWidget* m_pOpenGLWidget { nullptr };
};

#endif // DDSPREVIEWERWIDGET_H
