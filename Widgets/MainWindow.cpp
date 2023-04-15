#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <DDSConversion/DDSFunc.h>
#include <Widgets/DDSPreviewerWidget.h>

#include <QDir>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->processingProgressBar->setVisible(false);
    ui->runBenchmarkButton->setVisible(false);
    ui->processedTexturesTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
    delete ui;
}

CompressionType MainWindow::GetCurrentCompressionType()
{
    return static_cast<CompressionType>(ui->conversionModeComboBox->currentIndex());
}

TextureOrientation MainWindow::GetCurrentTextureOrientation()
{
    return static_cast<TextureOrientation>(ui->textureOrientationComboBox->currentIndex());
}

void MainWindow::AddProcessedTextures(std::vector<std::pair<std::string, std::string>> imageFiles)
{
    for (const auto& entry : imageFiles)
    {
        QTableWidgetItem* pItemAssetPath = new QTableWidgetItem(entry.second.c_str());
        QTableWidgetItem* pItemCompressionType = new QTableWidgetItem(ui->conversionModeComboBox->currentText());

        ui->processedTexturesTable->insertRow(this->ui->processedTexturesTable->rowCount());

        ui->processedTexturesTable->setItem(this->ui->processedTexturesTable->rowCount() - 1, 0, pItemAssetPath);
        ui->processedTexturesTable->setItem(this->ui->processedTexturesTable->rowCount() - 1, 1, pItemCompressionType);
    }
}

void MainWindow::DisplayPreviewerWidget(QString filePath)
{
    DDSPreviewerWidget* widget = new DDSPreviewerWidget();
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->SetTexture(filePath, GetCurrentTextureOrientation());
    widget->setWindowTitle("DDS Previewer: " + QString(filePath));
    widget->show();
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

        ConvertImagesIntoDDS(imagesPath, GetCurrentCompressionType(), GetCurrentTextureOrientation());

        AddProcessedTextures(imagesPath);
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
            ConvertImagesIntoDDS({{inputImageName.toStdString(), outputTextureName.toStdString()}}, GetCurrentCompressionType(), GetCurrentTextureOrientation());

            AddProcessedTextures({{inputImageName.toStdString(), outputTextureName.toStdString()}});
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

            ConvertImagesIntoDDS(imagesPath, GetCurrentCompressionType(), GetCurrentTextureOrientation());

            AddProcessedTextures(imagesPath);
        }
    }
}

void MainWindow::on_processedTexturesTable_cellDoubleClicked(int row, int column)
{
    if (column == 0) // only handle if double clicked on asset path column
    {
        QTableWidgetItem* pItem = ui->processedTexturesTable->item(row, column);
        DisplayPreviewerWidget(pItem->text());
    }
}

void MainWindow::on_openTextureButton_clicked()
{
    QString inputImageName = QFileDialog::getOpenFileName(this, tr("Open Texture"), "", tr("Texture (*.dds)"));

    if(!inputImageName.isEmpty() && !inputImageName.isNull())
    {
        DisplayPreviewerWidget(inputImageName);
    }
}
