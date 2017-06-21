#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <windows.h>
#include <QTimer>
#include <QDebug>
#include <QLineEdit>
#include <QSlider>
#include <QHBoxLayout>

#define eachb 17650
#define eachg 15000//17900
#define eachr 16600
int colormin=50,colormax=112,black_yu=60,key=0;
long int blue_count=0,black_count=0,red_count=0,total_count=0,old_blue_count=0,old_black_count=0,old_red_count=0,old_total_count=0;
int price=0,old_price=0,ci=0,shi=0,ge=0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{   fpga = new FPGA;
    mcamera = new Camera("/dev/video1",320,240,30);
    frame = cvCreateImage(cvSize(320, 240), 8, 3);
    ui->setupUi(this);
    gv1 = ui->gv1;
    QTimer *timer  = new QTimer();
    timer->start(200);
    //connect(timer,SIGNAL(timeout()),this,SLOT(send_a_char()));
    fpga->HexSet(0,16);fpga->HexSet(1,16);fpga->HexSet(2,16);fpga->HexSet(3,16);fpga->HexSet(4,16);fpga->HexSet(5,16);
    connect(timer,SIGNAL(timeout()),this,SLOT(read_native_pic()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete frame;
}



void MainWindow::send_char(char ch){
    qDebug()<<"begin to precess:"+ch;
    qDebug()<<"precessed precess:"+ch;
}

void MainWindow::read_native_pic(){
    Mat yt;
    mcamera->Update();
    mcamera->toIplImage(frame);
    yt = frame;

//    do

//             yt = imread("/home/root/a.jpg");
//    while(yt.empty());
    //whiteimprove(yt,yt);

//    if (fpga->KeyRead(&Status))
//        m_FPGA_KeyStatus = Status;
//    if(!(m_FPGA_KeyStatus&0x01))//anxian shi 0,songkai 1
//    {
        colorReduce(yt,yt);
        {
          total_count=blue_count+black_count+red_count;
          if(total_count>=(old_total_count+old_total_count/10)||total_count<=(old_total_count-old_total_count/10))
          {
              old_total_count=total_count;
              ci=0;
          }
          else
              ci++;
          price=((blue_count+eachb/2)/eachb)*1+((black_count+eachg/2)/eachg)*2+((red_count+eachr/2)/eachr)*3;
          printf("price=%d   total_count=%d",price,total_count);
          if(ci<=10)
              price=0;
          shi=price/10;ge=price%10;
          fpga->HexSet(0,ge);fpga->HexSet(1,shi);
        }
//    }

    QImage image = mat2QImage(yt);
    QGraphicsScene *scene=new QGraphicsScene();
    scene->addPixmap(QPixmap::fromImage(image));
    gv1->setScene(scene);
    gv1->show();
}

void MainWindow::send_a_char(){
    QString strs = "ABCDEFGHIJKLMNOPQRSTUVWSXYZ";
    QByteArray qba = strs.toLatin1();
    char* ssss = qba.data();
    static int char_num = 0;
    send_char(ssss[char_num]);
    char_num++;
    char_num=char_num%51;

}

QImage MainWindow::mat2QImage(const cv::Mat &mat){
    // 8-bits unsigned, NO. OF CHANNELS = 1
       if(mat.type() == CV_8UC1)
       {
            QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
           // Set the color table (used to translate colour indexes to qRgb values)
           image.setColorCount(256);
           for(int i = 0; i < 256; i++)
           {
               image.setColor(i, qRgb(i, i, i));
           }
           // Copy input Mat
           uchar *pSrc = mat.data;
           for(int row = 0; row < mat.rows; row ++)
           {
               uchar *pDest = image.scanLine(row);
               memcpy(pDest, pSrc, mat.cols);
               pSrc += mat.step;
           }
           return image;
       }
       // 8-bits unsigned, NO. OF CHANNELS = 3
       else if(mat.type() == CV_8UC3)
       {
           // Copy input Mat
           const uchar *pSrc = (const uchar*)mat.data;
           // Create QImage with same dimensions as input Mat
           QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
           return image.rgbSwapped();
       }
       else if(mat.type() == CV_8UC4)
       {
           qDebug() << "CV_8UC4";
           // Copy input Mat
           const uchar *pSrc = (const uchar*)mat.data;
           // Create QImage with same dimensions as input Mat
           QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
           return image.copy();
       }
       else
       {
           qDebug() << "ERROR: Mat could not be converted to QImage.";
           return QImage();
       }


}
void MainWindow::colorReduce(Mat& inputImage, Mat& outputImage)
{
    //参数准备
    outputImage = inputImage.clone();  //拷贝实参到临时变量
    int rowNumber = outputImage.rows;  //行数
    int colNumber = outputImage.cols;  //列数
    total_count=rowNumber*colNumber;
    blue_count=0;black_count=0;red_count=0;
    //存取彩色图像像素
    for(int i = 0;i < rowNumber;i++)
    {
        for(int j = 0;j < colNumber;j++)
        {
            //// ------------------------【开始处理每个像素】--------------------
            //outputImage.at<Vec3b>(i,j)[0] =  outputImage.at<Vec3b>(i,j)[0]/div*div + div/2;  //蓝色通道
            //outputImage.at<Vec3b>(i,j)[1] =  outputImage.at<Vec3b>(i,j)[1]/div*div + div/2;  //绿色通道
            //outputImage.at<Vec3b>(i,j)[2] =  outputImage.at<Vec3b>(i,j)[2]/div*div + div/2;  //红是通道

            if(outputImage.at<Vec3b>(i,j)[0]>=colormin&&outputImage.at<Vec3b>(i,j)[1]<=colormax&&outputImage.at<Vec3b>(i,j)[2]<=colormax)//
            {
                blue_count++;
                outputImage.at<Vec3b>(i,j)[0]=255;outputImage.at<Vec3b>(i,j)[1]=0;outputImage.at<Vec3b>(i,j)[2]=0;
            }
            if(outputImage.at<Vec3b>(i,j)[1]<=black_yu&&outputImage.at<Vec3b>(i,j)[0]<=black_yu&&outputImage.at<Vec3b>(i,j)[2]<=black_yu)//
            {
                black_count++;
                outputImage.at<Vec3b>(i,j)[1]=255;outputImage.at<Vec3b>(i,j)[0]=0;outputImage.at<Vec3b>(i,j)[2]=0;
            }
            if(outputImage.at<Vec3b>(i,j)[2]>=colormin&&outputImage.at<Vec3b>(i,j)[1]<=colormax&&outputImage.at<Vec3b>(i,j)[0]<=colormax)//
            {
                red_count++;
                outputImage.at<Vec3b>(i,j)[2]=255;outputImage.at<Vec3b>(i,j)[1]=0;outputImage.at<Vec3b>(i,j)[0]=0;
            }
            // -------------------------【处理结束】----------------------------
        }  // 行处理结束
    }
    //blue_count=blue_count*100/(i*j);black_count=black_count*100/(i*j);red_count=red_count*100/(i*j);
     printf("blue=%d,black=%d,red=%d\n",blue_count,black_count,red_count);
}
void MainWindow::whiteimprove(Mat& g_srcImage, Mat& dstImage)
{
    //参数准备
    vector<Mat> g_vChannels;

    //分离通道
    split(g_srcImage, g_vChannels);
    Mat imageBlueChannel = g_vChannels.at(0);
    Mat imageGreenChannel = g_vChannels.at(1);
    Mat imageRedChannel = g_vChannels.at(2);
    double imageBlueChannelAvg = 0;
    double imageGreenChannelAvg = 0;
    double imageRedChannelAvg = 0;

    //求各通道的平均值
    imageBlueChannelAvg = mean(imageBlueChannel)[0];
    imageGreenChannelAvg = mean(imageGreenChannel)[0];
    imageRedChannelAvg = mean(imageRedChannel)[0];

    //求出个通道所占增益
    double K = (imageRedChannelAvg + imageGreenChannelAvg + imageRedChannelAvg) / 3;
    double Kb = K / imageBlueChannelAvg;
    double Kg = K / imageGreenChannelAvg;
    double Kr = K / imageRedChannelAvg;

    //更新白平衡后的各通道BGR值
    addWeighted(imageBlueChannel, Kb, 0, 0, 0, imageBlueChannel);
    addWeighted(imageGreenChannel, Kg, 0, 0, 0, imageGreenChannel);
    addWeighted(imageRedChannel, Kr, 0, 0, 0, imageRedChannel);

    merge(g_vChannels, dstImage);//图像各通道合并
}
MyDialog1::MyDialog1(QWidget *parent) :
  QDialog(parent)
{
    QString str=QString::number(colormin,10);
   lineEdit = new QLineEdit(str);
  //新建一个水平方向的滑动条QSlider控件
  slider = new QSlider(Qt::Horizontal);
  //设置滑动条控件的最小值
  slider->setMinimum(0);
  //设置滑动条控件的最大值
  slider->setMaximum(255);
  //设置滑动条控件的值
  slider->setValue(colormin);

  //信号和槽 当滑动条的值发生改变时，即产生一个valueChanged(int)信号 设置QLineEdit控件的显示文本
  connect(slider, SIGNAL(valueChanged(int)), this, SLOT(setLineEditValue(int)));
  QHBoxLayout *layout = new QHBoxLayout();
  layout->addWidget(lineEdit);
  layout->addWidget(slider);

  this->setLayout(layout);
  this->resize(200, 50);
  this->setWindowTitle("min");
}
MyDialog2::MyDialog2(QWidget *parent) :
  QDialog(parent)
{

    QString str=QString::number(colormax,10);
   lineEdit = new QLineEdit(str);
  //新建一个水平方向的滑动条QSlider控件
  slider = new QSlider(Qt::Horizontal);
  //设置滑动条控件的最小值
  slider->setMinimum(0);
  //设置滑动条控件的最大值
  slider->setMaximum(255);
  //设置滑动条控件的值
  slider->setValue(colormax);

  //信号和槽 当滑动条的值发生改变时，即产生一个valueChanged(int)信号 设置QLineEdit控件的显示文本
  connect(slider, SIGNAL(valueChanged(int)), this, SLOT(setLineEditValue(int)));
  QHBoxLayout *layout = new QHBoxLayout();
  layout->addWidget(lineEdit);
  layout->addWidget(slider);

  this->setLayout(layout);
  this->resize(200, 50);
  this->setWindowTitle("max");
}
MyDialog3::MyDialog3(QWidget *parent) :
  QDialog(parent)
{

    QString str=QString::number(black_yu,10);
   lineEdit = new QLineEdit(str);
  //新建一个水平方向的滑动条QSlider控件
  slider = new QSlider(Qt::Horizontal);
  //设置滑动条控件的最小值
  slider->setMinimum(0);
  //设置滑动条控件的最大值
  slider->setMaximum(255);
  //设置滑动条控件的值
  slider->setValue(black_yu);

  //信号和槽 当滑动条的值发生改变时，即产生一个valueChanged(int)信号 设置QLineEdit控件的显示文本
  connect(slider, SIGNAL(valueChanged(int)), this, SLOT(setLineEditValue(int)));
  QHBoxLayout *layout = new QHBoxLayout();
  layout->addWidget(lineEdit);
  layout->addWidget(slider);

  this->setLayout(layout);
  this->resize(200, 50);
  this->setWindowTitle("black");
}

Dialogzzu::Dialogzzu(QWidget *parent)
    : QDialog(parent)
{
    QLabel *label;
    QVBoxLayout *layout;
    layout =new QVBoxLayout();
    label =new QLabel();
    QImage *image=new QImage("/home/root/zzu.jpg");
    label->setPixmap(QPixmap::fromImage(*image));
    label->setAlignment(Qt::AlignLeft);
    layout->addWidget(label);
    setLayout(layout);
}
Dialogzzu::~Dialogzzu()
{

}
Dialogprice::Dialogprice(QWidget *parent)
    : QDialog(parent)
{
    QLabel *label;
    QVBoxLayout *layout;
    layout =new QVBoxLayout();
    label =new QLabel();
    QImage *image=new QImage("/home/root/price.jpg");
    label->setPixmap(QPixmap::fromImage(*image));
    label->setAlignment(Qt::AlignRight);
    layout->addWidget(label);
    setLayout(layout);
}
Dialogprice::~Dialogprice()
{

}
void MyDialog1::setLineEditValue(int value)
{
  int pos = slider->value();
  colormin=pos;
  QString str = QString("%1").arg(pos);
  lineEdit->setText(str);
}
void MyDialog2::setLineEditValue(int value)
{
  int pos1 = slider->value();
  colormax=pos1;
  QString str = QString("%1").arg(pos1);
  lineEdit->setText(str);

}
void MyDialog3::setLineEditValue(int value)
{
  int pos2 = slider->value();
  black_yu=pos2;
  QString str = QString("%1").arg(pos2);
  lineEdit->setText(str);
}
