#ifndef STREAMTAB_H
#define STREAMTAB_H

#include <QTabWidget>

namespace Ui {
    class StreamTab;
}

class StreamTab : public QTabWidget
{
    Q_OBJECT

public:
    explicit StreamTab(QWidget *parent = 0);
    ~StreamTab();

private:
    Ui::StreamTab *ui;
};

#endif // STREAMTAB_H
