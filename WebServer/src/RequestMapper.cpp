#include "RequestMapper.h"

RequestMapper::RequestMapper(QObject *parent)
    : HttpRequestHandler{parent}
{

}

void RequestMapper::service(HttpRequest &request, HttpResponse &response)
{
    QByteArray path = request.getPath();
    qDebug("RequestMapper: path=%s",path.data());

    HttpSession session = sessionStore->getSession(request,response,true);

    QByteArray sessionId = sessionStore->getSessionId(request,response);
    QString username = session.get("username").toString();
    QString idRole = session.get("idrole").toString();
    qDebug() << "RequestMapper:" << "username" << username;
    qDebug() << "RequestMapper:" << "idrole" << idRole;

    if(path == "/logout"){
        if(!sessionId.isEmpty() && username != ""){
            session.remove("username");
            session.remove("idrole");
            session.remove("logintime");
            m_login.CloseDb();
            m_reg.CloseDb();
            m_database.LogOut();
            qDebug() << "!! DELETED!";
        }
        response.redirect("/login");
        return;
    }
    if (sessionId.isEmpty() && path != "/login" && path != "/register" && path != "/about"&& path != "/home") {
        qDebug("RequestMapper: redirect to login page");
        response.redirect("/login");
        return;
    }
    if(path == "/"){
        response.redirect("/home");
    }
    else if (path.startsWith("/file")){
        staticFileController->service(request, response);
    }
    else if(path == "/home"){
        m_home.service(request, response);
    }
    else if(path == "/login"){
        m_login.service(request, response);
    }
    else if(path == "/register"){
        m_reg.service(request, response);
    }
    else if(path == "/database") {
        m_database.service(request, response);
    }
    else if (path == "/about") {
        m_about.service(request, response);
    }
    else if (path == "/cookie") {
        m_cookie.service(request, response);
    }
    else {
        response.setStatus(404,"Not found");
        response.write("The URL is wrong, no such document.",true);
    }
    qDebug("RequestMapper: finished request");
}
