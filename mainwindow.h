#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QSqlQuery>
#include <QFileDialog>
#include <QRegExp>
#include <QWidget>
#include <QNetworkAccessManager>
#include <QWebEngineView>
#include <QSqlTableModel>
#include <QCompleter>
#include "database.h"
#include "webclass.h"
//#include "completerID.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QSqlDatabase dataBase;
    QSqlQuery sqlQuery;
    QString pathDB;
    QStringList* list;
private:
    QString AppID;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Обработчик данных полученных от объекта QNetworkAccessManager
    void onResult(QNetworkReply *reply);//будет разбираться JSON файл при получении ответа от сайта с содержимым файла.
    void on_actionExit_triggered();
    void on_search_city_clicked();

    void on_DockMapBtn_clicked();
    void changeForecast();  //обновляет прогноз клева

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *networkManager;

private:
    QWebEngineView *m_view;
private:
    DataBase        *db;       //объект для взаимодействия с бд
    QSqlTableModel  *model;    //модель представления таблицы базы данных
private:
    void setupModelDb(const QString &tableName, const QStringList &headers);
    void createTableViewUi();    //формирует вид TableView
private:
    WebClass *webobj;
    QWebChannel *channel;
private:
    //completerID *completer;
private slots:
    void onAddWordCompleter();
};
#endif // MAINWINDOW_H
