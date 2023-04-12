#pragma once

#include <QMainWindow>
#include <DDSConversion/DDSDefines.h>

class QTableWidgetItem;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    CompressionType GetCurrentCompressionType();
    TextureOrientation GetCurrentTextureOrientation();

    void AddProcessedTextures(std::vector<std::pair<std::string, std::string>> imageFiles);

    void DisplayPreviewerWidget(QString filePath);

private slots:
    void on_runBenchmarkButton_clicked();

    void on_convertImageButton_clicked();

    void on_convertFolderButton_clicked();

    void on_processedTexturesTable_cellDoubleClicked(int row, int column);

    void on_openTextureButton_clicked();

private:
    Ui::MainWindow *ui;
};

