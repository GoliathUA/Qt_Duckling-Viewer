#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->mainToolBar->hide();
    ui->statusBar->hide();

    QWebSettings *defaultSettings = QWebSettings::globalSettings();
    defaultSettings->setAttribute(QWebSettings::JavascriptEnabled, true);
    defaultSettings->setAttribute(QWebSettings::PluginsEnabled, true);

    this->tabWidget = new TabStreamWidget(this);





    connect(ui->alwayOnTopItem, SIGNAL(triggered()), this, SLOT(alwayOnTopOptionHandle()));


    //setAlwaysOnTop(true);

    //this->startupAction = new QAction(QString::fromUtf8("Запускать при загрузки системы"), this);

        //connect(this->startupAction, SIGNAL(triggered()), this, SLOT(startupOptionHandle()));




    //QWidget *layout = new QWidget(parent);

    //STab *tab = new STab(parent);

    //tab->setSizePolicy(Geometry(QRect(0, 0, 800, 800));


    //ui->tabWidget->addTab(tab, tr("asdasdad"));

    //ui->tabWidget->setStyleSheet("QTabBar::tab { width: 100px; height: 20px; }");


    //ui->tabWidget->height(200);


    //ui->webView->load(QUrl("http://ru.twitch.tv/trundletime"));
    //ui->webView->setContent(QString("<html><body><center>AAAAA</center><iframe height=\"360\" width=\"640\" frameborder=\"0\" src=\"http://www.own3d.tv/liveembed/13576\"></iframe><br/><a href=\"http://www.own3d.tv/live/13576\">CLGaming.net - bigfatlp</a></body></html>").toLatin1());

    //ui->webView->setContent(QString("<html><body><center>AAAAA</center><object width=\"737\" height=\"414\" type=\"application/x-shockwave-flash\" data=\"http://sc2tv.ru/player/player.swf\"><param value=\"true\" name=\"allowFullScreen\"><param value=\"always\" name=\"allowScriptAccess\"><param value=\"transparent\" name=\"wmode\"><param value=\"http://sc2tv.ru/player/player.swf\" name=\"movie\"><param value=\"comment=rambler_online&amp;st=http://sc2tv.ru/player/styles/video28-1446.txt&amp;file=rtmp://fms01.rambler.ru/live/alex007&amp;vnAd=1&amp;vn_profile_id=152942&amp;vn_category_id=7\" name=\"flashvars\"></object></body></html>").toLatin1());




}


void MainWindow::ssssss()
{
    qDebug("sdad!!!!");
}

void MainWindow::alwayOnTopOptionHandle()
{
    setAlwaysOnTop(ui->alwayOnTopItem->isChecked());
}

void MainWindow::resizeEvent(QResizeEvent * event)
{
    this->tabWidget->setGeometry(QRect(ui->centralWidget->x(), ui->centralWidget->y(), ui->centralWidget->width(), ui->centralWidget->height()));
}

void MainWindow::setAlwaysOnTop(bool checked)
{
#ifdef Q_OS_WIN
    // #include <windows.h>
    if (checked) {
        SetWindowPos(this->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    } else {
        SetWindowPos(this->winId(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }
#else
    Qt::WindowFlags flags = this->windowFlags();
    if (checked) {
        this->setWindowFlags(flags | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
        this->show();
    } else {
        this->setWindowFlags(flags ^ (Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint));
        this->show();
    }
#endif
}



MainWindow::~MainWindow()
{
    delete ui;
}
