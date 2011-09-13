#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebSettings>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSystemTrayIcon>

#include "tabstreamwidget.h"
#include "shortviewerdialog.h"
#include "settings.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

void setTranslator(const QString& filename);
QString getTranslatorPath(const QString& lang);

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
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;

public slots:
    void alwayOnTopOptionHandle();
    void handleAboutItem();
    void handleShortView();
    void handleCloseShortView(STab *tab);
};

#endif // MAINWINDOW_H
