#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QFile>
#include <fstream>
#include <QBitArray>
#include <QDockWidget>
#include <QWebEngineView>
#include <QMessageBox>
#include <QScriptEngine>
#include <QScriptContext>
#include <QScriptValue>
#include <QModelIndex>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    list = new QStringList;

    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));          //go to tray
    qDebug() << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();

    db = new DataBase();
    db->connectToDataBase();

//Инициализируем модель для представления данных с заданием названий колонок
    this->setupModelDb(TABLE,
                     QStringList() << "1"
                                   << "2"
                                   << "3"
               );
//Инициализируем внешний вид таблицы с данными
    this->createTableViewUi();

     m_view = new QWebEngineView(this);
     QWebEnginePage *page = m_view->page();
     ui->preview->setPage(page);    //добавим QWebEngineView в доксистему

     connect(page, &QWebEnginePage::featurePermissionRequested,
             [this, page](const QUrl &securityOrigin, QWebEnginePage::Feature feature)
     {
//Этот сигнал испускается всякий раз, когда веб-страница запрашивает использование
//определенной функции или устройства, включая не только службы определения местоположения,
//но и устройства захвата звука или блокировки мыши, например.
//В этом примере мы обрабатываем только запросы на службы определения местоположения:
         if (feature != QWebEnginePage::Geolocation)
             return;

         QMessageBox msgBox(this);
         msgBox.setText(tr("%1 wants to know your location").arg(securityOrigin.host()));
         msgBox.setInformativeText(tr("Do you want to send your current location to this website?"));
         msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
         msgBox.setDefaultButton(QMessageBox::Yes);

         if (msgBox.exec() == QMessageBox::Yes) {
             page->setFeaturePermission(
                 securityOrigin, feature, QWebEnginePage::PermissionGrantedByUser);
         } else {
             page->setFeaturePermission(
                 securityOrigin, feature, QWebEnginePage::PermissionDeniedByUser);
         }
     });

        page->load(QUrl(QStringLiteral("qrc:/resource/js_yaMap/html/mapbasics.html")));

        webobj = new WebClass();
        channel = new QWebChannel(this);
        channel->registerObject("qProxy", webobj);
        page->setWebChannel(channel);

//        completer = new completerID(this);
//        completer->mysetModel(model);
//        ui->lbl_City->setCompleter(completer->getCompl());
        QCompleter* completer = new QCompleter( this );
        completer->setModel(model);
        completer->setCompletionColumn(1);
        completer->setCaseSensitivity( Qt::CaseInsensitive );
        ui->FindFish->setCompleter(completer);
}

MainWindow::~MainWindow()
{
    //m_view->~QWebEngineView();
    db->closeDataBase();
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_search_city_clicked()
{
//обработчик запроса погоды по названию города
    QNetworkAccessManager* manager = new QNetworkAccessManager(0);
    //Подключаем networkManager к обработчику ответа
    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::onResult);
    QString nameCity = ui->lbl_City->text();    //to take the name CITY
    ui->lbl_City->clear();                      //clear the input line

    QString urlwithCity = "http://api.openweathermap.org/data/2.5/weather?q="
            + nameCity
            +"&appid=f32fcd94d9aad60903d7702471434295";
    QUrl url(urlwithCity);
    QNetworkRequest request(url);
    //request.setRawHeader(QByteArray("APPID"),QByteArray("f32fcd94d9aad60903d7702471434295"));

    manager->get(request);  //Получаем данные, JSON файл с сайта по определённому url
}

void MainWindow::onResult(QNetworkReply *reply)
{
    // Если ошибки отсутсвуют
    if(!reply->error()){

        // То создаём объект Json Document, считав в него все данные из ответа
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

        // Забираем из документа корневой объект
        QJsonObject root = document.object();
        /* Находим объект "departament", который располагается самым первым в корневом объекте.
         * С помощью метода keys() получаем список всех объектов и по первому индексу
         * забираем название объекта, по которому получим его значение
         * */
//          ui->textEdit->append(QString::number(root.value("timezone").toInt()));
//          ui->textEdit->append(root.value("name").toString());
//          ui->textEdit->append(QString::number(root.value("temp").toInt()));
            QJsonValue jv = root.value("main");

           qDebug() << jv;
            //то забираем массив из данного свойства
             QJsonArray ja = jv.toArray();
             qDebug() << ja;
             // Перебирая все элементы массива
             for(int i = 0; i < ja.count(); i++){
                 QJsonObject subtree = ja.at(i).toObject();
            qDebug() << "value:" << subtree.value("temp").toString();
             }


    }
    reply->deleteLater();


//    qDebug() << reply->error();
//    QByteArray bytes = reply->readAll();
//    QJsonDocument jsonDocument(QJsonDocument::fromJson(bytes));
//    QString saveFileName = QFileDialog::getSaveFileName(this,
//                                                        tr("Save Json File"),
//                                                        QString(),
//                                                        tr("JSON (*.json)"));
//    QFile jsonFile(saveFileName);
//    if (!jsonFile.open(QIODevice::WriteOnly))
//    {
//        return;
//    }

//    // Write the current Json object to a file.
//    jsonFile.write(QJsonDocument(jsonDocument).toJson(QJsonDocument::Indented));
//    jsonFile.close();   // Close file
}

//QJsonValue findKey(const QString& key, const QJsonValue& value) {
//    if (value.isObject()) {
//        const QJsonObject obj = value.toObject();
//        if (obj.contains(key))
//            return obj.value(key);           // return 'early' if object contains key

//        for (const auto& value : obj) {
//            QJsonValue recurse = findKey(key, value);  // call itself, forwarding a value
//            if (!recurse.isNull())
//                return recurse;              // value found, return 'early'
//        }
//    } else if (value.isArray()) {
//        for (const auto& value : value.toArray()) {
//            QJsonValue recurse = findKey(key, value);
//            if (!recurse.isNull())
//                return recurse;
//        }
//    }

//    return QJsonValue();          // base case: a null value
//}

void MainWindow::on_DockMapBtn_clicked()
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(0);
    //Подключаем networkManager к обработчику ответа
    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::onResult);
                                        //57.264586, 44.532377
    QString nameCity = ui->lbl_City->text();    //to take the name CITY
    ui->lbl_City->clear();                      //clear the input line

    QString lat, lon;
    lat = QString::number(webobj->getLat());
    lon =QString::number(webobj->getLon());

    QString urlwithCity = "http://api.openweathermap.org/data/2.5/weather?"
            "lat=" + lat +
            "&lon=" + lon +
            "&units=metric" +
            "&appid=f32fcd94d9aad60903d7702471434295";

    //QString urlwithCity = "http://api.openweathermap.org/data/2.5/weather?q=" + nameCity + "&appid=f32fcd94d9aad60903d7702471434295";
    QUrl url(urlwithCity);
    QNetworkRequest request(url);
    //request.setRawHeader(QByteArray("APPID"),QByteArray("f32fcd94d9aad60903d7702471434295"));

    manager->get(request);  //Получаем данные, JSON файл с сайта по определённому url
}

void MainWindow::changeForecast()
{

}
//Метод для инициализации модели представления данных
void MainWindow::setupModelDb(const QString &tableName, const QStringList &headers)
{
    qDebug()<<" start setupModelDb";
//Производим инициализацию модели представления данных
//с установкой имени таблицы в базе данных, по которому
//будет производится обращение в таблице
    model = new QSqlTableModel(this);
    model->setTable(tableName);
//    qDebug()<<"start thread::sleep ";
//    std::this_thread::sleep_for (std::chrono::seconds(12));
//    qDebug()<<"stop thread::sleep ";
    if (model->lastError().type() != QSqlError::NoError)
    {
        QMessageBox msgBox(this);
        msgBox.setText(model->lastError().text());
        //emit someEmit(model->lastError().text());
    }

//Устанавливаем названия колонок в таблице с сортировкой данных
//        for(int i = 0, j = 0; i < model->columnCount()+1; i++, j++){
//            model->setHeaderData(i,Qt::Horizontal,headers[j]);
//        }
//Устанавливаем сортировку по возрастанию данных по нулевой колонке
    model->setSort(0,Qt::AscendingOrder);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    qDebug()<<" start setupModelDb  CLOSE";
}

void MainWindow::createTableViewUi()
{
    qDebug()<<" start createTableViewUi";
    ui->tableViewBd->setModel(model);     // Устанавливаем модель на TableView
    ui->tableViewBd->setColumnHidden(0, true);    // Скрываем колонку с id записей
    ui->tableViewBd->setColumnHidden(3, true);
    ui->tableViewBd->setColumnHidden(4, true);
    ui->tableViewBd->setColumnHidden(5, true);
    ui->tableViewBd->setColumnHidden(6, true);
    ui->tableViewBd->setColumnHidden(7, true);
    ui->tableViewBd->setColumnHidden(8, true);
    ui->tableViewBd->setColumnHidden(9, true);
// Разрешаем выделение строк
    ui->tableViewBd->setSelectionBehavior(QAbstractItemView::SelectRows);
// Устанавливаем режим выделения лишь одно строки в таблице
    ui->tableViewBd->setSelectionMode(QAbstractItemView::SingleSelection);
// Устанавливаем размер колонок по содержимому
    ui->tableViewBd->resizeColumnsToContents();
    ui->tableViewBd->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->tableViewBd->horizontalHeader()->setStretchLastSection(true);
    ui->tableViewBd->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    model->select(); // Делаем выборку данных из таблицы
    //model->setData(model->index(1,1),"3",Qt::EditRole);
    //model->submitAll();

}

void MainWindow::onAddWordCompleter()
{
    const QString text = ui->lbl_City->text().trimmed();
    if( text.isEmpty() ) {
        return;
    }
    //model->setFilter(text).isEmpty()
//    if( !ui->listWidget->findItems( text, Qt::MatchFixedString ).isEmpty() ) {
//        return;
//    }
//    model->setFilter(text);
    ui->lbl_City->setText(text);
    ui->lbl_City->clear();
}
