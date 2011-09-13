#ifndef SHORTVIEWERDIALOG_H
#define SHORTVIEWERDIALOG_H

#include <QDialog>

#include "stab.h"
#ifdef Q_OS_WIN
#include <windows.h>
#endif

namespace Ui {
    class ShortViewerDialog;
}

class ShortViewerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShortViewerDialog(QWidget *parent = 0);
    void setTab(STab *tab);
    void resizeEvent(QResizeEvent * event);
    ~ShortViewerDialog();

private:
    Ui::ShortViewerDialog *ui;
    STab *tab;

public slots:
    void handleClose(int id);

signals:
     void closeViewer(STab *tab);

};

#endif // SHORTVIEWERDIALOG_H
