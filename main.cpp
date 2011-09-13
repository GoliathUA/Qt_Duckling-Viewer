#include <QtGui/QApplication>
#include "mainwindow.h"

QApplication *app;
QTranslator *translator;

//QSettings MainWindow::settings;


int main(int argc, char *argv[])
{
    app = new QApplication(argc, argv);

    translator = new QTranslator();
    setTranslator(QLocale::system().name());

    MainWindow appWindow;
    appWindow.show();

    return app->exec();
}

void setTranslator(const QString& lang)
{
    QString langPath = getTranslatorPath(lang);

    if (!QFile::exists(langPath)) {
        langPath = getTranslatorPath(QString("en_EN"));
    }

    if(translator->load(langPath)) {
        app->installTranslator(translator);
    }
}

QString getTranslatorPath(const QString& lang)
{
    QString langPath = QApplication::applicationDirPath();
    langPath.append("/languages/");
    langPath.append(lang);
    langPath.append(".qm");

    return langPath;
}

