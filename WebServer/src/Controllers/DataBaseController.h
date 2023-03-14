#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <httprequesthandler.h>
#include "templatecache.h"
#include "src/global.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlDriver>
#include <QList>

using namespace stefanfrings;

class DataBaseController : public HttpRequestHandler
{
    Q_OBJECT
private:

public:
    explicit DataBaseController(QObject *parent = nullptr);
    void service(HttpRequest& request, HttpResponse& response);
private:
    QList<QList<QString>> GetFromDataBase(QString sqlReq);
};

#endif // DATABASECONTROLLER_H
