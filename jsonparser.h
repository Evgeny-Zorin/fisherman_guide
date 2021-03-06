#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QObject>
#include <QNetworkReply>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "weather.h"

class jsonparser : public QObject
{
    Q_OBJECT
public:
    explicit jsonparser(QObject *parent = nullptr);

signals:
public:
    void saveToDisk(QWidget* wgt, QNetworkReply *reply);
    bool parsWeather(QNetworkReply *reply, weather* wtr);

};

#endif // JSONPARSER_H
