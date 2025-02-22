#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

class DataBaseManager : public QObject
{
    Q_OBJECT
public:
    static DataBaseManager& instance();
    ~DataBaseManager();
    bool openDataBase(const QString& path);
    void closeDataBase();
    QString getDataBasePath() const;
    void setDataBase(const QString& path);

private:
    explicit DataBaseManager(QObject *parent = nullptr);
    QSqlDatabase db;
    QString dbPath = "D:/temp/QTDemo/StudentManagerSystem/sqlite/studentInfo.db";
};

#endif // DATABASEMANAGER_H
