#ifndef DATABASE_H
#define DATABASE_H


#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlQuery>
#include <QObject>

class DataBase : QObject
{
    Q_OBJECT
private:
    QSqlDatabase dataBase;

public:
    DataBase(QString _dbName, QString _userName, QString _hostName, QString _password);

};

#endif // DATABASE_H
