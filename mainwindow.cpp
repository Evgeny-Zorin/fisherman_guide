#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <fstream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    list = new QStringList;

    QNetworkAccessManager* manager = new QNetworkAccessManager(0);
    //Подключаем networkManager к обработчику ответа
    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::onResult);
                                        //57.264586, 44.532377
    QUrl url("http://api.openweathermap.org/data/2.5/weather?q=Moscow,ru&");
    QNetworkRequest request(url);
    request.setRawHeader(QByteArray("APPID"),QByteArray(""));


    manager->get(request);  //Получаем данные, а именно JSON файл с сайта по определённому url


qDebug() << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();
}
//    dataBase = QSqlDatabase::addDatabase("QSQLITE");
//    dataBase.setDatabaseName("users.db");

//    if(dataBase.open()){
//        sqlQuery = QSqlQuery(dataBase);
//        sqlQuery.exec("SELECT * FROM users");
//        for(const QString& tableName: dataBase.tables())
//            qDebug()<<tableName;
//        int count = sqlQuery.record().count();
//        for(int i = 0; i < count; i++)
//            qDebug()<<sqlQuery.record().field(i).name();
//        while(sqlQuery.next())
//            qDebug()<<sqlQuery.value(1).toString();
//    }


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    pathDB = QFileDialog::getOpenFileName(this, "Open file", "", "*db");
    dataBase = QSqlDatabase::addDatabase("QSQLITE");
    dataBase.setDatabaseName(pathDB);
    if(dataBase.open()) {
        sqlQuery = QSqlQuery(dataBase);
        fillingData();
        refreshList();
    }
}

void MainWindow::fillingData()
{
    addTables();
    makeRequest();
    addColumns();
    addValue(0);
}

//добавляем доступные таблицы
void MainWindow::addTables()
{
    ui->comboBoxTables->clear();
    for(const QString &tableName: dataBase.tables())
        ui->comboBoxTables->addItem(tableName);
}

//запрос на выбранную пользователем таблицу
void MainWindow::makeRequest()
{
    QString tableName = ui->comboBoxTables->currentText();
    sqlQuery.exec("SELECT * FROM " + tableName + "");
}

//добавляем строки из выбранной таблицы
void MainWindow::addColumns()
{
    ui->comboBoxColumn->clear();
    int columnCount = sqlQuery.record().count();
    for(int i = 0; i < columnCount; i++)
        ui->comboBoxColumn->addItem(sqlQuery.record().field(i).name());
}

//добавляем значения выбранного столбца
void MainWindow::addValue(int index)
{
    ui->listWidget->clear();
    while(sqlQuery.next())
        ui->listWidget->addItem(sqlQuery.value(index).toString());
}

void MainWindow::refreshList()
{
    list->clear();
    for(int i = 0; i < ui->listWidget->count(); i++)
        list->append(ui->listWidget->item(i)->text());
}

void MainWindow::on_comboBoxTables_activated(int index)
{
    makeRequest();
    addColumns();
    addValue(0);
    refreshList();
}

void MainWindow::on_comboBoxColumn_activated(int index)
{
    makeRequest();
    addColumns();
    addValue(index);
    refreshList();
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    QRegExp regExp(arg1, Qt::CaseInsensitive, QRegExp::Wildcard);
    ui->listWidget->clear();
    ui->listWidget->addItems(list->filter(regExp));
}

void MainWindow::onResult(QNetworkReply *reply)
{
    qDebug() << reply->error();
    QByteArray bytes = reply->readAll();
    QJsonDocument jsonDocument(QJsonDocument::fromJson(bytes));
    QString saveFileName = QFileDialog::getSaveFileName(this,
                                                        tr("Save Json File"),
                                                        QString(),
                                                        tr("JSON (*.json)"));
    QFile jsonFile(saveFileName);
    if (!jsonFile.open(QIODevice::WriteOnly))
    {
        return;
    }

    // Write the current Json object to a file.
    jsonFile.write(QJsonDocument(jsonDocument).toJson(QJsonDocument::Indented));
    jsonFile.close();   // Close file

//    qDebug() << reply->error();
//   // QByteArray bytes = reply->readAll();
//    QString string = reply->readAll();
//            //qDebug() << "reply == huntReplay: " << bytes;
//            //std::string nameFile  = "Json.json";
//            QFile file("out.txt");
//            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
//                return;

//            QTextStream out(&file);
//            out << "The magic number is: " << string;


//    // Если ошибки отсутствуют
//    if(!reply->error()){

//        // То создаём объект Json Document, считав в него все данные из ответа
//        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

//        // Забираем из документа корневой объект
//        QJsonObject root = document.object();
//        /* Находим объект "departament", который располагается самым первым в корневом объекте.
//         * С помощью метода keys() получаем список всех объектов и по первому индексу
//         * забираем название объекта, по которому получим его значение
//         * */
//        ui->textEdit->append(root.keys().at(0) + ": " + root.value(root.keys().at(0)).toString());

//        // Второе значение пропишем строкой
//        QJsonValue jv = root.value("employees");
//        // Если значение является массивом, ...
//        if(jv.isArray()){
//            // ... то забираем массив из данного свойства
//            QJsonArray ja = jv.toArray();
//            // Перебирая все элементы массива ...
//            for(int i = 0; i < ja.count(); i++){
//                QJsonObject subtree = ja.at(i).toObject();
//                // Забираем значения свойств имени и фамилии добавляя их в textEdit
//                ui->textEdit->append(subtree.value("firstName").toString() +
//                                     " " +
//                                     subtree.value("lastName").toString());
//            }
//        }
//        // В конце забираем свойство количества сотрудников отдела и также выводим в textEdit
//        ui->textEdit->append(QString::number(root.value("number").toInt()));
//    }
//    reply->deleteLater();
}
