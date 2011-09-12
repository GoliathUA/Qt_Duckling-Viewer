#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebSettings>
#include <QHBoxLayout>
#include "tabstreamwidget.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    TabStreamWidget *tabWidget;
    explicit MainWindow(QWidget *parent = 0);
    void resizeEvent (QResizeEvent * event);
    void setAlwaysOnTop(bool checked);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void alwayOnTopOptionHandle();
    void ssssss();

};

#endif // MAINWINDOW_H
