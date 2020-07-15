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
                     QStringList() << "id"
                                   << "Дата"
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
}

MainWindow::~MainWindow()
{
    //m_view->~QWebEngineView();
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionOpen_DBase_triggered()
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

void MainWindow::on_pushButton_clicked()
{
    pathDB = QFileDialog::getOpenFileName(this, "Open file", "", "");
    dataBase = QSqlDatabase::addDatabase("QSQLITE");
    dataBase.setDatabaseName(pathDB);
    if(dataBase.open()) {
        qDebug()<<"start read DB";
        sqlQuery = QSqlQuery(dataBase);
        fillingData();
        refreshList();
    }
    else qDebug()<<"error open DB";
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

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    QRegExp regExp(arg1, Qt::CaseInsensitive, QRegExp::Wildcard);
    ui->listWidget->clear();
    ui->listWidget->addItems(list->filter(regExp));
}

void MainWindow::on_search_city_clicked()
{
//обработчик запроса погоды по ID города
}

void MainWindow::onResult(QNetworkReply *reply)
{
    qDebug() << reply->error();
    QByteArray bytes = reply->readAll();
//       // QString string = reply->readAll();
//               qDebug() << "reply == huntReplay: " << bytes;

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
}

void MainWindow::on_DockMapBtn_clicked()
{
    QNetworkAccessManager* manager = new QNetworkAccessManager(0);
    //Подключаем networkManager к обработчику ответа
    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::onResult);
                                        //57.264586, 44.532377
    //api.openweathermap.org/data/2.5/weather?q={city name}&appid={your api key}
    QString nameCity = ui->lbl_City->text();    //to take the name CITY
    ui->lbl_City->clear();                      //clear the input line

    QString lat, lon;
    lat = QString::number(webobj->getLat());
    lon =QString::number(webobj->getLon());

    QString urlwithCity = "http://api.openweathermap.org/data/2.5/weather?"
            "lat=" + lat +
            "&lon=" + lon +
            "&appid=f32fcd94d9aad60903d7702471434295";

    //QString urlwithCity = "http://api.openweathermap.org/data/2.5/weather?q=" + nameCity + "&appid=f32fcd94d9aad60903d7702471434295";
    QUrl url(urlwithCity);
    QNetworkRequest request(url);
    //request.setRawHeader(QByteArray("APPID"),QByteArray("f32fcd94d9aad60903d7702471434295"));

    manager->get(request);  //Получаем данные, JSON файл с сайта по определённому url
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
    if (model->lastError().type() != QSqlError::NoError)
    {
        QMessageBox msgBox(this);
        msgBox.setText(model->lastError().text());
        //emit someEmit(model->lastError().text());
    }

//Устанавливаем названия колонок в таблице с сортировкой данных
//        for(int i = 0, j = 0; i < model->columnCount(); i++, j++){
//            model->setHeaderData(i,Qt::Horizontal,headers[j]);
//        }
//Устанавливаем сортировку по возрастанию данных по нулевой колонке
    model->setSort(0,Qt::AscendingOrder);
}

void MainWindow::createTableViewUi()
{
//    qDebug()<<" start createTableViewUi";
    ui->tableViewBd->setModel(model);     // Устанавливаем модель на TableView
    ui->tableViewBd->setColumnHidden(0, true);    // Скрываем колонку с id записей
// Разрешаем выделение строк
    ui->tableViewBd->setSelectionBehavior(QAbstractItemView::SelectRows);
// Устанавливаем режим выделения лишь одно строки в таблице
    ui->tableViewBd->setSelectionMode(QAbstractItemView::SingleSelection);
// Устанавливаем размер колонок по содержимому
    ui->tableViewBd->resizeColumnsToContents();
    ui->tableViewBd->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewBd->horizontalHeader()->setStretchLastSection(true);
    model->select(); // Делаем выборку данных из таблицы
}
