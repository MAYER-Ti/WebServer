#include "RequestMapper.h"

RequestMapper::RequestMapper(QObject *parent)
    : HttpRequestHandler{parent}
{

}

void RequestMapper::service(HttpRequest &request, HttpResponse &response)
{
    QByteArray path=request.getPath();
    qDebug("RequestMapper: path=%s",path.data());

    QByteArray sessionId=sessionStore->getSessionId(request,response);
    if (sessionId.isEmpty() && path!="/login") {
        qDebug("RequestMapper: redirect to login page");
        response.redirect("/login");
        return;
    }
    if(path == "/"){
        response.redirect("/login");
    }
    else if (path.startsWith("/file")){
        staticFileController->service(request, response);
    }
    else if(path == "/database") {
        m_database.service(request, response);
    }
    else if (path == "/list") {
        m_data.service(request, response);
    }
    else if (path == "/cookie") {
        m_cookie.service(request, response);
    }
    else if(path == "/login"){
        m_login.service(request, response);
    }
    else {
        response.setStatus(404,"Not found");
        response.write("The URL is wrong, no such document.",true);
    }

    qDebug("RequestMapper: finished request");
}
