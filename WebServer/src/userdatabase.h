#ifndef USERDATABASE_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QList>
#include <QTime>

#define USERDATABASE_H

#define KEY_TO_PASS 1238973
#define HOSTNAME "localhost"
#define USERNAME "postgres"
#define DBNAME "postgres"
#define PASS "12345678"

class UserDataBase : public QObject
{
    Q_OBJECT
private:
    QSqlDatabase m_ndb;
    QString nameConnect;
    QSqlDatabase m_secdb;

public:
    UserDataBase(QString _nameConnect, QObject *parent = nullptr);
    UserDataBase(QObject *parent = nullptr);

    static QString encodeStr(QString str, quint32 key = KEY_TO_PASS);
    static QString decodeStr(QString str, quint32 key = KEY_TO_PASS);

    QList<QList<QString>> getRequest(QString sqlReq);


    void setNameConnect(QString _nameConnect){ nameConnect = _nameConnect; }
    QString getNameConnect(){ return nameConnect; }
    bool IsOpen(){ return m_ndb.isOpen(); }
    void CloseDb(){m_ndb.close(); }
    bool OpenDb(){ return m_ndb.open(); }
    bool IsConnect(){ return (!m_ndb.hostName().isEmpty() &&
                              !m_ndb.userName().isEmpty() &&
                              !m_ndb.databaseName().isEmpty() &&
                               m_ndb.contains(m_ndb.connectionName())); }

    void CloseConnect(){ m_ndb.removeDatabase(m_ndb.connectionName()); }
    bool DropRow(QString tbName, QString idRow);
    bool EditRow(QString tbName, QList<QByteArray> editValues, QList<QByteArray> headerValues, QString idEdit);
    bool DropTable(QString tbName);
    bool CreateTable(QString tbName);
    bool CreateColumn(QString tbName, QString columnName, QString columnType, bool isPK, bool isNotNull = true);
    bool CreateLink(QString tbName, QString poleTbName, QString fromTbName, QString fromPoleTbName);
    bool SetIdGroup(QString userLogin, QString groupId);
    bool CreateConnectToDb(QString _hostName = HOSTNAME, QString _userName = USERNAME, QString _dbName = DBNAME, QString _pass = PASS);
    bool CreateConnectWithUser(QString userLogin);
    bool InsertRow(QString tableName, QList<QByteArray> values);
    bool DropColumn(QString tableName, QString columnName);
    bool InsertColumn(QString tableName, QString columnName, QString dataType, bool isPK);
    QList<QList<QString>> getTables();
    QList<QString> getDbList(QString idRole, QString groupIdUser = "");
    int getIdRoleFromUser(QString userlogin);
    bool CreateDB();
    bool isUser(QString userLogin, QString userPass);
    bool InputNewUser(QString userLogin, QString userYear, QString userPass, QString groupIdText = "NULL");
    bool DropUser(QString userLogin, QString codeUserPass);

};

#endif // USERDATABASE_H
