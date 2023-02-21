#ifndef LISTDATACONTROLLER_H
#define LISTDATACONTROLLER_H

#include "../global.h"
#include <httprequesthandler.h>
#include "template.h"
#include <QTime>

using namespace stefanfrings;

class ListDataController : public HttpRequestHandler
{
    Q_OBJECT
public:
    explicit ListDataController(QObject *parent = nullptr);
    void service(HttpRequest& request, HttpResponse& response);
private:
    QList<QString> list;
};

#endif // LISTDATACONTROLLER_H
