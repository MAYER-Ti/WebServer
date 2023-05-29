#include "LoginController.h"
#include "src/global.h"


LoginController::LoginController(QObject *parent)
    : HttpRequestHandler{parent}
{
    userDataBase.setNameConnect("loginDB_con");
    if(userDataBase.CreateConnectToDb()){
        userDataBase.CreateDB();
        qDebug() << "LoginController: db connect succesed!";
    }
    else
        qDebug() << "LoginController: db connect faild!";
}

void LoginController::service(HttpRequest &request, HttpResponse &response)
{
    //cookie session
    HttpSession session = sessionStore->getSession(request,response,true);
    QByteArray language = request.getHeader("Accept-Language");
    QString sessionUsername = session.get("username").toString();
    if(sessionUsername == ""){
        //get request login & pass
        QByteArray requestUsername = request.getParameter("username");
        QByteArray requestPassword = request.getParameter("password");;
        if(requestUsername != "" && requestPassword != ""){
            //connect or create dbLog if does not exist

            bool isUser = userDataBase.isUser(requestUsername, requestPassword);
            qDebug() << "LoginController: isUser - " << isUser;


            //log-in
            if(sessionUsername == "" && isUser/*requestUsername == "admin" && requestPassword == "admin"*/){
                int idRole = userDataBase.getIdRoleFromUser(requestUsername);

                session.set("username", requestUsername);
                session.set("idrole", QString::number(idRole));
                session.set("logintime", QTime::currentTime());
                QString groupidUser;
                QList<QList<QString>> grid = userDataBase.getRequest(QString("SELECT groupid FROM logusers WHERE loginuser = '%1'").arg(requestUsername.constData()));
                qDebug() << "LoginController: get group id - " << grid;
                if(!grid.isEmpty())
                    groupidUser = grid.at(1).at(0);
                else
                    groupidUser = "";
                session.set("groupid", groupidUser);
            }

            // show request log & pass
            qDebug("LoginController: Request username=%s", requestUsername.constData());
            qDebug("LoginController: Request password=%s", requestPassword.constData());
            qDebug() << "LoginController: Session username=" << sessionUsername;
        }
    }
    //close connection to db
    userDataBase.CloseDb();
    //set http header
    response.setHeader("Content-Type", "text/html; charset=UTF-8");

    //set template
    Template temp = templateCache->getTemplate("login", language);
    temp.setCondition("logged-in", session.contains("username"));
    response.write(temp.toUtf8(), true);
}
