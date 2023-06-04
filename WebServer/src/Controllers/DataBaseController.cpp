#include "DataBaseController.h"

DataBaseController::DataBaseController(QObject *parent)
    : HttpRequestHandler{parent}
{

}

void DataBaseController::service(HttpRequest &request, HttpResponse &response)
{
    HttpSession session = sessionStore->getSession(request,response,true);
    QString username = session.get("username").toString();
    QString idRole = session.get("idrole").toString();
    QString groupId = session.get("groupid").toString();
    QByteArray language = request.getHeader("Accept-Language");
    qDebug("language=%s",qPrintable(language));
    //set name connect to db
    if(!userDataBase.HaveConnect(QString(session.getId()))){
        userDataBase.setNameConnect(QString(session.getId()));
    }
    response.setHeader("Content-Type", "text/html; charset=UTF-8");
    //get tamplate
    Template temp = templateCache->getTemplate("database",language);
    temp.setCondition("logged-in", session.contains("username"));
    temp.setCondition("isadmin", (idRole == ADMIN));
    temp.setCondition("message", true);

    qDebug() << "DataBaseController: isadmin - " << (idRole == ADMIN);
    QString requestUserName = request.getParameter("userName").constData();
    QString requestSqlReq = request.getParameter("sqlRequest").constData();

    //instrument vars
    const QString requestDoingTbName = request.getParameter("doingtbname").constData();
    const QString requestTypedo = request.getParameter("typedo").constData();
    const QString requestColumnName = request.getParameter("namepole").constData();
    const QString requestColumnType = request.getParameter("typepole").constData();
    const bool requestColumnIsPK = !request.getParameter("isPK").isEmpty();
    const QList<QByteArray> requestInsertValues = request.getParameters("input");
    const QList<QByteArray> requestDoingUsers = request.getParameters("doinguser");
    const QString requestDoingUser(requestDoingUsers.length() >= 2 ? requestDoingUsers[1].constData() : "");
    const QString requestDoingUserPass = request.getParameter("doinguserpass").constData();
    const QString requestDoingUserIdGroup = request.getParameter("groupid").constData();
    const QString requestTbToLink = request.getParameter("nameTableToLink").constData();
    const QString requestFildToLink = request.getParameter("namefildToLink").constData();
    const QString requestIdRowToDelete = request.getParameter("idRowToDelete").constData();
    const QList<QByteArray> requestEditValues = request.getParameters("editFild");
    const QList<QByteArray> requestEditHeaderValues = request.getParameters("headerEditFild");
    const QString requestIdRowEdit = request.getParameter("idRowEdit");
    qDebug() << "DataBaseController: requestTypeDo - " << requestTypedo;
    temp.setVariable("usernamedb", requestUserName);
    temp.setVariable("sqlreqdb", requestSqlReq);
    temp.setVariable("doingtbnamedb", requestDoingTbName);
    temp.setVariable("namepoledb", requestColumnName);

    //first connect
    if((requestTypedo == "") && (!userDataBase.IsConnect())){
        if(idRole == ADMIN){
            if(userDataBase.CreateConnectToDb()){
                qDebug() << "DataBaseController: connect admin";
                qDebug() << "DataBaseController: db connect succesed!";
                temp.setVariable("messagetext", "Status: Connect to database successed!");
                temp.setCondition("sing-up_dataBase", true);
                qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << true;
            }
            else{
                qDebug() << "DataBaseController: db connect faild!";
                temp.setVariable("messagetext", "Status: Connect to database faild!");
                temp.setCondition("sing-up_dataBase", false);
                qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << false;
            }
        }
        else if(idRole == CLIENT){
            if(userDataBase.CreateConnectWithUser(username)){
                qDebug() << "DataBaseController: connect client";
                qDebug() << "DataBaseController: db connect succesed!";
                temp.setVariable("messagetext", " Status: Connect to database successed!");
            }
            else{
                qDebug() << "DataBaseController: db connect faild!";
                temp.setVariable("messagetext", "Status: Connect to database faild!");
                temp.setCondition("sing-up_dataBase", false);
                qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << false;
            }
        }
        else{
            qDebug() << "DataBaseController: id role faild!" << idRole;
            temp.setVariable("messagetext", "Status: Connect faild!");
            temp.setCondition("sing-up_dataBase", false);
            qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << false;
        }
    }
    //connect
    if((requestTypedo == "connect")){
        qDebug() << "DataBaseController: Connection";
        qDebug() << "DataBaseController: username - " << requestUserName;
        if(idRole == ADMIN){
            if(requestUserName != "" && requestUserName != "admin"){
                if(userDataBase.CreateConnectWithUser(requestUserName)){
                    qDebug() << "DataBaseController: connect admin";
                    qDebug() << "DataBaseController: db connect succesed!";
                    temp.setVariable("messagetext", "Status: Connect to database successed!");
                }
                else{
                    temp.setCondition("sing-up_dataBase", false);
                    qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << false;
                    qDebug() << "DataBaseController: db connect faild!";
                    temp.setVariable("messagetext", "Status: Connect to database faild!");
                }
            }
            else{
                if(requestUserName == "" || requestUserName == "admin"){
                    if(userDataBase.CreateConnectToDb()){
                        qDebug() << "DataBaseController: connect admin";
                        qDebug() << "DataBaseController: db connect succesed!";
                        temp.setVariable("messagetext", "Status: Connect to database successed!");
                        temp.setCondition("sing-up_dataBase", true);
                        qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << true;
                    }
                    else{
                        qDebug() << "DataBaseController: db connect faild!";
                        temp.setVariable("messagetext", "Status: Connect to database faild!");
                        temp.setCondition("sing-up_dataBase", false);
                        qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << false;
                    }
                }
            }
        }
        else if(idRole == CLIENT){
            if(username == "postgres"){
                qDebug() << "DataBaseController: no access to connect!";
                temp.setVariable("messagetext", "Status: No access to connect!");
                temp.setCondition("response_not_null", false);
                qDebug() << "DataBaseController: setCondition - response_not_null - " << false;
                temp.setCondition("sing-up_dataBase", false);
                temp.setCondition("listTablesNotNull", false);
                qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << false;
            }
            else{
                if(requestUserName != ""){
                    if(userDataBase.CreateConnectWithUser(requestUserName)){
                        qDebug() << "DataBaseController: db connect succesed!";
                        temp.setVariable("messagetext", " Status: Connect to database successed!");
                    }
                    else{
                        qDebug() << "DataBaseController: db connect faild!";
                        temp.setVariable("messagetext", " Status: Connect to dataBase faild!");
                        temp.setCondition("sing-up_dataBase", false);
                    }
                }
                else{
                    if(userDataBase.CreateConnectWithUser(username)){
                        qDebug() << "DataBaseController: db connect succesed!";
                        temp.setVariable("messagetext", " Status: Connect to database successed!");
                    }
                    else{
                        qDebug() << "DataBaseController: db connect faild!";
                        temp.setVariable("messagetext", " Status: Connect to dataBase faild!");
                        temp.setCondition("sing-up_dataBase", false);
                    }
                }
            }
        }
        else{
            qDebug() << "DataBaseController: Connect faild!";
            temp.setVariable("messagetext", "Status: Connect faild!");
            temp.setCondition("sing-up_dataBase", false);
            qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << false;
        }
    }
    //delete row
    if(userDataBase.IsConnect() && (requestTypedo == "delete_row")){
        if(requestIdRowToDelete != "" && requestDoingTbName != ""){
            qDebug() << "DataBaseController: delete row";
            if(!userDataBase.DropRow(requestDoingTbName, requestIdRowToDelete)){
                qDebug() << "DataBaseController: DeleteRow faild!";
                temp.setVariable("messagetext", " Status: Delete row faild!");
            }
            else{
                qDebug() << "DataBaseController: DeleteRow successed!";
                temp.setVariable("messagetext", " Status: Delete row successed!");
            }
        }
    }
    //set group id to user
    if(userDataBase.IsConnect() && (requestTypedo == "set_groupid")){
        if(requestDoingUser != "" && requestDoingUserIdGroup != ""){
            qDebug() << "DataBaseController: set group id";
            if(!userDataBase.SetIdGroup(requestDoingUser, requestDoingUserIdGroup)){
                qDebug() << "DataBaseController: set group id faild!";
                temp.setVariable("messagetext", " Status: Set group id faild!");
            }
            else{
                qDebug() << "DataBaseController: set group id successed!";
                temp.setVariable("messagetext", " Status: Set group id successed!");
            }
        }
        else{
            qDebug() << "DataBaseController: No data available";
            temp.setVariable("messagetext", " Status: No data available!");
        }
    }
    //edit row
    if(userDataBase.IsConnect() && (requestTypedo == "edit_row")){
        if(requestEditValues.count() > 0 && requestDoingTbName != 0){
            qDebug() << "DataBaseController: edit row";
            temp.setCondition("sing-up_dataBase", true);
            qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << true;
            if(!userDataBase.EditRow(requestDoingTbName, requestEditValues, requestEditHeaderValues, requestIdRowEdit)){
                qDebug() << "DataBaseController: EditRow faild!";
                temp.setVariable("messagetext", " Status: Edit row faild!");
            }
            else{
                qDebug() << "DataBaseController: EditRow successed!";
                temp.setVariable("messagetext", " Status: Edit row successed!");
            }
        }
    }
    //insert row
    if(userDataBase.IsConnect() && (requestTypedo == "append_row")){
        if(requestInsertValues.count() > 0 && requestDoingTbName != ""){
            qDebug() << "DataBaseController: insert row";
            temp.setCondition("sing-up_dataBase", true);
            qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << true;
            if(!userDataBase.InsertRow(requestDoingTbName, requestInsertValues)){
                qDebug() << "DataBaseController: InsertRow faild!";
                temp.setVariable("messagetext", " Status: Insert row faild!");
            }
            else{
                qDebug() << "DataBaseController: InsertRow successed!";
                temp.setVariable("messagetext", " Status: Insert row successed!");
            }
        }
    }
    //insert column
    if(userDataBase.IsConnect()  && requestTypedo == "create"){
        if(requestColumnType != "" && requestDoingTbName != ""){
            temp.setCondition("sing-up_dataBase", true);
            qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << true;
            qDebug() << "DataBaseController: insert column";
            if(!userDataBase.InsertColumn(requestDoingTbName, requestColumnName, requestColumnType, requestColumnIsPK)){
                qDebug() << "DataBaseController: InsertColumn faild!";
                temp.setVariable("messagetext", " Status: InsertColumn faild!");
            }
        }
    }
    //drop column
    if(userDataBase.IsConnect()  && requestTypedo == "drop"){
        if(requestColumnType != "" && requestDoingTbName != ""){
            temp.setCondition("sing-up_dataBase", true);
            qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << true;
            qDebug() << "DataBaseController: drop column";
            if(!userDataBase.DropColumn(requestDoingTbName, requestColumnName)){
                qDebug() << "DataBaseController: DropColumn faild!";
                temp.setVariable("messagetext", " Status: DropColumn faild!");
            }
        }
    }
    //create user
    if(userDataBase.IsConnect() && requestTypedo == "create_user"){
        if((requestDoingUser != "" && requestDoingUserPass != "")){
            temp.setCondition("sing-up_dataBase", true);

            if(userDataBase.InputNewUser(requestDoingUser, "", UserDataBase::CriptStr(requestDoingUserPass), requestDoingUserIdGroup)){
                qDebug() << "DataBaseController: input new user successed!";
                temp.setVariable("messagetext", " Status: Input new user successed!");
            }
            else{
                qDebug() << "DataBaseController: input new user failed!";
                temp.setVariable("messagetext", " Status: Input new user failed!");
            }
        }
        else{
            qDebug() << "DataBaseController: No data available";
            temp.setVariable("messagetext", " Status: No data available!");
            qDebug() << requestDoingUser << requestDoingUserPass;
        }

    }
    //drop user
    if(userDataBase.IsConnect() && requestTypedo == "drop_user"){
        if((requestDoingUser != "")){
            temp.setCondition("sing-up_dataBase", true);
            if(userDataBase.DropUser(requestDoingUser)){
                qDebug() << "DataBaseController: drop user successed!";
                temp.setVariable("messagetext", " Status: drop user successed!");
            }
            else{
                qDebug() << "DataBaseController: drop user failed!";
                temp.setVariable("messagetext", " Status: drop user failed!");
            }
        }
        else{
            qDebug() << "DataBaseController: No data available";
            temp.setVariable("messagetext", " Status: No data available!");
        }
    }
    //create link
    if(userDataBase.IsConnect() && requestTypedo == "create_link"){
        if(requestFildToLink != "" && requestTbToLink != "" && requestColumnName != "" && requestDoingTbName != ""){
            if(!userDataBase.CreateLink(requestDoingTbName,requestColumnName, requestTbToLink, requestFildToLink)){
                qDebug() << "DataBaseController: CreateLink failed";
                temp.setVariable("messagetext", " Status: Create link failed!");
            }
            else{
                qDebug() << "DataBaseController: CreateLink successed";
                temp.setVariable("messagetext", " Status: Create link successed!");
            }
        }
        else{
            qDebug() << "DataBaseController: No data available";
            temp.setVariable("messagetext", " Status: No data available!");
        }
    }
    //show databases list and tables list
    if(userDataBase.IsConnect()){
        temp.setCondition("sing-up_dataBase", true);
        qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << true;

        //show a list of databases
        qDebug() << "DataBaseController: show a list of databases";
        QList<QString> dbList = userDataBase.getDbList(idRole, groupId);
        qDebug() << "DataBaseController: databases - " << dbList;
        temp.loop("listdb", dbList.count());
        if(dbList.count() == 0){
            temp.setVariable("messagetext", "Status: No connect!");
            temp.setCondition("sing-up_dataBase", false);
            qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << false;
        }
        else{
            temp.loop("listdb", dbList.length());
            for(int i = 0; i < dbList.count(); i++){
                temp.setVariable("listdb" + QString::number(i) + ".value", dbList.at(i));
                temp.setVariable("listdb" + QString::number(i) + ".number", QString::number(i));
            }
        }

        //show a list of tables
        qDebug() << "DataBaseController: show a list of tables";
        QList<QList<QString>> tablesInfoList = userDataBase.getTables();
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
    //drop table
    if(userDataBase.IsConnect() && requestTypedo == "drop"){
        if(requestDoingTbName != ""){
            temp.setCondition("sing-up_dataBase", true);
            qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << true;
            if(!userDataBase.DropTable(requestDoingTbName)){
                qDebug() << "DataBaseController: Drop table " << requestDoingTbName << " faild";
                temp.setVariable("messagetext", " Status: Drop table " + requestDoingTbName + " faild");
            }
            else{
                qDebug() << "DataBaseController: drop table";
                temp.setVariable("messagetext", " Status: Drop table successed!");
            }
        }
        else{
            qDebug() << "DataBaseController: No data available";
            temp.setVariable("messagetext", " Status: No data available!");
        }
    }
    //create table
    if(userDataBase.IsConnect() && requestTypedo == "create"){
        if(requestDoingTbName != ""){
            temp.setCondition("sing-up_dataBase", true);
            qDebug() << "DataBaseController: setCondition - sing-up_dataBase - " << true;
            if(!userDataBase.CreateTable(requestDoingTbName)){
                qDebug() << "DataBaseController: Create table " << requestDoingTbName << " faild";
                temp.setVariable("messagetext", " Status: Create table " + requestDoingTbName + " faild");
            }
            else{
                qDebug() << "DataBaseController: Create table";
                temp.setVariable("messagetext", " Status: Create table successed!");
            }
        }
        else{
            qDebug() << "DataBaseController: No data available";
            temp.setVariable("messagetext", " Status: No data available!");
        }
    }
    //create column
    if(userDataBase.IsConnect() && requestTypedo == "create"){
        if(requestColumnName != "" && requestDoingTbName != ""){
            if(!userDataBase.CreateColumn(requestDoingTbName, requestColumnName, requestColumnType, requestColumnIsPK)){
                qDebug() << "DataBaseController: Create column " << requestColumnName << "faild";
                temp.setVariable("messagetext", "Status: Create column " + requestColumnName + " faild");
            }
            else{
                qDebug() << "DataBaseController: Create column";
                temp.setVariable("messagetext", " Status: Create column successed!");
            }
        }
        else{
            qDebug() << "DataBaseController: No data available";
            temp.setVariable("messagetext", " Status: No data available!");
        }
    }
    //show result of sql request
    if(!(requestSqlReq.isEmpty()) && userDataBase.IsConnect() && requestTypedo == "sqlreq"){
        qDebug() << "DataBaseController: show result of sql request";
        QList<QList<QString>> rows = userDataBase.getRequest(requestSqlReq);
        if(!rows.isEmpty()){
            temp.setCondition("response_not_null", true);
            temp.setVariable("messagetext", " Status: Sql request successed!");
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
            temp.setVariable("messagetext", " Status: result is empty!");
            temp.setCondition("response_not_null", false);
            qDebug() << "DataBaseController: setCondition - response_not_null - " << false;
        }
    }
    else{
        temp.setCondition("response_not_null", false);
        qDebug() << "DataBaseController: setCondition - response_not_null - " << false;
    }
    userDataBase.CloseDb();
    response.write(temp.toUtf8(),true);
}

void DataBaseController::LogOut()
{
    userDataBase.CloseDb();
    userDataBase.CloseConnect();

}
