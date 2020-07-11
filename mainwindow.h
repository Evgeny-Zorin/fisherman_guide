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
#include "database.h"

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

    void on_pushButton_clicked();
    void on_comboBoxTables_activated(int index);
    void on_comboBoxColumn_activated(int index);
    void on_lineEdit_textChanged(const QString &arg1);

    // Обработчик данных полученных от объекта QNetworkAccessManager
    void onResult(QNetworkReply *reply);//будет разбираться JSON файл при получении ответа от сайта с содержимым файла.
    void on_actionOpen_DBase_triggered();
    void on_actionExit_triggered();
    void on_search_city_clicked();


    void on_DockMapBtn_clicked();

private:
    Ui::MainWindow *ui;

    void fillingData();
    void addTables();
    void makeRequest();
    void addColumns();
    void addValue(int index);
    void refreshList();

    QNetworkAccessManager *networkManager;

private:
    QWebEngineView *m_view;
};
#endif // MAINWINDOW_H
