#include "stab.h"
#include "ui_stab.h"

STab::STab(QWidget *parent) : QWidget(parent), ui(new Ui::STab)
{
    ui->setupUi(this);
    ui->progressBar->hide();

    connect(ui->loadButton, SIGNAL(clicked()), this, SLOT(setDownloadStreamPage()));
    connect(ui->urlEdit, SIGNAL(returnPressed()), this, SLOT(setDownloadStreamPage()));
}

void STab::resizeEvent(QResizeEvent * event)
{
    ui->verticalLayoutWidget->setGeometry(QRect(0, 0, this->width(), this->height()));
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
        emit titleChanged(QString("Нет стрима"));
        qDebug() << QString("Response error: ") << reply->error();
        showMessage(QString::fromUtf8("Ошибка загрузки страницы, проверти правильность введенной ссылки."));
        return;
    }

    QString content = QString::fromUtf8(reply->readAll());

    //
    QString szExp = QString("<iframe.*src=\"(http://www.own3d.tv/liveembed/[^\"]+)\"></iframe>");
    QRegExp rx(szExp, Qt::CaseInsensitive);

    int pos = rx.indexIn( content);
    QString szUrl;
    while( ( pos = rx.indexIn( content, pos)) != -1) {
        szUrl = rx.cap(1);
        qDebug() << szUrl;
        pos += rx.matchedLength();
    }

    if (szUrl.isEmpty()) {
        showMessage(QString::fromUtf8("Стрим не был найден на указанной страницы."));
        return;
    }

    connect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(handleFinishStreamLoading(bool)));

    // Получаем title для таба
    QString titleRegExp = QString("<title>([^<]+)</title>");
    QRegExp titleRx(titleRegExp, Qt::CaseInsensitive);

    titleRx.indexIn(content);
    QString title = titleRx.cap(1);

    emit titleChanged(title);


    ui->webView->load(QUrl(szUrl));

    //ui->webView->setContent(reply->readAll());
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
    ui->loadButton->hide();
    ui->urlEdit->hide();
    ui->webView->hide();
    ui->progressBar->show();
}

void STab::prepareUi()
{
    ui->loadButton->show();
    ui->urlEdit->show();
    ui->progressBar->hide();
    ui->webView->show();
}


