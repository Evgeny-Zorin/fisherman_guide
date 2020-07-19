#include "messegecontroller.h"

MessegeController::MessegeController(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
        connect(socket,SIGNAL(readyRead()),this,SLOT(sockReady()));
        connect(socket,SIGNAL(disconnected()),this,SLOT(sockDick()));
        socket->connectToHost("127.0.0.1",7777);
}

void MessegeController::sockReady()
{
    if(socket->waitForConnected(250)){
        socket->waitForConnected(250);
        dataChat = socket->readAll();
        //qDebug() << data;
        docChat = QJsonDocument::fromJson(dataChat, &docChatError);
        if (docChatError.errorString().toInt() == QJsonParseError::NoError){
            if((docChat.object().value("type").toString() == "connect") &&
                (docChat.object().value("status").toString() == "True")){
                //QMessageBox::information(this, tr("Information"), tr("Connection completed."));
                qDebug()<< tr("Connection completed.");
            }
            else if (docChat.object().value("type").toString() == "resultSelect"){
                QStandardItemModel* model = new QStandardItemModel(nullptr);
                model->setHorizontalHeaderLabels(QStringList() << "time");
                QJsonArray docArr = docChat.object().value("result").toArray();
                for(int i = 0; i < docArr.count(); i++){
                    QStandardItem* col = new QStandardItem(docArr[i].toObject().value("time").toString());
                    model->appendRow(col);
                }
                tblChat->setModel(model);
            }
            else{
                qDebug()<< tr("Fail connection.");
                //QMessageBox::information(this, tr("Information"), tr("Fail connection."));
            }
        }
        else{
                qDebug()<< tr("Error transmission of data format.\n");
             //QMessageBox::information(this, tr("Information"), tr("Error transmission of data format.\n") + docChatError.errorString());
        }

    }
}

void MessegeController::sockDick()
{
    socket->deleteLater();
}
