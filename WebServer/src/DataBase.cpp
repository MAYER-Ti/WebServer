#include "DataBase.h"



DataBase::DataBase(QString _dbName, QString _userName, QString _hostName, QString _password)
{
    dataBase = QSqlDatabase::addDatabase("QPSQL");
    dataBase.setDatabaseName(_dbName);
    dataBase.setUserName(_userName);
    dataBase.setHostName(_hostName);
    dataBase.setPassword(_password);
}
