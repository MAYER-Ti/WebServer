#include "global.h"

#include <QCoreApplication>
#include <QSettings>
#include <QFile>
#include <QDir>

#include "httplistener.h"
#include "httprequesthandler.h"
#include "RequestMapper.h"

using namespace stefanfrings;

QString searchConfigFile() {
    QString binDir = QCoreApplication::applicationDirPath();
    QString appName=QCoreApplication::applicationName();
    qDebug() << "app name" << appName;
    qDebug() << "bin dir" << binDir;
    QString fileName("webserver.ini");

    QStringList searchList;
    searchList.append(binDir);
    searchList.append(binDir+"/etc");
    searchList.append(binDir+"/../etc");
    searchList.append(binDir+"/../"+appName+"/etc");     // for development with shadow build (Linux)
    searchList.append(binDir+"/../../"+appName+"/etc");  // for development with shadow build (Windows)
    searchList.append(QDir::rootPath()+"etc/opt");
    searchList.append(QDir::rootPath()+"etc");

    foreach (QString dir, searchList)
    {
        QFile file(dir+"/"+fileName);
        if (file.exists())
        {
            fileName=QDir(file.fileName()).canonicalPath();
            qDebug("Using config file %s",qPrintable(fileName));
            return fileName;
        }
    }

    // not found
    foreach (QString dir, searchList)
    {
        qWarning("%s/%s not found",qPrintable(dir),qPrintable(fileName));
    }
    qFatal("Cannot find config file %s",qPrintable(fileName));
    return nullptr;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.addLibraryPath(app.applicationDirPath()+"/plugins");
    QString pathIni = searchConfigFile();

    //Session store
    QSettings* sessionSettings = new QSettings(pathIni,QSettings::IniFormat, &app);
    sessionSettings->beginGroup("sessions");
    sessionStore = new HttpSessionStore(sessionSettings, &app);

    // Static file controller
    QSettings* fileSettings = new QSettings(pathIni,QSettings::IniFormat,&app);
    fileSettings->beginGroup("files");
    staticFileController = new StaticFileController(fileSettings, &app);

    // configure template cache
    QSettings* templateCacheSettings = new QSettings(pathIni,QSettings::IniFormat,&app);
    templateCacheSettings->beginGroup("templates");
    templateCache = new TemplateCache(templateCacheSettings, &app);


    // Http settings
    QSettings* listenerSettings = new QSettings(pathIni,QSettings::IniFormat, &app);
    listenerSettings->beginGroup("listener");

    // Start the HTTP server
    new HttpListener(listenerSettings, new RequestMapper(&app), &app);

    return app.exec();
}
