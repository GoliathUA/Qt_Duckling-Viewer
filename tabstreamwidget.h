#ifndef TABSTREAMWIDGET_H
#define TABSTREAMWIDGET_H

#include <QTabWidget>
#include <QShortcut>

#include "stab.h"

namespace Ui {
    class TabStreamWidget;
}

class TabStreamWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit TabStreamWidget(QWidget *parent = 0);
    ~TabStreamWidget();

public slots:
    void createTab();
    void createTab(STab *tab);
    void changeTabTitle(QString title);
    void handleCloseTab(int index);
    void handleCurrentChanged(int index);


private:
    Ui::TabStreamWidget *ui;
};

#endif // TABSTREAMWIDGET_H
