#include "dither.h"
#include <QPixmap>
#include <cmath>
#include <vector>

QImage Dither::ditherImage(const QImage &originalImage, DitherType type){
    switch(type){
    case DitherType::Basic:
        return basicErrorDither(originalImage);
    case DitherType::FloydSteinberg:
        return floydSteinbergDither(originalImage);
    case DitherType::Bayer2x2:
        return bayer2x2Dither(originalImage);
    case DitherType::Bayer4x4:
        return bayer4x4Dither(originalImage);
    case DitherType::Bayer8x8:
        return bayer8x8Dither(originalImage);
    default:
        return originalImage;
    }
}

int Dither::findClosest(double number, int range[], int rangeSize){
    double minDifference = abs(range[0] - number);
    int minInRange = range[0];
    for(int i = 0; i < rangeSize; i++){
        double diffBetweeValAndNumber = abs(range[i] - number);
        if(abs(range[i] - number) < minDifference){
            minDifference = diffBetweeValAndNumber;
            minInRange = range[i];
        }
    }
    return minInRange;
}

QImage Dither::basicErrorDither(const QImage &originalImage) {
    int range[2] = {0, 255};
    int error = 0;

    QImage convertedImage = originalImage.convertToFormat(QImage::Format_ARGB32);
    for (int y = 0; y < convertedImage.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(convertedImage.scanLine(y));
        for (int x = 0; x < convertedImage.width(); ++x) {
            QRgb &rgb = line[x];
            double average = ceil((qRed(rgb) + qBlue(rgb) + qGreen(rgb))/3);
            average = average + error;
            int closestToAverageInRange = findClosest(average, range, sizeof(range) / sizeof(range[0]));
            error = average - closestToAverageInRange;
            rgb = qRgba(closestToAverageInRange, closestToAverageInRange, closestToAverageInRange, qAlpha(rgb));
        }
    }
    return convertedImage;
}

QImage Dither::floydSteinbergDither(const QImage &originalImage){
    int range[2] = {0, 255};
    std::vector<std::vector<double>> errorVector(originalImage.height(), std::vector<double>(originalImage.width(), 0));
    QImage convertedImage = originalImage.convertToFormat(QImage::Format_ARGB32);
    for (int y = 0; y < convertedImage.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(convertedImage.scanLine(y));
        for (int x = 0; x < convertedImage.width(); ++x) {
            QRgb &rgb = line[x];
            double average = ceil((qRed(rgb) + qBlue(rgb) + qGreen(rgb))/3);
            average = average + errorVector[y][x];
            int closestToAverageInRange = findClosest(average, range, sizeof(range) / sizeof(range[0]));
            double error = average - closestToAverageInRange;
            error = error / 16;
            if(x + 1 < convertedImage.width()){
                errorVector[y][x+1] += error*7; //right
            }
            if(y + 1 < convertedImage.height()){
                errorVector[y+1][x] += error*5; //bottom
                if(x - 1 >= 0){
                    errorVector[y+1][x-1] += error*3; //bottom left
                }
                if(x + 1 < convertedImage.width()){
                    errorVector[y+1][x+1] += error; //bottom right
                }
            }
            rgb = qRgba(closestToAverageInRange, closestToAverageInRange, closestToAverageInRange, qAlpha(rgb));
        }
    }
    return convertedImage;
}

QImage Dither::bayer2x2Dither(const QImage &originalImage){
    double bayerMatrix[2][2] = {
        {-0.375,  0.125},
        { 0.375, -0.125}
    };
    int gridSize = 2;

    int range[2] = {0, 255};

    QImage convertedImage = originalImage.convertToFormat(QImage::Format_ARGB32);
    for (int y = 0; y < convertedImage.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(convertedImage.scanLine(y));
        for (int x = 0; x < convertedImage.width(); ++x) {
            QRgb &rgb = line[x];
            double average = ceil((qRed(rgb) + qBlue(rgb) + qGreen(rgb))/3);
            double spread = 255/(sizeof(range) / sizeof(range[0]));
            average = average + spread * bayerMatrix[y%gridSize][x%gridSize] - 1/2;
            int closestToAverageInRange = findClosest(average, range, sizeof(range) / sizeof(range[0]));
            rgb = qRgba(closestToAverageInRange, closestToAverageInRange, closestToAverageInRange, qAlpha(rgb));
        }
    }
    return convertedImage;
}


QImage Dither::bayer4x4Dither(const QImage &originalImage){
    double bayerMatrix[4][4] = {
        {-0.46875,  0.03125, -0.34375,  0.15625},
        { 0.28125, -0.21875,  0.40625, -0.09375},
        {-0.28125,  0.21875, -0.40625,  0.09375},
        { 0.46875, -0.03125,  0.34375, -0.15625}
    };
    int gridSize = 4;

    int range[2] = {0, 255};

    QImage convertedImage = originalImage.convertToFormat(QImage::Format_ARGB32);
    for (int y = 0; y < convertedImage.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(convertedImage.scanLine(y));
        for (int x = 0; x < convertedImage.width(); ++x) {
            QRgb &rgb = line[x];
            double average = ceil((qRed(rgb) + qBlue(rgb) + qGreen(rgb))/3);
            double spread = 255/(sizeof(range) / sizeof(range[0]));
            average = average + spread * bayerMatrix[y%gridSize][x%gridSize] - 1/2;
            int closestToAverageInRange = findClosest(average, range, sizeof(range) / sizeof(range[0]));
            rgb = qRgba(closestToAverageInRange, closestToAverageInRange, closestToAverageInRange, qAlpha(rgb));
        }
    }
    return convertedImage;
}


QImage Dither::bayer8x8Dither(const QImage &originalImage){
    double bayerMatrix[8][8] = {
        {-0.515625, -0.015625, -0.390625,  0.109375, -0.484375,  0.015625, -0.359375,  0.140625},
        { 0.234375, -0.265625,  0.359375, -0.140625,  0.265625, -0.234375,  0.390625, -0.109375},
        {-0.328125,  0.171875, -0.453125,  0.046875, -0.296875,  0.203125, -0.421875,  0.078125},
        { 0.421875, -0.078125,  0.296875, -0.203125,  0.453125, -0.046875,  0.328125, -0.171875},
        {-0.46875,   0.03125,  -0.34375,   0.15625,  -0.5,       0.0,      -0.375,     0.125},
        { 0.28125,  -0.21875,   0.40625,  -0.09375,   0.25,     -0.25,      0.375,    -0.125},
        {-0.28125,   0.21875,  -0.40625,   0.09375,  -0.3125,    0.1875,   -0.4375,    0.0625},
        { 0.46875,  -0.03125,   0.515625, -0.15625,   0.4375,   -0.0625,    0.3125,   -0.1875}
    };
    int gridSize = 8;

    int range[2] = {0, 255};

    QImage convertedImage = originalImage.convertToFormat(QImage::Format_ARGB32);
    for (int y = 0; y < convertedImage.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(convertedImage.scanLine(y));
        for (int x = 0; x < convertedImage.width(); ++x) {
            QRgb &rgb = line[x];
            double average = ceil((qRed(rgb) + qBlue(rgb) + qGreen(rgb))/3);
            double spread = 255/(sizeof(range) / sizeof(range[0]));
            average = average + spread * bayerMatrix[y%gridSize][x%gridSize] - 1/2;
            int closestToAverageInRange = findClosest(average, range, sizeof(range) / sizeof(range[0]));
            rgb = qRgba(closestToAverageInRange, closestToAverageInRange, closestToAverageInRange, qAlpha(rgb));
        }
    }
    return convertedImage;
}

