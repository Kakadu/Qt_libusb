#include <QtWidgets/QApplication>
#include "qt_libusb.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Qt_libusb w;
    w.setFixedSize(w.size());
    w.show();
    return a.exec();
}
