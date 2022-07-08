#include "ckernel.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);//创建qt的应用

 //   w.show();
 //   return a.exec();
    Ckernel::Getinstance();
    return a.exec();
}
