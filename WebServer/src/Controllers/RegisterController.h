#ifndef REGISTERCONTROLLER_H
#define REGISTERCONTROLLER_H

#include <QObject>
#include <httprequesthandler.h>
#include "templatecache.h"
#include "../global.h"
#include "../userdatabase.h"

//#include "LoginController.h"

class RegisterController : public HttpRequestHandler
{
private:
    UserDataBase userDataBase;
public:
    explicit RegisterController(QObject *parent = nullptr);
    void service(HttpRequest& request, HttpResponse& response);
    void CloseDb(){ userDataBase.CloseDb(); }
};

#endif // REGISTERCONTROLLER_H
