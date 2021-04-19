#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "socket1.h"
#include "time.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iostream"
#include "QString"
#include "QDebug"
#include "QMessageBox"
#pragma comment(lib,"ws2_32.lib");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->lineEdit->clear();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString ntp_address = this->ui->lineEdit->text();
    if(!ipEslestiMi(ntp_address)){
        this->ui->lineEdit->setText("Gecersiz Ip Adresi Format Uygun Degil");
    }
    else{
#ifdef Q_OS_LINUX
        QString ntp_command = QString("pkexec env DISPLAY=$DISPLAY XAUTHORITY=$XAUTHORITY ntpdate -s ") + ntp_address;
        FILE *ntp_sync_command = ::popen(ntp_command.toStdString().c_str(), "r");
        ::pclose(ntp_sync_command);
#else

        socket1 x;
        tm  timeptr;
        bool a =x.connettoserver(ntp_address , timeptr);
        if(a){
            ntp = ntp_address;
            SYSTEMTIME st;
            st.wDay = timeptr.tm_mday;
            st.wMonth =1 + timeptr.tm_mon;
            st.wYear = 1900+ timeptr.tm_year;
            st.wSecond = timeptr.tm_sec;
            st.wMinute = timeptr.tm_min;
            st.wHour = timeptr.tm_hour;
            bool res = SetLocalTime(&st);

            QMessageBox msgbox;
            msgbox.setText("Ntp Ayarlandı.");
            msgbox.exec();
            //            this->close();
        }
        else {
            this->ui->lineEdit->setText("Ntp Bu Ip'ye Bağlanamıyor");
        }
    }
#endif
}

bool MainWindow::ipEslestiMi(const QString &ip)
{
    QString Octet = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-4])";
    QRegExp ipRegex("^" + Octet + "\\." + Octet + "\\." + Octet + "\\." + Octet + "$");
    return ipRegex.exactMatch(ip);
}
