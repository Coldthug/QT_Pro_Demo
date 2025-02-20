#include "databasemanager.h"



DataBaseManager &DataBaseManager::instance()
{
    static DataBaseManager instance;
    return instance;
}

DataBaseManager::~DataBaseManager()
{
    closeDataBase();
}

bool DataBaseManager::openDataBase(const QString &path)
{
    db.setDatabaseName(path);
    if(!db.isOpen()){
        qDebug()<<"fail to open database:" << db.lastError().text();
        return false;
    }
    return true;
}

void DataBaseManager::closeDataBase()
{
    if(db.isOpen()){
        db.close();
    }
}

QString DataBaseManager::getDataBasePath() const
{
    return dbPath;
}

void DataBaseManager::setDataBase(const QString &path)
{
    if(path!=dbPath){
        dbPath = path;
        closeDataBase();
        openDataBase(path);
    }
}

DataBaseManager::DataBaseManager(QObject *parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");

}
