#include "qt_libusb.h"
#include "ui_qt_libusb.h"
#include <QStatusBar>
#include <QDesktopWidget>
#include <libusb.h>

using namespace std;

Qt_libusb::Qt_libusb(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Qt_libusb)
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);

    //centering the window
    int screenWidth, width, screenHeight, height, x, y;
    QSize windowSize;
    QDesktopWidget *desktop = QApplication::desktop();

    screenWidth = desktop->width();
    screenHeight = desktop->height();

    windowSize = size();
    width = windowSize.width();
    height = windowSize.height();

    x = (screenWidth - width) / 2;
    y = (screenHeight - height) / 2;
    y -= 50;

    move ( x, y );
}

Qt_libusb::~Qt_libusb()
{
    delete ui;
}

void Qt_libusb::listview_populate_usb_devices()
{
    ui->listWidget->clear(); //clearing the list view

    libusb_device_handle *device_handle; //handle to USB device
    libusb_device_descriptor device_descriptor; //the device descriptor
    libusb_device  **device_list; //to store all found USB devices
    libusb_context *ctx; //a libusb context for library intialization
    ssize_t i; //for the device iterator for loop
    ssize_t device_count; //holding number of devices in list
    QString list_entry; //for using library outputs in char * in Qt framework

    int r; //for return values
    int progress_bar_value = 0;
    int progress_bar_correction = 0;
    char device_open = 0; //for checking whether a device was opened or not
    unsigned char string_buffer_manufacturer[4096]; //for storing manufacturer descriptor
    unsigned char string_buffer_product[4096]; //for storing product descriptor
    const char *cc_manufacturer; //for the casting to use the string output from libusb in Qt
    const char *cc_product; //for the casting to use the string output from libusb in Qt

    r = libusb_init(&ctx); //initializing the library

    //inform error on status bar
    if(r < 0) ui->statusBar->showMessage("Error: Initializing libusb");

    else
    {
        //detecting all the connected devices
        device_count = libusb_get_device_list(ctx, &device_list);

        //inform error on status bar
        if(device_count <= 0)
            ui->statusBar->showMessage("Info: No device found");

        else
        {
            //resetting the progress bar
            ui->progressBar->reset();

            //for loop iterating through found devices
            for(i=0;i<device_count;i++)
            {
                //getting device descriptor
                r = libusb_get_device_descriptor(device_list[i], &device_descriptor);

                //inform error on status bar
                if(r < 0)   ui->statusBar->showMessage("Error: Failed to get device descriptor");

                else
                {
                    //opening the device
                    r = libusb_open(device_list[i],&device_handle);

                    if(r < 0)
                    {
                        //inform error on status bar
                        ui->statusBar->showMessage("Error: Opening USB device");
                        //indicating that device is not open
                        device_open = 0;
                    }

                    else
                    {
                        //indicating that device is open
                        device_open = 1;

                        //getting the ASCII text value from the descriptor field
                        r = libusb_get_string_descriptor_ascii(device_handle,device_descriptor.iManufacturer,string_buffer_manufacturer,sizeof(string_buffer_manufacturer));

                        if(r < 0)
                        {
                            //reporting error on the list entry
                            ui->listWidget->addItem("Error: Converting descriptor to ASCII [iManufacturer]");
                            ui->listWidget->item(ui->listWidget->count() - 1)->setForeground(Qt::white);
                            ui->listWidget->item(ui->listWidget->count() - 1)->setBackground(Qt::red);
                        }

                        else
                        {
                            //getting the ASCII text value from the descriptor field
                            r = libusb_get_string_descriptor_ascii(device_handle,device_descriptor.iProduct,string_buffer_product,sizeof(string_buffer_product));

                            if(r < 0)
                            {
                                //reporting error on the list entry
                                ui->listWidget->addItem("Error: Converting descriptor to ASCII [iProduct]");
                                ui->listWidget->item(ui->listWidget->count() - 1)->setForeground(Qt::white);
                                ui->listWidget->item(ui->listWidget->count() - 1)->setBackground(Qt::red);
                            }

                            else
                            {
                                //converting the string to const char*
                                cc_manufacturer = (const char*)string_buffer_manufacturer;
                                QString manufacturer(cc_manufacturer);

                                //converting the string to const char*
                                cc_product = (const char*)string_buffer_product;
                                QString product(cc_product);

                                //constructing the string for list entry
                                list_entry = manufacturer + " " + product;

                                ui->listWidget->horizontalScrollBar();
                                ui->listWidget->verticalScrollBar();
                                ui->listWidget->addItem(list_entry); //adding the list entry

                                //closing opened USB device
                                if (device_open == 1)
                                {
                                    //closing the deivce
                                    libusb_close(device_handle);
                                    //indicating that device is open
                                    device_open = 0;
                                }

                                //calculate and set the progress bar value
                                progress_bar_value = (i+1) * (100/device_count);

                                //correcting value
                                if ( (i+1) == device_count)
                                {
                                    progress_bar_correction = 100 - progress_bar_value;
                                    progress_bar_value = progress_bar_value + progress_bar_correction;
                                }

                                //setting the value
                                ui->progressBar->setValue(progress_bar_value);
                            }
                        }
                    }
                }
            }
            //closing opened USB device
            if (device_open == 1)
            {
                //closing the deivce
                libusb_close(device_handle);
                //indicating that device is open
                device_open = 0;
            }
        }
    }
}
