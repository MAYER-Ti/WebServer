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
    //get loin & pass
    QByteArray username = request.getParameter("username");
    QByteArray password = request.getParameter("password");
    // show log & pass
    qDebug("username=%s", username.constData());
    qDebug("password=%s", password.constData());

    //set http header
    response.setHeader("Content-Type", "text/html; charset=UTF-8");
    //get tamplate head
    Template tempHead = templateCache->getTemplate("head");
    response.write(tempHead.toUtf8());
    //get tamplate header and set condition logged-in
    Template tempHeader = templateCache->getTemplate("header");
    tempHeader.setCondition("logged-in", session.contains("username"));
    response.write(tempHeader.toUtf8());
    if(!session.contains("username") ||  (username != "admin" && password != "admin")){
        //get tamplate login and set condition logged-in
        Template tempLogin = templateCache->getTemplate("main_login");
        tempLogin.setCondition("logged-in", session.contains("username"));
        response.write(tempLogin.toUtf8());
    }
    else{
        response.write("<main class='main-index'><center><h2>loggin is successful</h2></center></main>");
    }

    if (username == "admin" and password == "admin") {
        session.set("username", username);
        session.set("logintime", QTime::currentTime());
    }

    //get tamplate footer
    Template tempFooter = templateCache->getTemplate("footer");
    response.write(tempFooter.toUtf8());

    //get tamplate end
    Template tempEnd = templateCache->getTemplate("end");
    response.write(tempEnd.toUtf8(),true);
}
