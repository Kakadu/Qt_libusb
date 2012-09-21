#ifndef QT_LIBUSB_H
#define QT_LIBUSB_H

#include <QMainWindow>

namespace Ui {
class Qt_libusb;
}

class Qt_libusb : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Qt_libusb(QWidget *parent = 0);
    ~Qt_libusb();
    
private:
    Ui::Qt_libusb *ui;

public slots:
    void listview_populate_usb_devices();

};

#endif // QT_LIBUSB_H
