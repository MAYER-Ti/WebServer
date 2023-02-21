#ifndef LOGINCONTROLLER_H
#define LOGINCONTROLLER_H

#include <httprequesthandler.h>
#include <QTime>

using namespace stefanfrings;

class LoginController : public HttpRequestHandler
{
    Q_OBJECT
public:
    explicit LoginController(QObject *parent = nullptr);
    void service(HttpRequest& request, HttpResponse& response);
};

#endif // LOGINCONTROLLER_H
