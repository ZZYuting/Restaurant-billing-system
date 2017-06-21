#include "mainwindow.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    MyDialog1 dialog1;
    MyDialog2  dialog2;
    MyDialog3  dialog3;
    Dialogzzu  zzu;
    Dialogprice  price;
    w.show();
    dialog1.show();
    dialog2.show();
    dialog3.show();
    zzu.show();
    price.show();
   return a.exec();
}

