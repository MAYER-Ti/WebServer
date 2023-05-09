#ifndef LISTDATACONTROLLER_H
#define LISTDATACONTROLLER_H

#include "../global.h"
#include <httprequesthandler.h>
#include "template.h"
#include <QTime>

using namespace stefanfrings;

class AboutController : public HttpRequestHandler
{
    Q_OBJECT
public:
    explicit AboutController(QObject *parent = nullptr);
    void service(HttpRequest& request, HttpResponse& response);
private:
};

#endif // LISTDATACONTROLLER_H
