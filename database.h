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
#include <QMessageBox>


//Директивы имен таблицы, полей таблицы и базы данных
#define DATABASE_HOSTNAME   "FishGuadeDB"
#define DATABASE_NAME       "FishGuadeDB.db"

#define TABLE                   "Fish"  //Fish
#define TABLE_DATE              "Date"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);
     ~DataBase();

    void connectToDataBase();       //Подключение к базе данных
    void closeDataBase();
    bool inserIntoTable(const QVariantList &data);  //вставка записей в таблицу

private:
    QSqlDatabase db;    //объект базы данных
    QString pathDb;     //путь к бд

private:
    bool openDataBase();
    bool restoreDataBase();
    bool backupTableOpen();



};

#endif // DATABASE_H
