#include "DataBaseController.h"

DataBaseController::DataBaseController(QObject *parent)
    : HttpRequestHandler{parent}
{

}

void DataBaseController::service(HttpRequest &request, HttpResponse &response)
{
    HttpSession session = sessionStore->getSession(request,response,true);
    //QString username = session.get("username").toString();

    QByteArray language = request.getHeader("Accept-Language");
    qDebug("language=%s",qPrintable(language));

    response.setHeader("Content-Type", "text/html; charset=UTF-8");
    //get tamplate head
    Template tempHead = templateCache->getTemplate("head",language);
    response.write(tempHead.toUtf8());
    //get tamplate header
    Template tempHeader = templateCache->getTemplate("header",language);
    tempHeader.setCondition("logged-in", session.contains("username"));
    response.write(tempHeader.toUtf8());

    //get tamplate main
    Template tempMain = templateCache->getTemplate("main_db",language);
    tempMain.setCondition("logged-in", session.contains("username"));


    QList<QByteArray> listParametersKeys = request.getParameterMap().uniqueKeys();
    if(!listParametersKeys.isEmpty()){
        qDebug() << "DataBaseController: Parameters input:" << listParametersKeys;
    }
    QString dbName = "postgres"/*request.getParameter("dbName").constData()*/;
    QString userName = "postgres"/*request.getParameter("userName").constData()*/;
    QString hostName = "127.0.0.1"/*request.getParameter("hostName").constData()*/;
    QString password = "superuserdbpass"/*request.getParameter("password").constData()*/;


    //isDbLogin = true;
    QSqlDatabase dataBase;
    dataBase = QSqlDatabase::addDatabase("PSQL7", "condb");
    dataBase.setDatabaseName(dbName);
    dataBase.setUserName(userName);
    dataBase.setHostName(hostName);
    dataBase.setPassword(password);

    if(!dataBase.isOpen())
        qDebug() << "DataBaseController: database not open!";
    else
        qDebug() << "DataBaseController: database open!";



    if(!request.getParameter("sqlRequest").isNull()){
        tempMain.setCondition("response_not_null", true);



        QString str = request.getParameter("sqlRequest").constData();
        QList<QList<QString>> rows = GetFromDataBase(str);
        qDebug() << rows;

        dataBase.close();
        dataBase.removeDatabase("condb");



        qDebug() << "row count:" << rows.count();

        qDebug() << "column count:" << rows[0].count();

        //set template var
        tempMain.loop("row", rows.count());
        for(int i = 0; i < rows.count(); i++){
            tempMain.loop("row" + QString::number(i) + ".column", rows[i].count());
            for(int j = 0; j < rows[i].count(); j++){
                tempMain.setVariable("row" + QString::number(i)+ ".column" + QString::number(j) + ".value", rows[i][j]);
            }
        }
    }
    else{
        tempMain.setCondition("response_not_null", false);
    }

    response.write(tempMain.toUtf8());
    //get tamplate footer
    Template tempfooter = templateCache->getTemplate("footer",language);
    response.write(tempfooter.toUtf8());

    //get tamplate end
    Template tempEnd = templateCache->getTemplate("end",language);
    response.write(tempEnd.toUtf8(),true);

}

QList<QList<QString>> DataBaseController::GetFromDataBase(QString sqlReq){
    QSqlQuery query;
    QList<QList<QString>> rows;
    query.prepare(sqlReq);
    if(!query.isValid()){
        qDebug() << "SQL request:" << sqlReq;
        qDebug() << "SQL request is not valid!";
        return {{"0"}};
    }
    if(!query.exec()){
        qDebug() << "SQL operation failed!";
        return {{"0"}};
    }
    int columnCount = query.record().count()-1;
    //set header row
    QList<QString> headerRow;
    for(int i = 0; i < columnCount; i++){
        headerRow.append(query.record().fieldName(i));
    }
    rows.append(headerRow);

    //set value rows
    while(query.next()){
        QList<QString> valueRow;
        for(int i = 0; i < columnCount; i++){
            valueRow.append(query.value(i).toString());
        }
        rows.append(valueRow);
    }
    return rows;
}
