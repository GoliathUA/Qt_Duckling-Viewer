#include "tabstreamwidget.h"
#include "ui_tabstreamwidget.h"

TabStreamWidget::TabStreamWidget(QWidget *parent) : QTabWidget(parent), ui(new Ui::TabStreamWidget)
{
    ui->setupUi(this);

    createTab();

    QShortcut *shortcut = new QShortcut(this);
    shortcut->setContext(Qt::ApplicationShortcut);
    shortcut->setKey(Qt::CTRL + Qt::Key_T);
    connect(shortcut, SIGNAL(activated()), this, SLOT(createTab()));
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(handleCloseTab(int)));
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(handleCurrentChanged(int)));
}

void TabStreamWidget::createTab()
{
    STab *tab = new STab(this);
    connect(tab, SIGNAL(titleChanged(QString)), this, SLOT(changeTabTitle(QString)));

    //QIcon icon = QIcon(":/images/title.png");

    this->addTab(tab, QString::fromUtf8("Нет стрима"));
}

void TabStreamWidget::createTab(STab *tab)
{
    this->addTab(tab, tab->getTitle());
    this->setCurrentWidget(tab);
    //qDebug() << this->indexOf(tab);
}

void TabStreamWidget::changeTabTitle(QString title)
{
    STab* tab = qobject_cast<STab*>(const_cast<QObject*>(sender()));

    int tabIndex = this->indexOf(tab);

    this->setTabText(tabIndex, title);
    this->setTabToolTip(tabIndex, title);
}

void TabStreamWidget::handleCloseTab(int index)
{
    this->removeTab(index);
}

void TabStreamWidget::handleCurrentChanged(int index)
{
    if (index != -1) {
        QWidget *tab = this->widget(index);
        tab->resize(this->size());
    }
}


TabStreamWidget::~TabStreamWidget()
{
    delete ui;
}
