#ifndef STAB_H
#define STAB_H

#include <QWidget>
#include <QtNetwork/QtNetwork>
#include <QtNetwork/QNetworkAccessManager>

#include "settings.h"

namespace Ui {
    class STab;
}

class STab : public QWidget
{
    Q_OBJECT
public:
    explicit STab(QWidget *parent = 0);
    void resizeEvent(QResizeEvent * event);
    void showMessage(const QString& message);
    void hideNavigationPanel();
    void showNavigationPanel();
    QString getTitle();

signals:

public slots:
    void setDownloadStreamPage();
    void handleStreamPage(bool);
    void handleStreamSourcePage();
    void handleFinishStreamLoading(bool);

signals:
     void titleChanged(QString title);

private:
    Ui::STab *ui;
    QString title;

    bool setStreamByLink(const QString& regExp, const QString& source);
    void prepareUiToLoad();
    void prepareUi();
};

#endif // STAB_H
