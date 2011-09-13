#include "settings.h"

QSettings* Settings::getHostsInstance()
{
    static QSettings settings(QCoreApplication::applicationDirPath().append("/hosts.ini"), QSettings::IniFormat);

    return &settings;
}

