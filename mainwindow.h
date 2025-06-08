#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QLabel>
#include <optional>
#include "dither.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_minusButton_clicked();

    void on_plusButton_clicked();

    void on_ditherButton_clicked();

    void on_ditherTypeBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    std::optional<QImage> originalImage;
    std::optional<QImage> currentImage;
    DitherType ditherType = DitherType::None;
    QLabel *imageLabel = nullptr;
    double scaling = 1;
};
#endif // MAINWINDOW_H
