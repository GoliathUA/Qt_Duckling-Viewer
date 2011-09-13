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
    connect(ui->aboutItem, SIGNAL(triggered()), this, SLOT(handleAboutItem()));
    connect(ui->newTabItem, SIGNAL(triggered()), this->tabWidget, SLOT(createTab()));
    connect(ui->shortViewItem, SIGNAL(triggered()), this, SLOT(handleShortView()));



    //
    QIcon icon = QIcon(":/images/title.png");
    this->trayIcon = new QSystemTrayIcon(this);
    this->trayIcon->setIcon(icon);

    this->trayMenu = new QMenu(this);

    QAction *logoutAction = new QAction(tr("Logout"), this);
    //connect(logoutAction, SIGNAL(triggered()), this, SLOT(logout()));
    this->trayMenu->addAction(logoutAction);

    this->trayIcon->setContextMenu(trayMenu);






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


void MainWindow::handleAboutItem()
{
    QMessageBox::about(this, trUtf8("Duckling Viewer", "Application title"), trUtf8("About Application"));
}

void MainWindow::handleShortView()
{
    int currentTabIndex = this->tabWidget->currentIndex();
    if (currentTabIndex == -1) {
        QMessageBox::information(this, trUtf8("asdasd"), trUtf8("werwerwer"));
        return;
    }

    this->trayIcon->show();
    this->hide();

    STab *tab = qobject_cast<STab*>(this->tabWidget->widget(currentTabIndex));

    ShortViewerDialog *viewer = new ShortViewerDialog(this);

    connect(viewer, SIGNAL(closeViewer(STab *)), this, SLOT(handleCloseShortView(STab *)));
    viewer->setTab(tab);
    viewer->show();

}

void MainWindow::handleCloseShortView(STab *tab)
{
    this->trayIcon->hide();
    tab->showNavigationPanel();
    this->tabWidget->createTab(tab);
    this->show();
}


MainWindow::~MainWindow()
{
    delete ui;
}
