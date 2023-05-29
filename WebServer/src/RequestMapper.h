#ifndef REQUESTMAPPER_H
#define REQUESTMAPPER_H

#include "global.h"

#include <httprequesthandler.h>

#include "Controllers/AboutController.h"
#include "Controllers/LoginController.h"
#include "Controllers/CookieTestController.h"
#include "Controllers/DataBaseController.h"
#include "Controllers/RegisterController.h"
#include "Controllers/HomeController.h"

using namespace stefanfrings;

class RequestMapper : public HttpRequestHandler
{
    Q_OBJECT
public:
    explicit RequestMapper(QObject *parent = nullptr);
    void service(HttpRequest& request, HttpResponse& response);

    AboutController m_about;
    LoginController m_login;
    CookieTestController m_cookie;
    DataBaseController m_database;
    RegisterController m_reg;
    HomeController m_home;


};

#endif // REQUESTMAPPER_H
