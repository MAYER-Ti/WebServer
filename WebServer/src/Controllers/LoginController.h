#ifndef LOGINCONTROLLER_H

#include <httprequesthandler.h>
#include <QTime>
#include "../global.h"
#include "../userdatabase.h"

#define LOGINCONTROLLER_H

#define KEYLOG 173072

using namespace stefanfrings;

class LoginController : public HttpRequestHandler
{
    Q_OBJECT
private:
    UserDataBase userDataBase;
public:
    explicit LoginController(QObject *parent = nullptr);
    void service(HttpRequest& request, HttpResponse& response);
    void CloseDb() { userDataBase.CloseDb(); }

};

#endif // LOGINCONTROLLER_H
