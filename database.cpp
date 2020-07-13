#include "database.h"
#include "mainwindow.h"


DataBase::DataBase(QObject *parent) : QObject(parent)
{
}

DataBase::~DataBase()
{
}

void DataBase::connectToDataBase()
{
    qDebug()<<" work connectToDataBase";
//Перед подключением к базе данных производим проверку на её существование.
//В зависимости от результата производим открытие базы данных или её восстановление из копии
    pathDb = QCoreApplication::applicationDirPath() + QDir::separator() + DATABASE_NAME;
    if(!QFile(pathDb).exists()){
        this->restoreDataBase();
    } else {
        this->openDataBase();
    }
}

//Методы восстановления базы данных
bool DataBase::restoreDataBase()
{
    qDebug()<<" work restoreDataBase";
    if(this->openDataBase()){
        if(!this->createTable()){
            return false;
        } else {
            return true;
        }
    } else {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
    return false;
}

//Метод для открытия базы данных
bool DataBase::openDataBase()
{
    qDebug()<<" work openDataBase";
//База данных открывается по заданному пути
//и имени базы данных, если она существует
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName(pathDb);
    if(db.open()){
            qDebug()<<" DATABASE is open";
        return true;
    } else {
        return false;
    }
}

//Метод закрытия базы данных
void DataBase::closeDataBase()
{
    db.close();
}

//Метод для создания таблицы в базе данных
bool DataBase::createTable()
{
    return true;
}

//Метод для вставки записи в базу данных
bool DataBase::inserIntoTable(const QVariantList &data)
{
    QSqlQuery query;
    return true;
}


