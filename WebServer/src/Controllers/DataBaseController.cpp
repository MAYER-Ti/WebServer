#include "DataBaseController.h"

DataBaseController::DataBaseController(QObject *parent)
    : HttpRequestHandler{parent}
{
    userDataBase.setNameConnect("mainDB_con");
}

void DataBaseController::service(HttpRequest &request, HttpResponse &response)
{
    HttpSession session = sessionStore->getSession(request,response,true);
    QString username = session.get("username").toString();
    QString idRole = "0"/*session.get("idrole").toString()*/;
    QByteArray language = request.getHeader("Accept-Language");
    qDebug("language=%s",qPrintable(language));

    response.setHeader("Content-Type", "text/html; charset=UTF-8");
    //get tamplate header
    Template temp = templateCache->getTemplate("database",language);
    temp.setCondition("logged-in", session.contains("username"));
    temp.setCondition("isadmin", (idRole == ADMIN));
    temp.setCondition("message", true);
    //test !!!!!!
    temp.setCondition("sing-up_dataBase", true);
    temp.setCondition("listTablesNotNull", true);

    qDebug() << "DataBaseController: isadmin - " << (idRole == ADMIN);
    QString requestdbName;
    QString requestUserName;
    QString requestPassword;
    QString requestSqlReq;
    QString requestHostName = "localhost";
    //get var in request
    //var database
    if(idRole == ADMIN){
        requestdbName = request.getParameter("dbname").constData();
        requestUserName = request.getParameter("userName").constData();
        requestPassword = request.getParameter("password").constData();
        requestSqlReq = request.getParameter("sqlRequest").constData();
    }
    else{
        requestdbName = username + "_db";
        requestUserName = username + "_user";
        requestPassword = request.getParameter("password").constData();
        requestSqlReq = request.getParameter("sqlRequest").constData();
    }
    //var instrument
    const QString requestDoingTbName = request.getParameter("doingtbname").constData();
    const QString requestTypedo = request.getParameter("typedo").constData();
    const QString requestColumnName = request.getParameter("namepole").constData();
    const QString requestColumnType = request.getParameter("typepole").constData();
    const bool requestColumnIsPK = !request.getParameter("isPK").isEmpty();
    const QList<QByteArray> requestInsertValues = request.getParameters("input");
    qDebug() << "DataBaseController: requestInsertValues - " << requestInsertValues;
// namedb usernamedb passworddb
    temp.setVariable("namedb", requestdbName);
    temp.setVariable("usernamedb", requestUserName);
    temp.setVariable("passworddb", requestPassword);
    //Connect to db
    if((requestdbName == "" || requestUserName == "" || (idRole == ADMIN && requestPassword != "") || (idRole == CLIENT && requestPassword == "")/*|| requestPassword == ""*/) && (!userDataBase.IsConnect())){
        qDebug() << "DataBaseController: No parameters";
        qDebug() << "namedb:" << requestdbName << "usernamedb:" << requestUserName << "hostnamedb:" << requestHostName << "password:" << requestPassword;
        temp.setCondition("response_not_null", false);
        qDebug() << "DataBaseController: setCondition - response_not_null - " << false;
        temp.setCondition("sing-up_dataBase", false);
        qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << false;
        temp.setCondition("listTablesNotNull", false);
        qDebug() << "DataBaseController: setCondition - listTablesNotNull - " << false;
        temp.setCondition("message", false);
        qDebug() << "DataBaseController: setCondition - message - " << false;
    }
    else if(!requestdbName.isEmpty() &&
            !requestUserName.isEmpty() &&
            !requestHostName.isEmpty()){
        qDebug() << "DataBaseController: Connection";
        qDebug() << "DataBaseController: dbname - " << requestdbName;
        qDebug() << "DataBaseController: username - " << requestUserName;
        qDebug() << "DataBaseController: hostName - " << requestHostName;
        qDebug() << "DataBaseController: password - " << requestPassword;

        if(idRole == ADMIN){
            if(userDataBase.CreateConnectToDb(requestHostName , requestUserName, requestdbName, requestPassword)){
                qDebug() << "DataBaseController: connect admin";
                qDebug() << "DataBaseController: db connect succesed!";
                temp.setVariable("messagetext", "Status: Connect to database successed!");
//                tempMain.setCondition("sing-up_dataBase", true);
//                qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << true;
            }
            else{
                temp.setCondition("sing-up_dataBase", false);
                qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << false;
                qDebug() << "DataBaseController: db connect faild!";
                temp.setVariable("messagetext", "Status: Connect to database faild!");
            }
        }
        else if(idRole == CLIENT){
            if(requestUserName == "postgres" || requestdbName == "postgres"){
                qDebug() << "DataBaseController: no access to connect!";
                temp.setVariable("messagetext", "Status: No access to connect!");
                temp.setCondition("response_not_null", false);
                qDebug() << "DataBaseController: setCondition - response_not_null - " << false;
                temp.setCondition("sing-up_dataBase", false);
                temp.setCondition("listTablesNotNull", false);
                qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << false;
            }
            else{
                if(userDataBase.CreateConnectToDb(/*requestHostName*/ "localhost", requestUserName, requestdbName, requestPassword)){
                    qDebug() << "DataBaseController: connect client";
                    qDebug() << "DataBaseController: db connect succesed!";
                    temp.setVariable("messagetext", " Status: Connect to database successed!");
                }
                else{
                    qDebug() << "DataBaseController: db connect faild!";
                    temp.setVariable("messagetext", " Status: Connect to dataBase faild!");
                }
            }
        }
    }
    //insert row
    if(/*userDataBase.IsConnect() && */(requestInsertValues.count() && requestDoingTbName != "")){
        qDebug() << "DataBaseController: insert row";
        temp.setCondition("sing-up_dataBase", true);
        qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << true;
        if(!userDataBase.InsertRow(requestDoingTbName, requestInsertValues)){
            qDebug() << "DataBaseController: InsertRow faild!";
        }
    }
    //insert column
    if(userDataBase.IsConnect() && (requestColumnType != "" && requestDoingTbName != "" && requestTypedo != "")){
        temp.setCondition("sing-up_dataBase", true);
        qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << true;
        if(requestTypedo == "create"){
            qDebug() << "DataBaseController: insert column";
            if(!userDataBase.InsertColumn(requestDoingTbName, requestColumnName, requestColumnType, requestColumnIsPK)){
                qDebug() << "DataBaseController: InsertColumn faild!";
                temp.setVariable("messagetext", " Status: InsertColumn faild!");
            }
        }
        if(requestTypedo == "drop"){
            qDebug() << "DataBaseController: drop column";
            if(!userDataBase.DropColumn(requestDoingTbName, requestColumnName)){
                qDebug() << "DataBaseController: DropColumn faild!";
                temp.setVariable("messagetext", " Status: DropColumn faild!");
            }
        }
    }
    //show database list and list of tables if connect to db is true
    if(userDataBase.IsConnect()){
        temp.setCondition("sing-up_dataBase", true);

        qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << true;
        if(idRole == ADMIN){
            //show a list of databases
            qDebug() << "DataBaseController: show a list of databases";
            QList<QList<QString>> dbList = userDataBase.getDbList();
            qDebug() << "DataBaseController: databases - " << dbList;
            temp.loop("listdb", dbList.count());
            qDebug() << "DataBaseController: loop - listdb - " << dbList;
            if(dbList.isEmpty()){
                temp.setVariable("messagetext", "Status: No connect!");
                temp.setCondition("sing-up_dataBase", false);
            }
            else{
                int columnCount = dbList.at(0).count();
                for(int i = 0; i < dbList.count(); i++){
                    temp.loop("listdb" + QString::number(i) + ".column", columnCount);
                    temp.setVariable("listdb" + QString::number(i), QString::number(i));
                    for(int j = 0; j < columnCount; j++){
                        temp.setVariable("listdb" + QString::number(i) + ".column" + QString::number(j) + ".value", dbList.at(i).at(j));
                        temp.setVariable("listdb" + QString::number(i) + ".column" + QString::number(j) + ".number", QString::number(j));
                    }
                }
            }
        }
        //show a list of tables
        qDebug() << "DataBaseController: show a list of tables";
        QList<QList<QString>> tablesInfoList = userDataBase.getTables();
        qDebug() << "DataBaseController: loop - tables - " << tablesInfoList;
        if(tablesInfoList.isEmpty()){
            temp.setVariable("messagetext", "Status: No connect!");
            temp.setCondition("sing-up_dataBase", false);
            temp.setCondition("listTablesNotNull", false);
            qDebug() << "DataBaseController: setCondition - listTablesNotNull - " << false;
        }
        else{
            temp.setCondition("listTablesNotNull", true);
            int rowCount = tablesInfoList.count();
            temp.loop("tables", rowCount);
            for(int i = 0; i < rowCount; i++){
                temp.setVariable("tables" + QString::number(i) + ".idrow",QString::number(i));
                temp.setVariable("tables" + QString::number(i) + ".table_name", tablesInfoList.at(i).at(0));
                temp.setVariable("tables" + QString::number(i) + ".column_name", tablesInfoList.at(i).at(1));
                temp.setVariable("tables" + QString::number(i) + ".data_type", tablesInfoList.at(i).at(2));
                temp.setVariable("tables" + QString::number(i) + ".is_nullable", tablesInfoList.at(i).at(3));
            }
        }
    }
    else{
        temp.setCondition("sing-up_dataBase", false);
        temp.setVariable("messagetext", " Status: Error connected!");
        temp.setCondition("listTablesNotNull", false);
    }
    //if doingdb not null and typedo not null create or drop table
    if(userDataBase.IsConnect() && ((requestDoingTbName != "") && (requestTypedo != ""))){

        temp.setCondition("sing-up_dataBase", true);
        qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << true;
        if(requestTypedo == "create"){
            if(requestColumnName != ""){
                if(!userDataBase.CreateColumn(requestDoingTbName, requestColumnName, requestColumnType, requestColumnIsPK)){
                    qDebug() << "DataBaseController: Create column " << requestColumnName << "faild";
                    temp.setVariable("messagetext", "Status: Create table " + requestDoingTbName + " faild");
                }
                else{
                    qDebug() << "DataBaseController: Create column";
                    temp.setVariable("messagetext", " Status: Create column successed!");
                }
            }
            else if(!userDataBase.CreateTable(requestDoingTbName)){
                qDebug() << "DataBaseController: Create table " << requestDoingTbName << " faild";
                temp.setVariable("messagetext", " Status: Create table " + requestDoingTbName + " faild");
            }
            else{
                qDebug() << "DataBaseController: Create table";
                temp.setVariable("messagetext", " Status: Create table successed!");
            }
        }
        else if(requestTypedo == "drop"){
            if(!userDataBase.DropTable(requestDoingTbName)){
                qDebug() << "DataBaseController: Drop table " << requestDoingTbName << " faild";
            }
            else{
                qDebug() << "DataBaseController: drop table";

            }
        }
    }
    //show result of sql request
    if(!(requestSqlReq.isEmpty()) && userDataBase.IsConnect()){
        qDebug() << "DataBaseController: show result of sql request";
        QList<QList<QString>> rows = userDataBase.getRequest(requestSqlReq);
        qDebug() << "DataBaseController: loop - rows - " << rows;
        if(!rows.isEmpty()){
            temp.setCondition("response_not_null", true);
            qDebug() << "DataBaseController: setCondition - response_not_null - " << true;
            temp.loop("row", rows.count());
            for(int i = 0; i < rows.count(); i++){
                temp.loop("row" + QString::number(i) + ".column", rows[i].count());
                for(int j = 0; j < rows[i].count(); j++){
                    temp.setVariable("row" + QString::number(i)+ ".column" + QString::number(j) + ".value", rows[i][j]);
                }
            }
        }
        else{
            temp.setVariable("messagetext", " Status: SQL request not valid!");
            temp.setCondition("response_not_null", false);
            qDebug() << "DataBaseController: setCondition - response_not_null - " << false;
        }
    }
    else{
        temp.setCondition("response_not_null", false);
        qDebug() << "DataBaseController: setCondition - response_not_null - " << false;
    }
    userDataBase.CloseDb();
    //get tamplate end
    response.write(temp.toUtf8(),true);
}

void DataBaseController::LogOut()
{
    userDataBase.CloseDb();
    userDataBase.CloseConnect();

}
