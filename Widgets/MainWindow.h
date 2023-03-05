#pragma once

#include <QMainWindow>
#include <DDSConversion/DDSDefines.h>

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

private slots:
    void on_runBenchmarkButton_clicked();

    void on_convertImageButton_clicked();

    void on_convertFolderButton_clicked();

private:
    Ui::MainWindow *ui;
};

