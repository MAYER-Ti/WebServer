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
    QString idRole = session.get("idrole").toString();
    QByteArray language = request.getHeader("Accept-Language");
    qDebug("language=%s",qPrintable(language));

    response.setHeader("Content-Type", "text/html; charset=UTF-8");
    //get tamplate header
    Template temp = templateCache->getTemplate("database",language);
    temp.setCondition("logged-in", session.contains("username"));
    temp.setCondition("isadmin", (idRole == ADMIN));
    temp.setCondition("message", true);

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
//    const QStringList requestInsertValues =
//            QString(request.getParameter("insertvalues").constData()).split(" ");

// namedb usernamedb passworddb
    temp.setVariable("namedb", requestdbName);
    temp.setVariable("usernamedb", requestUserName);
    temp.setVariable("passworddb", requestPassword);
    //Connect to db
    if((requestdbName == "" || requestUserName == "" /*|| requestPassword == ""*/) && (!userDataBase.IsConnect())){
        qDebug() << "DataBaseController: No parameters";
        qDebug() << "namedb:" << requestdbName << "usernamedb:" << requestUserName << "hostnamedb:" << requestHostName << "password:" << requestPassword;
        temp.setCondition("response_not_null", false);
        qDebug() << "DataBaseController: setCondition - response_not_null - " << false;
        temp.setCondition("sing-up_dataBase", false);
        qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << false;
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
                temp.setCondition("message", true);
                temp.setVariable("messagetext", "Connect to database successed!");
//                tempMain.setCondition("sing-up_dataBase", true);
//                qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << true;
            }
            else{
                temp.setCondition("sing-up_dataBase", false);
                qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << false;
                qDebug() << "DataBaseController: db connect faild!";
                temp.setCondition("message", true);
                temp.setVariable("messagetext", "Connect to database faild!");
            }
        }
        else if(idRole == CLIENT){
            if(requestUserName == "postgres" || requestdbName == "postgres"){
                qDebug() << "DataBaseController: no access to connect!";
                temp.setCondition("message", true);
                temp.setVariable("messagetext", "No access to connect!");
                temp.setCondition("response_not_null", false);
                qDebug() << "DataBaseController: setCondition - response_not_null - " << false;
                temp.setCondition("sing-up_dataBase", false);
                qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << false;
            }
            else{
                if(userDataBase.CreateConnectToDb(/*requestHostName*/ "localhost", requestUserName, requestdbName, requestPassword)){
                    qDebug() << "DataBaseController: connect client";
                    qDebug() << "DataBaseController: db connect succesed!";
                    temp.setCondition("message", true);
                    temp.setVariable("messagetext", "Connect to database successed!");
                }
                else{
                    qDebug() << "DataBaseController: db connect faild!";
                    temp.setCondition("message", true);
                    temp.setVariable("messagetext", "Connect to dataBase faild!");
                }
            }
        }
    }
//    //insert row
//    if(userDataBase.IsConnect() && (requestInsertValues.count() && requestDoingTbName != "")){
//        qDebug() << "DataBaseController: insert row";
//        tempMain.setCondition("sing-up_dataBase", true);
//        qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << true;
//        if(!userDataBase.InsertRow(requestDoingTbName, requestInsertValues)){
//            qDebug() << "DataBaseController: InsertRow faild!";
//        }
//    }
    //insert column
    if(userDataBase.IsConnect() && (requestColumnType != "" && requestDoingTbName != "" && requestTypedo != "")){
        temp.setCondition("sing-up_dataBase", true);
        qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << true;
        temp.setCondition("message", true);
        if(requestTypedo == "create"){
            qDebug() << "DataBaseController: insert column";
            if(!userDataBase.InsertColumn(requestDoingTbName, requestColumnName, requestColumnType, requestColumnIsPK)){
                qDebug() << "DataBaseController: InsertColumn faild!";
                temp.setCondition("message", true);
                temp.setVariable("messagetext", "InsertColumn faild!");
            }
        }
        if(requestTypedo == "drop"){
            qDebug() << "DataBaseController: drop column";
            if(!userDataBase.DropColumn(requestDoingTbName, requestColumnName)){
                qDebug() << "DataBaseController: DropColumn faild!";
                temp.setCondition("message", true);
                temp.setVariable("messagetext", "DropColumn faild!");
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
            QList<QList<QString>> dbList = {{"1", "0", "0", "0", "0", "0", "0", "0"},
                                            {"0", "0", "0", "0", "0", "0", "0", "0"},
                                            {"0", "0", "0", "0", "0", "0", "0", "0"}}/*userDataBase.getDbList()*/;
            qDebug() << "DataBaseController: databases - " << dbList;
            temp.loop("listdb", dbList.count());
            qDebug() << "DataBaseController: loop - listdb - " << dbList;
            if(dbList.at(0).at(0) == "0"){
                temp.setCondition("message", true);
                temp.setVariable("messagetext", "Error connected!");
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
        QList<QList<QString>> tablesInfoList = {{"1", "0", "0", "0"},
                                                {"0", "0", "0", "0"},
                                                {"0", "0", "0", "0"},
                                                {"0", "0", "0", "0"}}/*userDataBase.getTables()*/;
        qDebug() << "DataBaseController: loop - tables - " << tablesInfoList;
        if(tablesInfoList.at(0).at(0) == "0"){
            temp.setCondition("message", true);
            temp.setVariable("messagetext", "Error connected!");
            temp.setCondition("sing-up_dataBase", false);
        }
        else{
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
        temp.setVariable("messagetext", "Error connected!");
        temp.setCondition("sing-up_dataBase", false);
    }
    //if doingdb not null and typedo not null create or drop table
    if(userDataBase.IsConnect() && ((requestDoingTbName != "") && (requestTypedo != ""))){

        temp.setCondition("sing-up_dataBase", true);
        qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << true;
        if(requestTypedo == "create"){
            if(!userDataBase.CreateTable(requestDoingTbName)){
                qDebug() << "DataBaseController: Create table " << requestDoingTbName << " faild";
                temp.setCondition("message", true);
                temp.setVariable("messagetext", "Create table " + requestDoingTbName + " faild");
            }
            else{
                qDebug() << "DataBaseController: Create table";
                temp.setCondition("message", true);
                temp.setVariable("messagetext", "Create table successed!");
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
            temp.setCondition("message", true);
            temp.setVariable("messagetext", "SQL request not valid!");
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
