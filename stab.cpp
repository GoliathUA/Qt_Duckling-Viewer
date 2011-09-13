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
    qDebug("STab::setDownloadStreamPage");

    prepareUiToLoad();

    ui->progressBar->setValue(5);

    connect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(handleStreamPage(bool)));

    ui->webView->load(QUrl(ui->urlEdit->text()));
}

void STab::handleStreamPage(bool isLoad)
{
    qDebug("STab::handleStreamPage");
    disconnect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(handleStreamPage(bool)));

    if (!isLoad) {
        emit titleChanged(trUtf8("Stream not found"));
        showMessage(trUtf8("Error: Stream not found"));
        return;
    }

    ui->progressBar->setValue(45);

    QNetworkAccessManager* accessManager = ui->webView->page()->networkAccessManager();
    QNetworkRequest request(ui->webView->url());
    QNetworkReply* reply = accessManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(handleStreamSourcePage()));
}

void STab::handleStreamSourcePage()
{
    qDebug("STab::handleStreamSourcePage");

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

    qDebug() << QString("Host") << host;

    QSettings* const settings = Settings::getHostsInstance();

    settings->beginGroup(host);
    QString type = settings->value("type").toString();
    settings->endGroup();

    if (type.isEmpty()) {
        this->currentGroup = QString("default");
    } else {
        this->currentGroup = host;
    }

    QString content = QString::fromUtf8(reply->readAll());

    if (!exeStream(this->currentGroup, content)) {
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

bool STab::setStreamByEmbed(const QString& regExp, const QString& source)
{
    qDebug("STab::setStreamByEmbed");

    QRegExp streamRx(regExp, Qt::CaseInsensitive, QRegExp::RegExp2);
    streamRx.setMinimal(true);

    streamRx.indexIn(source);
    QString code = streamRx.cap(1);

    if (code.isEmpty()) {
        return false;
    }

    connect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(handleFinishStreamLoading(bool)));

    QString content("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" /><style>body{padding:0px; margin:0px;}</style></head><body>");
    content.append(code);
    content.append("</body></html>");

    ui->webView->setContent(content.toUtf8());
    return true;
}


void STab::handleFinishStreamLoading(bool)
{
    qDebug("STab::handleFinishStreamLoading");

    disconnect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(handleFinishStreamLoading(bool)));

    ui->progressBar->setValue(99);

    //
    QSettings* const settings = Settings::getHostsInstance();
    settings->beginGroup(this->currentGroup);
    QString mode = settings->value("mode").toString();
    if (mode == QString("regexps")) {
        QWebFrame *frame = ui->webView->page()->mainFrame();
        QString content = frame->toHtml();

        int size = settings->beginReadArray("rule");
        for (int i = 0; i < size; ++i) {
            settings->setArrayIndex(i);

            QRegExp rx(settings->value("from").toString());
            content.replace(rx, settings->value("to").toString());
        }

        settings->endArray();

        qDebug() << content;

        ui->webView->setContent(content.toUtf8());
    }
    settings->endGroup();

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

bool STab::exeStream(const QString& groupName, const QString& source)
{
    qDebug("STab::exeStream");

    this->currentGroup = groupName;

    QSettings* const settings = Settings::getHostsInstance();

    settings->beginGroup(groupName);
    QString type = settings->value("type").toString();
    settings->endGroup();

    qDebug() << QString("Type") << type;

    if (type == QString("link")) {
        settings->beginGroup(groupName);
        QString regExp = settings->value("reg_exp").toString();
        settings->endGroup();
        qDebug() << QString("Stream RegExp: ") << regExp;
        return setStreamByLink(regExp, source);
    } else if (type == QString("multi")) {

        settings->beginGroup(groupName);
        QList<QString> lists;
        int size = settings->beginReadArray("rule");
        for (int i = 0; i < size; ++i) {
            settings->setArrayIndex(i);
            QString nextGroupName = settings->value("name").toString();
            lists.append(nextGroupName);
        }

        settings->endArray();
        settings->endGroup();

        for (int j = 0; j < lists.size(); j++) {
            qDebug() << QString("Group name") << j << lists[j];
            if(exeStream(lists[j], source)) {
                return true;
            }
        }

    } else if (type == QString("embed")) {
        settings->beginGroup(groupName);
        QString regExp = settings->value("reg_exp").toString();
        settings->endGroup();
        qDebug() << QString("Stream RegExp") << regExp;
        return setStreamByEmbed(regExp, source);
    }

    return false;
}



