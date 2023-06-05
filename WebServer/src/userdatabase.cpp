#include "userdatabase.h"
#include <QDebug>

bool UserDataBase::isUser(QString userLogin, QString userPass)
{
    if(userLogin == "" || userPass == "")
        return false;
    if(!m_ndb.open()){
        qDebug() << "UserDataBase: isUser - Open -" << m_ndb.lastError().text();
        return false;
    }

    QSqlQuery query(m_ndb);

    QString pasCode = UserDataBase::CriptStr(userPass);
    QString sqlReq = QString("SELECT loginuser, passworduser FROM logusers WHERE loginuser = '%1' AND passworduser = '%2';").arg(userLogin).arg(pasCode);
    if(!query.exec(sqlReq)){
        qDebug() << "isUser: Error - " << query.lastError().text();
        m_ndb.close();
        return false;
    }
    bool isContain = false;
    while(query.next()){
        if((query.value("loginuser").toString() == userLogin) && (query.value("passworduser").toString() == pasCode)){
            isContain = true;
            break;
        }
    }
    m_ndb.close();
    return isContain;
}

bool UserDataBase::InputNewUser(QString userLogin, QString userYear, QString userPass, QString groupIdText)
{
    if(userLogin == "" || userPass == "" || userLogin == "admin"){
        return false;
    }
    if(!m_ndb.open()){
        qDebug() << "UserDataBase: InputNewUser - Open - " << m_ndb.lastError().text();
        return false;
    }
    QSqlQuery query(m_ndb);
    qDebug() << "new user pass - " << userPass;
    QString codePas = UserDataBase::CriptStr(userPass);
    qDebug() << "new user pass code - " <<  codePas;
    qDebug() << "new user pass decode - " <<  UserDataBase::CriptStr(codePas);

    QString sqlReq = QString("SELECT loginuser FROM logusers WHERE loginuser = '%1';")
            .arg(userLogin);
    if(!query.exec(sqlReq)){
        qDebug() << "UserDataBase: InputNewUser - select - " << query.lastError().text();
        m_ndb.close();
        return false;
    }
    while(query.next()){
        if(query.value("loginuser").toString() == userLogin){
            m_ndb.close();
            return false;
            break;
        }
    }
    if(groupIdText == "" || "null"){
        groupIdText ="NULL";
    }
    sqlReq = QString("INSERT INTO logusers(loginUser, yearUser, passwordUser, groupid)"
                     "VALUES('%1', '%2', '%3', %4);")
            .arg(userLogin)
            .arg(userYear)
            .arg(codePas)
            .arg(groupIdText);
    if(!query.exec(sqlReq)){
        qDebug() << "UserDataBase: InputNewUser - insert - " << query.lastError().text();
        m_ndb.close();
        return false;
    }
    sqlReq = QString("INSERT INTO logusersrols(idUser, idRole)"
                     "VALUES((SELECT id FROM logusers WHERE loginUser = '%1'), '1');")
            .arg(userLogin);
    if(!query.exec(sqlReq)){
        qDebug() << "UserDataBase: InputNewUser - insert logusersrols - " << query.lastError().text();
        m_ndb.close();
        return false;
    }
    sqlReq = QString("CREATE USER %1_user WITH PASSWORD '%2';")
            .arg(userLogin).arg(userPass);
    qDebug() << sqlReq;
    if(!query.exec(sqlReq)){
        qDebug() << "UserDataBase: InputNewUser - create user - " << query.lastError().text();
        m_ndb.close();
        return false;
    }
    sqlReq = QString("CREATE DATABASE %1_db WITH OWNER %1_user;")
            .arg(userLogin);
    qDebug() << sqlReq;
    if(!query.exec(sqlReq)){
        qDebug() << "UserDataBase: InputNewUser - create database - " << query.lastError().text();
        m_ndb.close();
        return false;
    }
    m_ndb.close();
    return true;
}

bool UserDataBase::DropUser(QString userLogin)
{
    if(!m_ndb.open()){
        qDebug() << "UserDataBase: DropUser - Open - " << m_ndb.lastError().text();
        return false;
    }
    QSqlQuery query(m_ndb);
    QString sqlReq = QString("DELETE FROM logusers WHERE loginuser = '%1';")
            .arg(userLogin);
    if(!query.exec(sqlReq)){
        qDebug() << "UserDataBase: DropUser - delete - " << query.lastError().text();
        m_ndb.close();
        return false;
    }
    sqlReq = QString("DROP DATABASE IF EXISTS %1_db;")
            .arg(userLogin);
    if(!query.exec(sqlReq)){
        qDebug() << "UserDataBase: DropUser - drop user- " << query.lastError().text();
        m_ndb.close();
        return false;
    }

    sqlReq = QString("DROP OWNED BY %1_user;")
            .arg(userLogin);
    if(!query.exec(sqlReq)){
        qDebug() << "UserDataBase: DropUser - drop own user- " << query.lastError().text();
        m_ndb.close();
        return false;
    }
    sqlReq = QString("DROP USER IF EXISTS %1_user;")
            .arg(userLogin);
    if(!query.exec(sqlReq)){
        qDebug() << "UserDataBase: DropUser - drop user- " << query.lastError().text();
        m_ndb.close();
        return false;
    }
    return true;
}

QString UserDataBase::GetGroupId(QString username)
{
    QString result = "";
    if(!m_ndb.open()){
        qDebug() << "UserDataBase: GetGroupId - Open - " << m_ndb.lastError().text();
        return result;
    }
    QSqlQuery query(m_ndb);
    QString sqlReq = QString("SELECT groupid FROM logusers WHERE loginuser = '%1'").arg(username);
    if(!query.exec(sqlReq)){
        qDebug() << "UserDataBase: GetGroupId - Select - " << query.lastError().text();
        m_ndb.close();
        return result;
    }
    while(query.next()){
        result = query.value("groupid").toString();
    }
    m_ndb.close();
    return result;
}

QString UserDataBase::CriptStr(QString toCrypt)
{
    QString resultString = "";
    for ( int i = 0; i < toCrypt.length(); i++)
        resultString.append(QString(QChar(toCrypt[i]).unicode() ^ 0x211));
    return resultString;
}

QList<QList<QString>> UserDataBase::getRequest(QString sqlReq)
{
    QList<QList<QString>> rows;
    if(!m_ndb.open()){
        qDebug() << "UserDataBase: getRequest - Open -" << m_ndb.lastError().text();
        return rows;
    }
    qDebug() << "UserDataBase: sql request - " << QString(sqlReq);
    QSqlQuery query(m_ndb);
    if(!query.exec(QString(sqlReq))){
        qDebug() << "SQL operation failed!";
        qDebug() << query.lastError();
        m_ndb.close();
        return rows;
    }
    if(query.size() == 0){
        m_ndb.close();
        return rows;
    }
    int columnCount = query.record().count()-1;
    //set header row
    QList<QString> headerRow;
    for(int i = 0; i <= columnCount; i++){
        headerRow.append(query.record().fieldName(i));
    }
    rows.append(headerRow);
    //set value rows
    while(query.next()){
        QList<QString> valueRow;
        for(int i = 0; i <= columnCount; i++){
            valueRow.append(query.value(i).toString());
        }
        rows.append(valueRow);
    }
    if(rows.isEmpty()){
        m_ndb.close();
        return rows;
    }
    qDebug() << "DataBaseController: row count:" << rows.count();
    qDebug() << "DataBaseController: column count:" << rows[0].count();
    m_ndb.close();
    return rows;
}

bool UserDataBase::HaveConnect(QString nameCon)
{
    QStringList connections =  m_ndb.connectionNames();
    for(int i = 0; i < connections.length(); i++){
        if(connections[i] == nameCon+"_con")
            return true;
    }
    return false;
}

bool UserDataBase::DropRow(QString tbName, QString idRow)
{
    if(!m_ndb.open()){
        qDebug() << "UserDataBase: DropRow - Open -" << m_ndb.lastError().text();
        return false;
    }
    QSqlQuery query(m_ndb);
    QString sqlReq = QString("DELETE FROM %1 WHERE id = %2;").arg(tbName).arg(idRow);
    qDebug() << "UserDataBase: DropRow - sql request - " << sqlReq;
    if(!query.exec(sqlReq)){
        qDebug() << "UserDataBase: DropRow - " << query.lastError().text();
        m_ndb.close();
        return false;
    }
    else{
        m_ndb.close();
        return true;
    }
}

bool UserDataBase::EditRow(QString tbName, QList<QByteArray> editValues, QList<QByteArray> headerValues, QString idEdit)
{
    if(!m_ndb.open()){
        qDebug() << "UserDataBase: EditRow - Open -" << m_ndb.lastError().text();
        return false;
    }
    if(headerValues.length() == 0){
        qDebug() << "UserDataBase: EditRow - headerValues = 0";
        return false;
    }
    QString sqlReq = QString("UPDATE %1 SET ").arg(tbName);
    sqlReq += QString(headerValues[0] + " = " + editValues[0]);
    for(int i = 1; i < editValues.length(); i++)
        sqlReq += QString(", " + headerValues[i]+ " = " + editValues[i] + " ");
    QString idName = "id";
    for(int i = 0; i < headerValues.length(); i++){
        if(headerValues[i].contains("id")){
            idName = headerValues[i];
            break;
        }
    }
    sqlReq += "WHERE " + idName + " = " + idEdit + ";";
    qDebug() << sqlReq;
    QSqlQuery query(m_ndb);
    if(!query.exec(sqlReq)){
        qDebug() << "UserDataBase: EditRow - " << query.lastError().text();
        m_ndb.close();
        return false;
    }
    return true;
}


bool UserDataBase::DropTable(QString tbName)
{
    if(!m_ndb.open()){
        qDebug() << "UserDataBase: DropTable - Open -" << m_ndb.lastError().text();
        return false;
    }
    QSqlQuery query(m_ndb);
    QString sqlReq = QString("DROP TABLE %1;").arg(tbName);
    if(!query.exec(sqlReq)){
        qDebug() << "UserDataBase: DropTable - " << query.lastError().text();
        m_ndb.close();
        return false;
    }
    else{
        m_ndb.close();
        return true;
    }
}

bool UserDataBase::CreateTable(QString tbName)
{
    if(!m_ndb.open()){
        qDebug() << "UserDataBase: CreateTable - Open -" << m_ndb.lastError().text();
        return false;
    }
    QSqlQuery query(m_ndb);
    QString sqlReq = QString("CREATE TABLE %1();").arg(tbName);
    if(!query.exec(sqlReq)){
        qDebug() << "UserDataBase: CreateTable - " << query.lastError().text();
        m_ndb.close();
        return false;
    }
    else{
        m_ndb.close();
        return true;
    }
}

bool UserDataBase::CreateColumn(QString tbName, QString columnName, QString columnType, bool isPK, bool isNotNull/* = true*/)
{
    if(!m_ndb.open()){
        qDebug() << "UserDataBase: CreateColumn - Open -" << m_ndb.lastError().text();
        return false;
    }
    QSqlQuery query(m_ndb);
    QString sqlReq = QString("CREATE TABLE IF NOT EXIST %1();").arg(tbName);
    if(!query.exec(sqlReq)){
        qDebug() << "UserDataBase: CreateColumn - create table -" << m_ndb.lastError().text();
        m_ndb.close();
        return false;
    }
    QString pk;
    if(isPK)
        pk = "PRIMARY KEY";
    else
        pk = "";
    QString notNull;
    if(isNotNull)
        notNull = "NOT NULL";
    else
        notNull = "";
    sqlReq = QString("ALTER TABLE %1 ADD COLUMN IF NOT EXIST %2 %3 %4 %5;").arg(tbName).arg(columnName).arg(columnType).arg(notNull).arg(pk);
    if(!query.exec(sqlReq)){
        qDebug() << "UserDataBase: CreateColumn - add column -" << m_ndb.lastError().text();
        m_ndb.close();
        return false;
    }
    return true;
}

bool UserDataBase::CreateLink(QString tbName, QString poleTbName, QString fromTbName, QString fromPoleTbName)
{
    if(!m_ndb.open()){
        qDebug() << "UserDataBase: CreateLink - Open -" << m_ndb.lastError().text();
        m_ndb.close();
        return false;
    }
    QSqlQuery query(m_ndb);
    QString sqlReq = QString("ALTER TABLE %1 ALTER COLUMN %2 SET NOT NULL;").arg(tbName).arg(poleTbName);
    bool isExcepion = !query.exec(sqlReq);
    if(isExcepion){
        qDebug() << "UserDataBase: CreateLink - alter set not null tbname" << query.lastError().text();
        m_ndb.close();
        return false;
    }
    sqlReq = QString("ALTER TABLE %1 ALTER COLUMN %2 SET NOT NULL;").arg(fromTbName).arg(fromPoleTbName);
    isExcepion = !query.exec(sqlReq);
    if(isExcepion){
        qDebug() << "UserDataBase: CreateLink - alter set not null fromtbname" << query.lastError().text();
        m_ndb.close();
        return false;
    }
    sqlReq = QString("ALTER TABLE %1 ADD CONSTRAINT FK_%2_%3 FOREIGN KEY (%4) "
                     "REFERENCES %5 (%6) ON DELETE CASCADE;").arg(tbName).arg(tbName).arg(poleTbName).arg(poleTbName).arg(fromTbName).arg(fromPoleTbName);
    isExcepion = !query.exec(sqlReq);
    if(isExcepion){
        qDebug() << "UserDataBase: CreateLink - " << query.lastError().text();
        m_ndb.close();
        return false;
    }
    return true;
}

bool UserDataBase::SetIdGroup(QString userLogin, QString groupId)
{
    if(!m_ndb.open()){
        qDebug() << "UserDataBase: CreateLink - Open -" << m_ndb.lastError().text();
        m_ndb.close();
        return false;
    }
    QSqlQuery query(m_ndb);
    QString sqlReq = QString("UPDATE logusers "
                             "SET groupid = '%1' "
                             "WHERE loginUser = '%2'").arg(groupId).arg(userLogin);
    if(!query.exec(sqlReq)){
        qDebug() << "UserDataBase: SetIdGroup - update -" << query.lastError().text();
        m_ndb.close();
        return false;
    }
    sqlReq = QString("GRANT SELECT ON TABLE public.logroles TO %1_user;").arg(userLogin);
    if(!query.exec(sqlReq)){
        qDebug() << "UserDataBase: Grand logroles -" << query.lastError().text();
        m_ndb.close();
        return false;
    }
    sqlReq = QString("GRANT SELECT ON TABLE public.logusers TO %1_user;").arg(userLogin);
    if(!query.exec(sqlReq)){
        qDebug() << "UserDataBase: Grand logusers -" << query.lastError().text();
        m_ndb.close();
        return false;
    }
    sqlReq = QString("GRANT SELECT ON TABLE public.logusersrols TO %1_user;").arg(userLogin);
    if(!query.exec(sqlReq)){
        qDebug() << "UserDataBase: Grand logusersrols -" << query.lastError().text();
        m_ndb.close();
        return false;
    }
    sqlReq = QString("GRANT ALL ON ALL TABLES IN SCHEMA public TO postgres;");
    if(!query.exec(sqlReq)){
        qDebug() << "UserDataBase: Grand all to postgres  -" << query.lastError().text();
        m_ndb.close();
        return false;
    }
    return true;
}

bool UserDataBase::CreateConnectToDb(QString _hostName, QString _userName, QString _dbName, QString _pass)
{
    //close old conection
    if(m_ndb.isOpen()){
        m_ndb.close();
    }
    //create new connection
    m_ndb = QSqlDatabase::addDatabase("QPSQL", nameConnect);

    m_ndb.setHostName(_hostName);
    m_ndb.setUserName(_userName);
    m_ndb.setDatabaseName(_dbName);
    m_ndb.setPassword(_pass);
    if(!m_ndb.open()){
        qDebug() << m_ndb.lastError().text();
        return false;
    }
    m_ndb.close();
    return true;
}

bool UserDataBase::CreateConnectWithUser(QString userLogin)
{
    //close old conection
    if(m_ndb.contains(nameConnect)){
        m_ndb.close();
    }
    if(!m_secdb.isOpen()){
        m_secdb = QSqlDatabase::addDatabase("QPSQL", nameConnect+"_sec");
        m_secdb.setHostName(HOSTNAME);
        m_secdb.setUserName(USERNAME);
        m_secdb.setDatabaseName(DBNAME);
        m_secdb.setPassword(PASS);
        if(!m_secdb.open()){
            qDebug() << "UserDataBase: CreateConnectWithUser - Open sec db -" << m_secdb.lastError().text();
            return false;
        }
    }
    //get pass code
    QSqlQuery query(m_secdb);
    QString sqlReq = QString("SELECT passworduser FROM logusers WHERE loginuser = '%1';").arg(userLogin);
    if(!query.exec(sqlReq)){
        qDebug() << "UserDataBase: CreateConnectWithUser - select  -" << m_secdb.lastError().text();
        m_secdb.close();
        return false;
    }

    QString pas = "";
    while(query.next()){
        pas = query.value("passworduser").toString();
        qDebug() << "- " << pas;
    }
    qDebug() << pas;
    qDebug() << UserDataBase::CriptStr(pas);
    m_secdb.close();
    //create new connection
    m_ndb = QSqlDatabase::addDatabase("QPSQL", nameConnect);
    m_ndb.setHostName(HOSTNAME);
    m_ndb.setUserName(userLogin+"_user");
    m_ndb.setDatabaseName(userLogin+"_db");
    m_ndb.setPassword(UserDataBase::CriptStr(pas));
    if(!m_ndb.open()){
        qDebug() << "UserDataBase: CreateConnectWithUser - select - " << m_ndb.lastError().text();
        return false;
    }

    m_ndb.close();
    return true;
}

bool UserDataBase::InsertRow(QString tableName, QList<QByteArray> values)
{
    if(!m_ndb.open()){
        qDebug() << "UserDataBase: InsertRow - Open -" << m_ndb.lastError().text();
        return false;
    }
    QString sqlReq = QString("INSERT INTO %1 VALUES(%2").arg(tableName).arg(QString(values.at(0)));
    for(int i = 1; i < values.length(); i++){
        sqlReq += ", " + QString(values.at(i));
    }
    sqlReq += ");";
    qDebug() << sqlReq;
    QSqlQuery query(m_ndb);
    if(!query.exec(sqlReq)){
        qDebug() << "UserDataBase: InsertRow - " << query.lastError().text();
        m_ndb.close();
        return false;
    }
    return true;
}

bool UserDataBase::DropColumn(QString tableName, QString columnName)
{
    if(!m_ndb.open()){
        qDebug() << "UserDataBase: DropColumn - Open -" << m_ndb.lastError().text();
        return false;
    }
    QSqlQuery query(m_ndb);
    QString sqlReq = QString("ALTER TABLE IF EXISTS %1 DROP COLUMN IF EXISTS %2 CASCADE;").arg(tableName).arg(columnName);
    if(!query.exec(sqlReq)){
        qDebug() << "UserDataBase: DropColumn - Drop -" << query.lastError().text();
        m_ndb.close();
        return false;
    }
    m_ndb.close();
    return true;
}

bool UserDataBase::InsertColumn(QString tableName, QString columnName, QString dataType, bool isPK)
{
    if(!m_ndb.open()){
        qDebug() << "UserDataBase: InsertColumn - Open -" << m_ndb.lastError().text();
        return false;
    }
    QSqlQuery query(m_ndb);
    if(!query.exec(QString("CREATE TABLE IF NOT EXISTS %1();").arg(tableName))){
        qDebug() << "UserDataBase: InsertColumn - create table - " << query.lastError().text();
        m_ndb.close();
        return false;
    }
    QString pk = "";
    if(isPK)
        pk = QString("PRIMARY KEY");
    QString sqlReq = QString("ALTER TABLE %1 ADD COLUMN %2 %3 %4;").arg(tableName).arg(columnName).arg(dataType).arg(pk);
    if(!query.exec(sqlReq)){
        qDebug() << "UserDataBase: InsertColumn - " << query.lastError().text();
        m_ndb.close();
        return false;
    }
    m_ndb.close();
    return true;
}

QList<QList<QString>> UserDataBase::getTables()
{
    QString sqlReq = QString("SELECT table_name, column_name, data_type, is_nullable "
                             "FROM information_schema.columns "
                             "WHERE table_schema = 'public';");
    return getRequest(sqlReq);
}


QList<QString> UserDataBase::getDbList(QString idRole, QString groupIdUser)
{
    QList<QString> out;
    if(!m_secdb.isOpen()){
        m_secdb = QSqlDatabase::addDatabase("QPSQL", nameConnect+"_sec");
        m_secdb.setHostName(HOSTNAME);
        m_secdb.setUserName(USERNAME);
        m_secdb.setDatabaseName(DBNAME);
        m_secdb.setPassword(PASS);
        if(!m_secdb.open()){
            qDebug() << "UserDataBase: getDbList - Open sec db -" << m_secdb.lastError().text();
            return out;
        }
    }
    QSqlQuery querySec(m_secdb);
    QString SqlReq;
    if(idRole == CLIENT){
        if(groupIdUser != ""){
            SqlReq = QString("SELECT loginuser "
                             "FROM public.logusersrols, public.logusers "
                             "WHERE logusersrols.iduser = logusers.id "
                             "AND logusersrols.idrole = 1 "
                             "AND logusers.groupid = '%1';").arg(groupIdUser);
            qDebug() << SqlReq;
            if(!querySec.exec(SqlReq)){
                qDebug() << "UserDataBase: getDbList - SELECT -" << querySec.lastError();
                m_secdb.close();
                return out;
            }
            while(querySec.next()){
                out.append(querySec.value("loginuser").toString());
            }
        }
    }
    else if(idRole == ADMIN){
        SqlReq = QString("SELECT loginuser "
                         "FROM logusersrols, logusers "
                         "WHERE logusersrols.iduser = logusers.id;");
        qDebug() << "UserDataBase: sql -" << SqlReq;
        if(!querySec.exec(SqlReq)){
            qDebug() << "UserDataBase: getDbList - SELECT -" << querySec.lastError();
            m_secdb.close();
            return out;
        }
        while(querySec.next()){
            out.append(querySec.value("loginuser").toString());
        }
        qDebug() << "Userdatabase - dbs" << out;
    }
    m_secdb.close();
    return out;
}

int UserDataBase::getIdRoleFromUser(QString userlogin)
{
    int result = -1;
    if(!m_ndb.open()){
        qDebug() << "UserDataBase: getIdRoleFromUser - Open - " << m_ndb.lastError().text();
        return result;
    }
    QSqlQuery query(m_ndb);
    QString sqlReq = QString("SELECT idrole FROM logusersrols WHERE iduser = (SELECT id FROM logusers WHERE loginuser = '%1');")
            .arg(userlogin);
    if(!query.exec(sqlReq)){
        qDebug() << "SQL operation failed!";
        qDebug() << query.lastError();
        m_ndb.close();
        return result;
    }
    while(query.next()){
        result = query.value("idrole").toInt();
        qDebug() << "UserDataBase: id Role - "<< result;
    }
    m_ndb.close();
    return result;
}

bool UserDataBase::CreateDB()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "default_con");
    db.setDatabaseName(DBNAME);
    db.setHostName(HOSTNAME);
    db.setUserName(USERNAME);
    db.setPassword(PASS);
    if(!db.open()){
        qDebug() << "CreateDB - open database failed";
        return false;
    }
    QSqlQuery query(db);
    bool isWorked;
    isWorked = query.exec("CREATE SEQUENCE IF NOT EXISTS id_seq_users "
                          "INCREMENT 1 "
                          "MINVALUE 1 "
                          "MAXVALUE 9223372036854775807 "
                          "START 2 "
                          "CACHE 1;");
    if(!isWorked)
        qDebug() << "Userdatabase: create sequence user:" << query.lastError();

    isWorked = query.exec("CREATE SEQUENCE IF NOT EXISTS id_seq_rols "
                          "INCREMENT 1 "
                          "MINVALUE 1 "
                          "MAXVALUE 9223372036854775807 "
                          "START 2 "
                          "CACHE 1;");
    if(!isWorked)
        qDebug() << "Userdatabase: create sequence user:" << query.lastError();
    isWorked = query.exec("CREATE TABLE IF NOT EXISTS logroles ("
                          "id INT NOT NULL , "
                          "nameRole text, "
                          "CONSTRAINT PK_logroles PRIMARY KEY (id)"
                          ");");
    if(!isWorked)
        qDebug() << "Userdatabase: create tb1:" << query.lastError();

    isWorked = query.exec("CREATE TABLE IF NOT EXISTS logusers ("
                          "id INT NOT NULL , "
                          "loginuser text,"
                          "yearuser text,"
                          "passworduser text,"
                          "groupid text,"
                          "CONSTRAINT PK_logusers PRIMARY KEY (id)"
                          ");");
    if(!isWorked)
        qDebug() << "Userdatabase: create tb2:" << query.lastError();

    isWorked = query.exec("CREATE TABLE IF NOT EXISTS logusersrols ("
                          "idUser INT NOT NULL,"
                          "idRole INT NOT NULL,"
                          "CONSTRAINT PK_logusersrols PRIMARY KEY (idUser, idRole),"
                          "CONSTRAINT FK_logusersrols_idUser FOREIGN KEY (idUser)"
                          "REFERENCES logusers (id) ON DELETE CASCADE,"
                          "CONSTRAINT FK_logusersrols_idRole FOREIGN KEY (idRole)"
                          "REFERENCES logroles (id) ON DELETE CASCADE"
                          ");");
    if(!isWorked)
        qDebug() << "CreateDB: create tb3:" << query.lastError();

    isWorked = query.exec("INSERT INTO logroles(id, namerole) VALUES(0, 'admin'),(1, 'client');");
    if(!isWorked)
        qDebug() << "CreateDB: insert roles:" << query.lastError();

    isWorked = query.exec(QString("INSERT INTO logusers(id, loginuser, yearuser, passworduser, groupid) VALUES(0, 'admin', '%1', '%2', 'admin');").arg(QDate::currentDate().toString()).arg(UserDataBase::CriptStr("admin")));
    if(!isWorked)
        qDebug() << "CreateDB: insert admin:" << query.lastError();

    isWorked = query.exec("INSERT INTO logusersrols(idUser, idRole) VALUES(0, 0);");
    if(!isWorked)
        qDebug() << "CreateDB: insert usersrols:" << query.lastError();

    isWorked = query.exec("ALTER TABLE IF EXISTS logusers ALTER COLUMN id SET DEFAULT nextval('id_seq_users');");
    if(!isWorked)
        qDebug() << "CreateDB: add sequence logusers:" << query.lastError();

    isWorked = query.exec("ALTER TABLE IF EXISTS logroles ALTER COLUMN id SET DEFAULT nextval('id_seq_rols');");
    if(!isWorked)
        qDebug() << "CreateDB: add sequence logroles:" << query.lastError();

    isWorked = query.exec("GRANT ALL ON ALL TABLES IN SCHEMA public TO postgres;");
    if(!isWorked){
        qDebug() << "CreateDB: Grand all to postgres -" << query.lastError().text();
    }
    db.close();
    db.removeDatabase(db.connectionName());
    return isWorked;
}

UserDataBase::UserDataBase(QString _nameConnect, QObject *parent) : QObject(parent)
{
    nameConnect = _nameConnect;
}

UserDataBase::UserDataBase(QObject *parent) : QObject(parent)
{
    nameConnect = "default_db_connect";
}
