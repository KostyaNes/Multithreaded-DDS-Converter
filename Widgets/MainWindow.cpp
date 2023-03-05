#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDir>
#include <QFileDialog>

#include <DDSConversion/DDSFunc.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textureOrientationLabel->setVisible(false);
    ui->textureOrientationComboBox->setVisible(false);
    ui->processingProgressBar->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

CompressionType MainWindow::GetCurrentCompressionType()
{
    return static_cast<CompressionType>(ui->conversionModeComboBox->currentIndex());
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

        ConvertImagesIntoDDS(imagesPath, GetCurrentCompressionType());
    }
}

void MainWindow::on_convertImageButton_clicked()
{
    QString inputImageName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.jpeg *.bmp *.tif *.tiff)"));

    if(!inputImageName.isEmpty() && !inputImageName.isNull())
    {
        QString outputTextureName = QFileDialog::getSaveFileName(this, tr("Save Texture"), "", tr("Texture File (*.dds)"));

        if(!outputTextureName.isEmpty()&& !outputTextureName.isNull())
        {
            ConvertImagesIntoDDS({{inputImageName.toStdString(), outputTextureName.toStdString()}}, GetCurrentCompressionType());
        }
    }
}

void MainWindow::on_convertFolderButton_clicked()
{
    QDir inputFolder = QFileDialog::getExistingDirectory(this, tr("Open Folder"), "");

    if(!inputFolder.entryList().isEmpty())
    {
        QString outputFolder = QFileDialog::getExistingDirectory(this, tr("Save Folder"), "");

        if(!outputFolder.isEmpty() && !outputFolder.isNull())
        {
            QStringList images = inputFolder.entryList(QStringList() << "*.jpg" << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp" << "*.tif" << "*.tiff", QDir::Files);

            std::vector<std::pair<std::string, std::string>> imagesPath;

            for (const auto& image : images)
            {
                QStringList pieces = image.split(".");
                QString fileName = pieces.value(0);
                QString outputFileName = outputFolder + "/" + fileName + ".dds";

                imagesPath.push_back({inputFolder.absoluteFilePath(image).toStdString(), outputFileName.toStdString()});
            }

            ConvertImagesIntoDDS(imagesPath, GetCurrentCompressionType());
        }
    }
}
