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
#include <QEvent>
#include <QCloseEvent>
//#include <QTreeView>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pathApp = QCoreApplication::applicationDirPath();
    //this->setWindowIcon("qrc:/resource/img/img1.png");
    qDebug() << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();

    db = new DataBase();
    db->connectToDataBase();

    jparserObj = new jsonparser();  //выделение памяти под объект-парсер
    weatherObj = new weather();     //выделение памяти под объект-погода
    forecastObj = new forecast();   //выделение памяти под объект-анализатор

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
     //ui->preview->set
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
        m_view->hide();

        //~~~~~~~~
        trayIcon = new QSystemTrayIcon(this);                                       //icon initialization
        trayIcon->setIcon(this->style()->standardIcon(QStyle::SP_ComputerIcon));    //selected a standard image
        trayIcon->setToolTip(tr("Program works in the system tray."));              //Работа со сворачиванием программы трей
        QAction * viewWindow = new QAction(tr("Maximize the window"), this);        //action Expand the window
        connect(viewWindow, SIGNAL(triggered()), this, SLOT(show()));               //return from the tray
        //ui->menuSettings->addAction(viewWindow);

        trayIcon->show();       //showing the app icon in the tray
        connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));      //connecting the icon click signal to the handler
        connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));          //go to tray

        //tblChat = new QTableView();
        tblChat = ui->tblChat;  //получаю указатель на таблицу чата
        //lnChat = new QLineEdit();
        lnChat = ui->lineMessege;

        MsgController = new MessegeController();
        //MsgController->socket->connectToHost("127.0.0.1",7777);
        //передаю в класс контроллер указатели для работы с UI
        MsgController->setPtrLineChat(lnChat);
        MsgController->setPtrTableChat(tblChat);
        readSettings();
}

MainWindow::~MainWindow()
{
    //m_view->~QWebEngineView();
    db->closeDataBase();
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug()<<"MainWindow::closeEvent ";
    //If the window is visible and the checkbox is checked, then the application isn't terminated
    //the window is simply hidden, which is accompanied by
    //corresponding pop-up message
    if(this->isVisible() && ui->actionTrayIcon->isChecked())
    {
        qDebug()<<"isVisible";
        event->setAccepted(false);  //disable closing of the window
        this->hide();               //to hide the window
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);

        trayIcon->showMessage("MyClockWindow",
                              tr("The app is minimized to the tray. "
                                 "To expand the app window, click the app icon in the tray!"),
                              icon,
                              300);
    }
   else
    {
       writeSettings();
        }
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
    else
    QMainWindow::changeEvent(event);
}

void MainWindow::changeTranslator(QString postfix)
{
    QApplication::removeTranslator(translator); //disabling the current translator
    translator = new QTranslator(this);
    translator->load(QApplication::applicationName() + "_" + postfix); //uploading a new dictionary
    QApplication::installTranslator(translator);    //connecting a translator with a new dictionary
    //ui->statusBar->showMessage(tr("Current Language changed to %1").arg(postfix));
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
            + nameCity +
            "&units=metric" +
            +"&appid=f32fcd94d9aad60903d7702471434295";
    QUrl url(urlwithCity);
    QNetworkRequest request(url);
    //request.setRawHeader(QByteArray("APPID"),QByteArray("f32fcd94d9aad60903d7702471434295"));

    manager->get(request);  //Получаем данные, JSON файл с сайта по определённому url
}

void MainWindow::onResult(QNetworkReply *reply)
{
    //jparserObj->saveToDisk(this, reply);
    if (jparserObj->parsWeather(reply, weatherObj)){
        forecastObj->makeanalysis(weatherObj, model);
        ui->lbl_cityHead->setText(weatherObj->getnameCity());
        ui->lblTempValue->setText(QString::number(weatherObj->getmainTemp()) + " С");
        ui->lblHumidityValue->setText(QString::number(weatherObj->getmainHumidity()) + " %");
        //ui->lblPressureValue->setText(QString::number(weatherObj->getmainGrnd_level()));
        ui->lblCloudinessValue->setText(weatherObj->getweatherMain());
        ui->lblCountry->setText(weatherObj->getsysCountry());
    }
}

void MainWindow::on_DockMapBtn_clicked()
{
    //qDebug() <<"on_DockMapBtn_clicked";
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
    //qDebug()<<" start setupModelDb";
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
    model->setSort(1,Qt::AscendingOrder);   //сортируем модель по названием рыб(по алфавиту)
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //qDebug()<<" start setupModelDb  CLOSE";
}

void MainWindow::createTableViewUi()
{
    //qDebug()<<" start createTableViewUi";
    ui->tableViewBd->setModel(model);             // Устанавливаем модель на TableView
    ui->tableViewBd->setColumnHidden(0, true);    // Скрываем колонки с лишними записями
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
    //model->submitAll();
}

void MainWindow::writeSettings()
{
    //qDebug()<<"writeSettings";
    QSettings settings(pathApp + QDir::separator() + "settins.ini", QSettings::IniFormat);
    settings.beginGroup("MyClockWindow");
    settings.setValue("size", size());
    settings.setValue("state", saveState());
    settings.endGroup();
}

void MainWindow::readSettings()
{
    //qDebug()<<"readSettings";
    QSettings settings(pathApp + QDir::separator() + "settins.ini", QSettings::IniFormat);
    settings.beginGroup("MyClockWindow");
    resize(settings.value("size").toSize());
    restoreState(settings.value("state").toByteArray());
    settings.endGroup();
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

void MainWindow::on_lbl_City_returnPressed()
{
    on_search_city_clicked();
}

void MainWindow::on_actionEn_English_triggered()
{
    changeTranslator("en_EN");
}

void MainWindow::on_actionRu_Russian_triggered()
{
    changeTranslator("ru_RU");
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    qDebug()<<"iconActivated";
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:      //The event is ignored if the checkbox is not selected
        if(ui->actionTrayIcon->isChecked())
        {
            if(!this->isVisible()){
                this->show();           //if hidden, it opens to the screen
            } else {
                this->hide();           //if the window is visible, it is hidden,
            }
        }
        break;
    default:
        break;
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, tr("About"), tr("FisherMan Guide\n"
                                                   "by Evgeny Zorin\n"
                                                   "2020"));
}

void MainWindow::on_btnPull_clicked()
{
    if(MsgController->socket->isOpen()){
        MsgController->socket->write("{\"type\":\"select\",\"params\":\"TestFish\"}");
        MsgController->socket->waitForBytesWritten(250);
    }
    else{
        QMessageBox::information(this, tr("Information"), tr("Fail connection."));
    }
}

void MainWindow::on_btbSend_clicked()
{

}

void MainWindow::on_btnClear_clicked()
{

}
