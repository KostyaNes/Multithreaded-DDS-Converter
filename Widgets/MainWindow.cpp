#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDir>

#include <DDSConversion/DDSFunc.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_runBenchmarkButton_clicked()
{
    QString benchmarkInputFolder = "C:/Users/Admin/Desktop/MConverter/Multithreaded-DDS-Converter/BenchmarkImages/Folder_1";
    QString benchmarkOutputFolder = "C:/Users/Admin/Desktop/MConverter/Multithreaded-DDS-Converter/BenchmarkImages/Output";

    QDir inputFolder(benchmarkInputFolder);

    if(!inputFolder.entryList().isEmpty())
    {
        QStringList images = inputFolder.entryList(QStringList() << "*.jpg" << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp" << "*.tif" << "*.tiff", QDir::Files);

        std::vector<std::pair<std::string, std::string>> imagesPath;

        for (const auto& image : images)
        {
            QStringList pieces = image.split(".");
            QString fileName = pieces.value(0);
            QString outputFileName = benchmarkOutputFolder + "/" + fileName + ".dds";

            imagesPath.push_back({inputFolder.absoluteFilePath(image).toStdString(), outputFileName.toStdString()});
        }

        ConvertImagesIntoDDS(imagesPath, CompressionType::DXT1);
    }
}
