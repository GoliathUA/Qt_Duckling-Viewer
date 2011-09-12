#include "streamtab.h"
#include "ui_streamtab.h"

StreamTab::StreamTab(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::StreamTab)
{
    ui->setupUi(this);
}

StreamTab::~StreamTab()
{
    delete ui;
}
