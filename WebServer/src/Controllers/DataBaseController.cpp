#include "DataBaseController.h"

DataBaseController::DataBaseController(QObject *parent)
    : HttpRequestHandler{parent}
{

}

void DataBaseController::service(HttpRequest &request, HttpResponse &response)
{
    HttpSession session = sessionStore->getSession(request,response,true);
    QString username = session.get("username").toString();
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
    tempMain.setCondition("response_not_null", false);


    QList listParametersKeys = request.getParameterMap().uniqueKeys();
    if(!listParametersKeys.isEmpty()){
        qDebug() << "Parameters input:" << listParametersKeys;
        qDebug() << request.getParameters(listParametersKeys.at(0));
    }


    //if(request.getParameters(listParametersKeys))

    if(!request.getParameter("sqlRequest").isNull()){
        tempMain.setCondition("response_not_null", true);

        QSqlDatabase dataBase;
        dataBase = QSqlDatabase::addDatabase("QPSQL");
        dataBase.setDatabaseName(request.getParameter("dbName").constData());
        dataBase.setUserName(request.getParameter("userName").constData());
        dataBase.setHostName(request.getParameter("hostName").constData());
        dataBase.setPassword(request.getParameter("password").constData());
        dataBase.open();
        qDebug() << dataBase;

        QString str = request.getParameter("sqlRequest").constData();
        QList<QList<QString>> rows = GetFromDataBase(str);

        dataBase.close();

        tempMain.loop("row", rows.count()-1);
        tempMain.loop("column", rows[0].count()-1);

        //set template var
        for(int i = 0; i < rows.count(); i++)
            for(int j = 0; j < rows[i].count(); j++)
                tempMain.setVariable("row" + QString::number(i) + QString::number(j) + ".number", rows[i][j]);

    }

    response.write(tempMain.toUtf8());
    //get tamplate footer
    Template tempfooter = templateCache->getTemplate("footer",language);
    response.write(tempfooter.toUtf8());

    //get tamplate end
    Template tempEnd = templateCache->getTemplate("end",language);
    response.write(tempEnd.toUtf8(),true);

    //        Template t = templateCache->getTemplate("database",language);
    //        t.setVariable("NAME",username);
    //        t.setCondition("logged-in", !username.isEmpty());
    //        t.loop("row", list.size());
    //        for(int i = 0; i < list.size(); i++){
    //            QString number = QString::number(i);
    //            QString name = list.at(i);
    //            t.setVariable("row" + number + ".number", number);
    //            t.setVariable("row" + number + ".name", name);
    //        }
    //        response.write(t.toUtf8(),true);

}

QList<QList<QString>> DataBaseController::GetFromDataBase(QString sqlReq){
    QSqlQuery query;
    QList<QList<QString>> rows;
    query.prepare(sqlReq);
    if(!query.isValid()){
        qDebug() << "SQL request is not valid!";
        return rows;
    }
    if(!query.exec()){
        qDebug() << "SQL operation failed!";
        return rows;
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
