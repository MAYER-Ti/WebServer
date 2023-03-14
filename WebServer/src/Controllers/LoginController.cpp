#include "LoginController.h"
#include "src/global.h"


LoginController::LoginController(QObject *parent)
    : HttpRequestHandler{parent}
{

}

void LoginController::service(HttpRequest &request, HttpResponse &response)
{
    //cookie session
    HttpSession session = sessionStore->getSession(request,response,true);

    QString sessionUsername = session.get("username").toString();

    //get request login & pass
    QByteArray requestUsername = request.getParameter("username");
    QByteArray requestPassword = request.getParameter("password");

    //log-in
    if(sessionUsername == "" && requestUsername == "admin" && requestPassword == "admin"){
        session.set("username", requestUsername);
        session.set("logintime", QTime::currentTime());
    }
    // show request log & pass
    qDebug("LoginController: Request username=%s", requestUsername.constData());
    qDebug("LoginController: Request password=%s", requestUsername.constData());
    qDebug() << "LoginController: Session username=" << sessionUsername;


    //set http header
    response.setHeader("Content-Type", "text/html; charset=UTF-8");
    //get tamplate head
    Template tempHead = templateCache->getTemplate("head");
    response.write(tempHead.toUtf8());
    //get tamplate header and set condition logged-in
    Template tempHeader = templateCache->getTemplate("header");
    tempHeader.setCondition("logged-in", session.contains("username"));
    response.write(tempHeader.toUtf8());

    //get tamplate login and set condition logged-in
    Template tempLogin = templateCache->getTemplate("main_login");
    tempLogin.setCondition("logged-in", session.contains("username"));
    response.write(tempLogin.toUtf8());

    //get tamplate footer
    Template tempFooter = templateCache->getTemplate("footer");
    response.write(tempFooter.toUtf8());

    //get tamplate end
    Template tempEnd = templateCache->getTemplate("end");
    response.write(tempEnd.toUtf8(),true);
}
