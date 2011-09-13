#include "shortviewerdialog.h"
#include "ui_shortviewerdialog.h"

ShortViewerDialog::ShortViewerDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ShortViewerDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Tool);

    #ifdef Q_OS_WIN
    SetWindowPos(this->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    #else
    #endif

    connect(this, SIGNAL(finished(int)), this, SLOT(handleClose(int)));

    /*
    QDesktopWidget *desktop = QApplication::desktop();

    int x = (desktop->width() - this->width()) / 2;
    int y = (desktop->height() - this->height()) / 2;

    this->setGeometry(x, y, this->width(), this->height());
    */
}

void ShortViewerDialog::setTab(STab *tab)
{
    this->tab = tab;

    this->setWindowTitle(tab->getTitle());

    this->tab->hideNavigationPanel();
    ui->horizontalLayout->addWidget(this->tab);
    this->tab->show();
    ui->horizontalLayout->update();
}

void ShortViewerDialog::resizeEvent(QResizeEvent * event)
{
    ui->horizontalLayoutWidget->setGeometry(QRect(0, 0, this->width(), this->height()));
}

void ShortViewerDialog::handleClose(int id)
{
    emit closeViewer(this->tab);
}

ShortViewerDialog::~ShortViewerDialog()
{
    delete ui;
}
