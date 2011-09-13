#ifndef SETTINGS_H
#define SETTINGS_H
#include <QSettings>
#include <QCoreApplication>


class Settings
{
public:
    static QSettings *getHostsInstance();
private:
    Settings();
    Settings(const Settings&);
    Settings& operator= (const Settings&);
};
#endif // SETTINGS_H
