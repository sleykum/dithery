#ifndef DITHER_H
#define DITHER_H

#include <QImage>

enum class DitherType {
    None,
    Basic,
    FloydSteinberg,
    Bayer2x2,
    Bayer4x4,
    Bayer8x8
};

class Dither {
public:
    static QImage ditherImage(const QImage &originalImage, DitherType type);
private:
    static QImage basicErrorDither(const QImage &originalImage);
    static QImage floydSteinbergDither(const QImage &originalImage);
    static QImage bayer2x2Dither(const QImage &originalImage);
    static QImage bayer4x4Dither(const QImage &originalImage);
    static QImage bayer8x8Dither(const QImage &originalImage);
    static int findClosest(double number, int range[], int rangeSize);
};


#endif // DITHER_H
