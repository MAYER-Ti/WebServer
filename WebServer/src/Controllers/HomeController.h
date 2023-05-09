#ifndef HOMECONTROLLER_H
#define HOMECONTROLLER_H

#include <httprequesthandler.h>
#include "src/global.h"

using namespace stefanfrings;

class HomeController : public HttpRequestHandler
{
    Q_OBJECT
public:
    explicit HomeController(QObject *parent = nullptr);
    void service(HttpRequest& request, HttpResponse& response);
};

#endif // HOMECONTROLLER_H
