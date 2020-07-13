#ifndef DATABASE_H
#define DATABASE_H
#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>


/* Директивы имен таблицы, полей таблицы и базы данных */
#define DATABASE_HOSTNAME   "Fish_Db"
#define DATABASE_NAME       "Fish_Db.db"

#define TABLE                   "TableExample"
#define TABLE_DATE              "Date"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);
     ~DataBase();

    void connectToDataBase();       //Подключение к базе данных
    bool inserIntoTable(const QVariantList &data);  //вставка записей в таблицу

private:
    QSqlDatabase db;    //объект базы данных
    QString pathDb;     //путь к бд

private:
    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createTable();



};

#endif // DATABASE_H
