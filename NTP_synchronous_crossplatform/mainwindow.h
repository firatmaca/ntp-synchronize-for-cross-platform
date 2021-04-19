#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>

#include <QCoreApplication>
#include "iostream"
#if defined(Q_OS_WIN32)
#include "process.h"
#include "windows.h"
#endif
#include "qfile.h"
#include "qstring.h"
#include  "QTimer"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTimer timer;
    QString ntp;
private slots:
    void on_pushButton_clicked();
    bool ipEslestiMi(const QString &ip);


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
