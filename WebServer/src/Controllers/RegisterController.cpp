#include "RegisterController.h"
#include <QDebug>

RegisterController::RegisterController(QObject *parent)
    : HttpRequestHandler{parent}
{
    userDataBase.setNameConnect("RegisterDB_con");
    if(userDataBase.CreateConnectToDb()){
        qDebug() << "RegisterController: db connect succesed!";
        userDataBase.CreateDB();
    }
    else
        qDebug() << "RegisterController: db connect faild!";
}

void RegisterController::service(HttpRequest &request, HttpResponse &response)
{
    //cookie session
    HttpSession session = sessionStore->getSession(request,response,true);
    QByteArray language = request.getHeader("Accept-Language");
    QString sessionUsername = session.get("username").toString();
    QString sessionIdRole = session.get("idrole").toString();

    if(sessionUsername == ""){
        //get request login & pass
        QString requestUsername = request.getParameter("username").constData();
        QByteArray requestPassword = request.getParameter("password");
        QString requestYear = request.getParameter("year").constData();

        if(requestUsername != "" && requestPassword != ""){
            //connect or create dbLog if does not exist
            QByteArray pass(requestPassword);
            qDebug() << "RegisterController: " << pass;
            qDebug() << "RegisterController: " << UserDataBase::encodeStr(pass);
            qDebug() << "RegisterController: " << UserDataBase::decodeStr(pass);
            bool isUser = userDataBase.isUser(requestUsername, requestPassword);
            qDebug() << "isUser - " << isUser;

            if(!isUser){
                if(userDataBase.InputNewUser(requestUsername, requestYear, requestPassword)){
                    int idRole = userDataBase.getIdRoleFromUser(requestUsername);

                    session.set("username", requestUsername);
                    session.set("idrole", QString::number(idRole));
                    session.set("logintime", QTime::currentTime());
                    session.set("groupid", "");
                    qDebug() << "insert new user successed!";
                }

                else
                    qDebug() << "insert new user failed!";
            }

            // show request log & pass
            qDebug() << "RegisterController: Request username=" << requestUsername;
            qDebug() << "RegisterController: Request password=" << requestPassword;
            qDebug() << "RegisterController: Session username=" << sessionUsername;
            qDebug() << "RegisterController: Session idrole=" << sessionIdRole;

        }


    }
    //close connection to db
    userDataBase.CloseDb();
    //set http header
    response.setHeader("Content-Type", "text/html; charset=UTF-8");

    //set template
    Template temp = templateCache->getTemplate("register", language);
    temp.setCondition("logged-in", session.contains("username"));
    response.write(temp.toUtf8(), true);
}
