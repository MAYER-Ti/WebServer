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
    QByteArray language = request.getHeader("Accept-Language");
    QString sessionUsername = session.get("username").toString();
    if(sessionUsername == ""){
        if(!userDataBase.HaveConnect(QString(session.getId()))){
            userDataBase.setNameConnect(QString(session.getId()));
        }
        if(userDataBase.CreateConnectToDb()){
            qDebug() << "LoginController: db connect succesed!";
        }
        else{
            qDebug() << "LoginController: db connect faild!";
        }
        //get request login & pass
        QByteArray requestUsername = request.getParameter("username");
        QByteArray requestPassword = request.getParameter("password");
        if(requestUsername != "" && requestPassword != ""){
            //connect or create dbLog if does not exist
            bool isUser = userDataBase.isUser(requestUsername, requestPassword);
            qDebug() << "LoginController: isUser - " << isUser;
            //log-in
            if(sessionUsername == "" && isUser){
                int idRole = userDataBase.getIdRoleFromUser(requestUsername);
                qDebug() << "LoginController: get role id - " << idRole;
                QString idGroup = userDataBase.GetGroupId(requestUsername.constData());
                qDebug() << "LoginController: get group id - " << idGroup;
                session.set("username", requestUsername);
                session.set("idrole", QString::number(idRole));
                session.set("logintime", QTime::currentTime());
                session.set("groupid", idGroup);
            }
            // show request log & pass
            qDebug("LoginController: Request username=%s", requestUsername.constData());
            qDebug("LoginController: Request password=%s", requestPassword.constData());
            qDebug() << "LoginController: Session username=" << sessionUsername;
        }
    }
    //remove connection to db
    userDataBase.RemoveConnection();
    //set http header
    response.setHeader("Content-Type", "text/html; charset=UTF-8");
    //set template
    Template temp = templateCache->getTemplate("login", language);
    temp.setCondition("logged-in", session.contains("username"));
    response.write(temp.toUtf8(), true);
}
