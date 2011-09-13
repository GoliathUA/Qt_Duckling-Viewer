#include "stab.h"
#include "ui_stab.h"

STab::STab(QWidget *parent) : QWidget(parent), ui(new Ui::STab)
{
    ui->setupUi(this);
    ui->progressBar->hide();

    connect(ui->loadButton, SIGNAL(clicked()), this, SLOT(setDownloadStreamPage()));
    connect(ui->urlEdit, SIGNAL(returnPressed()), this, SLOT(setDownloadStreamPage()));

    ui->verticalLayout->setAlignment(Qt::AlignTop);
}

void STab::resizeEvent(QResizeEvent * event)
{
    ui->verticalLayoutWidget->setGeometry(QRect(0, 0, this->width(), this->height()));
    ui->frame->setGeometry(0, 0, this->width(), 37);
    ui->horizontalLayoutWidget->setGeometry(ui->frame->x(), ui->frame->y(), ui->frame->width(), ui->frame->height());
}

void STab::setDownloadStreamPage()
{
    prepareUiToLoad();

    ui->progressBar->setValue(5);

    connect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(handleStreamPage(bool)));
    ui->webView->load(QUrl(ui->urlEdit->text()));
}

void STab::handleStreamPage(bool)
{
    disconnect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(handleStreamPage(bool)));

    ui->progressBar->setValue(45);

    QNetworkAccessManager* accessManager = ui->webView->page()->networkAccessManager();
    QNetworkRequest request(ui->webView->url());
    QNetworkReply* reply = accessManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(handleStreamSourcePage()));
}

void STab::handleStreamSourcePage()
{ 
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(const_cast<QObject*>(sender()));

    ui->progressBar->setValue(80);

    disconnect(reply, SIGNAL(finished()), this, SLOT(handleStreamSourcePage()));

    if (reply->error() != 0) {
        emit titleChanged(trUtf8("Stream not found"));
        qDebug() << QString("Response error: ") << reply->error();
        showMessage(trUtf8("Error: Stream not found"));
        return;
    }

    QUrl url = reply->url();

    QRegExp wwwRegExp(QString("^www."), Qt::CaseInsensitive);
    QString host = url.host().replace(wwwRegExp, QString(""));

    qDebug() << QString("Host: ") << host;

    QSettings* const settings = Settings::getHostsInstance();

    settings->beginGroup(host);
    QString type = settings->value("type").toString();

    if (type.isEmpty()) {
        settings->endGroup();
        emit titleChanged(trUtf8("Undefined site"));
        showMessage(trUtf8("Error: Undefined site"));
        return;
    }

    //
    bool isStart = false;
    QString content = QString::fromUtf8(reply->readAll());

    if (type == QString("link")) {
        QString regExp = settings->value("reg_exp").toString();
        isStart = setStreamByLink(regExp, content);
    }

    settings->endGroup();

    if (!isStart) {
        showMessage(trUtf8("Stream not found on cerrent page"));
        return;
    }

    // Получаем title для таба
    QString titleRegExp = QString("<title>([^<]+)</title>");
    QRegExp titleRx(titleRegExp, Qt::CaseInsensitive);

    titleRx.indexIn(content);
    this->title = titleRx.cap(1);

    emit titleChanged(this->title);
}

bool STab::setStreamByLink(const QString& regExp, const QString& source)
{
    QRegExp streamRx(regExp, Qt::CaseInsensitive);

    streamRx.indexIn(source);
    QString url = streamRx.cap(1);

    if (url.isEmpty()) {
        return false;
    }

    connect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(handleFinishStreamLoading(bool)));
    ui->webView->load(QUrl(url));

    return true;
}

void STab::handleFinishStreamLoading(bool)
{
    disconnect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(handleFinishStreamLoading(bool)));

    ui->progressBar->setValue(100);

    prepareUi();
}

void STab::showMessage(const QString& message)
{
    prepareUi();

    QString content = QString("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" /></head><body>");
    content.append(message);
    content.append(QString("</body></html>"));

    ui->webView->setContent(content.toUtf8());
}

void STab::prepareUiToLoad()
{
    ui->loadButton->setEnabled(false);
    ui->urlEdit->setEnabled(false);
    ui->webView->hide();
    ui->progressBar->show();
}

void STab::prepareUi()
{
    ui->loadButton->setEnabled(true);
    ui->urlEdit->setEnabled(true);
    ui->progressBar->hide();
    ui->webView->show();
}

void STab::hideNavigationPanel()
{
    ui->frame->hide();
}

void STab::showNavigationPanel()
{
    ui->frame->show();
}

QString STab::getTitle()
{
    return this->title;
}


