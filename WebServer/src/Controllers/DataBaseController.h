#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <httprequesthandler.h>
#include "src/global.h"
#include <QList>
#include "../userdatabase.h"

#define ADMIN "0"
#define CLIENT "1"

using namespace stefanfrings;

class DataBaseController : public HttpRequestHandler
{
    Q_OBJECT
private:
    UserDataBase userDataBase;
public:
    explicit DataBaseController(QObject *parent = nullptr);
    void service(HttpRequest& request, HttpResponse& response);
    void LogOut();

};

#endif // DATABASECONTROLLER_H
