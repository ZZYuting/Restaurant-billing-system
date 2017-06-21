#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <qgraphicsview.h>
#include <qgraphicsscene.h>
#include <QDialog>
#include <QLabel>
#include <QPixmap>
#include "fpga.h"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include "libcam.h"
using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Mat mat;
    VideoCapture *vc;
    Camera *mcamera;
    IplImage *frame;
public:
    QImage mat2QImage(const cv::Mat &mat);
   void detectAndDisplay( Mat frame);
   void colorReduce(Mat& inputImage, Mat& outputImage);
   void whiteimprove(Mat& g_srcImage, Mat& dstImage);
private:
    Ui::MainWindow *ui;
    QGraphicsView *gv1;
    FPGA *fpga;
    uint32_t m_FPGA_KeyStatus;
    uint32_t Status;

public slots:
    void send_a_char(void);
    void read_native_pic(void);
    void send_char(char ch);
private slots:
    //void on_pushButton_clicked();
};
class QLineEdit;
class QSlider;
class MyDialog1 : public QDialog
{
  Q_OBJECT
public:
    explicit MyDialog1(QWidget *parent = 0);
signals:
public slots:
  void setLineEditValue(int value);
private:
  QLineEdit *lineEdit;
  QSlider *slider;
};
class MyDialog2 : public QDialog
{
  Q_OBJECT
public:
    explicit MyDialog2(QWidget *parent = 0);
signals:
public slots:
  void setLineEditValue(int value);
private:
  QLineEdit *lineEdit;
  QSlider *slider;
};
class MyDialog3 : public QDialog
{
  Q_OBJECT
public:
    explicit MyDialog3(QWidget *parent = 0);
signals:
public slots:
  void setLineEditValue(int value);
private:
  QLineEdit *lineEdit;
  QSlider *slider;
};
class Dialogzzu : public QDialog
{
    Q_OBJECT

public:
    Dialogzzu(QWidget *parent = 0);
    ~Dialogzzu();
};
class Dialogprice : public QDialog
{
    Q_OBJECT

public:
    Dialogprice(QWidget *parent = 0);
    ~Dialogprice();
};
#endif // MAINWINDOW_H
