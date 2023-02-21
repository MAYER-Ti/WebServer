#ifndef REQUESTMAPPER_H
#define REQUESTMAPPER_H

#include "global.h"

#include <httprequesthandler.h>

#include "Controllers/ListDataController.h"
#include "Controllers/LoginController.h"
#include "Controllers/CookieTestController.h"
#include "Controllers/DataBaseController.h"

using namespace stefanfrings;

class RequestMapper : public HttpRequestHandler
{
    Q_OBJECT
public:
    explicit RequestMapper(QObject *parent = nullptr);
    void service(HttpRequest& request, HttpResponse& response);

    ListDataController m_data;
    LoginController m_login;
    CookieTestController m_cookie;
    DataBaseController m_database;
};

#endif // REQUESTMAPPER_H
