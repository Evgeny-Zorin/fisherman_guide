#ifndef MESSEGECONTROLLER_H
#define MESSEGECONTROLLER_H

#include <QObject>
#include <QTcpSocket>
#include <QMessageBox>
#include <QByteArray>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTableView>
#include <QLineEdit>

class MessegeController : public QObject
{
    Q_OBJECT
public:
    explicit MessegeController(QObject *parent = nullptr);
public:
    QTcpSocket* socket; //переменная отвечает за передачу информации
    QByteArray dataChat;


    QJsonDocument docChat;
    QJsonParseError docChatError;
public slots:
    void sockReady();
    void sockDick();
private:
    QTableView* tblChat = nullptr;
    QLineEdit* lnChat = nullptr;
public:
    //не оч хорошо, тк два указателя ссылаются на один объект
    void setPtrTableChat(QTableView* pt){tblChat = pt;}
    void setPtrLineChat(QLineEdit* ln){lnChat = ln;}

};

#endif // MESSEGECONTROLLER_H
