#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui -> ditherTypeBox -> addItem("Select Algorithm..", static_cast<int>(DitherType::None));
    ui -> ditherTypeBox -> addItem("Basic", static_cast<int>(DitherType::Basic));
    ui -> ditherTypeBox -> addItem("Floyd-Steinberg", static_cast<int>(DitherType::FloydSteinberg));
    ui -> ditherTypeBox -> addItem("Bayer 2x2", static_cast<int>(DitherType::Bayer2x2));
    ui -> ditherTypeBox -> addItem("Bayer 4x4", static_cast<int>(DitherType::Bayer4x4));
    ui -> ditherTypeBox -> addItem("Bayer 8x8", static_cast<int>(DitherType::Bayer8x8));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete imageLabel;
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", "", "Images (*.png *.jpg *gif);;All Files (*)");
    if(!fileName.isEmpty()){
        ui -> label -> setText("File: " + fileName);
        QImage loadedImage(fileName);
        loadedImage = loadedImage.convertToFormat(QImage::Format_ARGB32);
        originalImage = loadedImage;
        currentImage = loadedImage;
        double newWidth = currentImage.value().width() * scaling;
        double newHeight = currentImage.value().height() * scaling;
        QImage scaledImage = currentImage.value().scaled(newWidth, newHeight, Qt::KeepAspectRatio, Qt::FastTransformation);
        QPixmap pixMap = QPixmap::fromImage(scaledImage);
        if(!imageLabel){
            imageLabel = new QLabel(ui -> scrollArea);
            ui -> scrollArea -> setWidget(imageLabel);
        }
        imageLabel -> setPixmap(pixMap);
        imageLabel -> setFixedSize(pixMap.width(), pixMap.height());
    }
}


void MainWindow::on_minusButton_clicked()
{
    if(scaling > 0.1f && currentImage.has_value()){
        scaling -= 0.1f;
        ui -> scalingLabel -> setText("Scaling: " + QString::number(scaling, 'f', 2) + "x");
        double newWidth = currentImage.value().width() * scaling;
        double newHeight = currentImage.value().height() * scaling;
        QImage scaledImage = currentImage.value().scaled(newWidth, newHeight, Qt::KeepAspectRatio, Qt::FastTransformation);
        QPixmap pixMap = QPixmap::fromImage(scaledImage);
        imageLabel -> setPixmap(pixMap);
        imageLabel -> setFixedSize(newWidth, newHeight);
    }
}


void MainWindow::on_plusButton_clicked()
{
    if(imageLabel && currentImage.has_value()){
        scaling += 0.1f;
        ui -> scalingLabel -> setText("Scaling: " + QString::number(scaling, 'f', 2) + "x");
        double newWidth = currentImage.value().width() * scaling;
        double newHeight = currentImage.value().height() * scaling;
        QImage scaledImage = currentImage.value().scaled(newWidth, newHeight, Qt::KeepAspectRatio, Qt::FastTransformation);
        QPixmap pixMap = QPixmap::fromImage(scaledImage);
        imageLabel -> setPixmap(pixMap);
        imageLabel -> setFixedSize(newWidth, newHeight);
    }
}


void MainWindow::on_ditherButton_clicked()
{
    if(originalImage.has_value()){
        QImage ditheredImage = Dither::ditherImage(originalImage.value(), ditherType);
        currentImage = ditheredImage;
        QPixmap pixMap = QPixmap::fromImage(ditheredImage);
        imageLabel -> setPixmap(pixMap);
    }
}


void MainWindow::on_ditherTypeBox_currentIndexChanged(int index)
{
    ditherType = static_cast<DitherType>(ui -> ditherTypeBox -> itemData(index).toInt());
}
