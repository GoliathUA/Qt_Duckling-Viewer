#ifndef STAB_H
#define STAB_H

#include <QWidget>
#include <QtNetwork/QtNetwork>
#include <QtNetwork/QNetworkAccessManager>
#include <QWebFrame>


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
    QString currentGroup;

    bool setStreamByLink(const QString& regExp, const QString& source);
    bool setStreamByEmbed(const QString& regExp, const QString& source);
    void prepareUiToLoad();
    void prepareUi();
    bool exeStream(const QString& groupName, const QString& source);
};

#endif // STAB_H
