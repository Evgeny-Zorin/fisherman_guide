#include "jsonparser.h"

jsonparser::jsonparser(QObject *parent) : QObject(parent)
{

}

bool jsonparser::parsWeather(QNetworkReply *reply, weather  *wtr)
{
    // Если ошибки отсутсвуют
    if(!reply->error()){
        // То создаём объект Json Document, считав в него все данные из ответа
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        // Забираем из документа корневой объект
        QJsonObject root = document.object();

        QJsonValue jvcoords = root.value("coord");
            if(jvcoords.isObject()){
                 QJsonObject jObjcoords = jvcoords.toObject();
                 wtr->setcoordLon(jObjcoords.value("lon").toDouble());
                 wtr->setcoordLat(jObjcoords.value("lat").toDouble());
            }
        QJsonValue jvmain = root.value("main");
            if(jvmain.isObject()){
               QJsonObject jObjmain = jvmain.toObject();
//qDebug() << jObjmain;
                wtr->setmainTemp(jObjmain.value("temp").toDouble());
                wtr->setmainHumidity(jObjmain.value("humidity").toInt());
                wtr->setmainPressure(jObjmain.value("pressure").toDouble());
                wtr->setmainGrnd_level(jObjmain.value("grnd_level").toDouble());
                wtr->setmainHumidity(jObjmain.value("humidity").toInt());
                //qDebug() << "wtr->setmainTemp: "<<wtr->getmainTemp();
            }
        QJsonValue jvweather = root.value("weather");
            if(jvweather.isArray()){
                QJsonArray  jArrweather = jvweather.toArray();
                for(int i = 0; i < jArrweather.count(); i++){
                    //qDebug() << "jArrweather.count() "<< jArrweather.count();
                    QJsonObject subtree =jArrweather.at(i).toObject();
                    //qDebug() << subtree.value(subtree.keys().at(i)).toString();
                    //if(subtree.value(subtree.keys().at(i)).toString() == "description")
                        wtr->setweatherDescription(subtree.value(subtree.keys().at(i)).toString());
                        wtr->setweatherMain(subtree.value(subtree.keys().at(i)).toString());
                    //qDebug() << wtr->getweatherDescription();
                }
            }
        QJsonValue jvname = root.value("name");
            if(!jvname.isNull()){
                wtr->setnameCity(jvname.toString());
                //qDebug()<< "wtr->setnameCity: "<< wtr->getnameCity();
            }
        QJsonValue jvsys = root.value("sys");
//qDebug()<<jvsys;
            if(jvsys.isObject()){
                QJsonObject jObjcountry = jvsys.toObject();
                wtr->setsysCountry(jObjcountry.value("country").toString());
                //qDebug()<<"country: "<<wtr->getsysCountry();
                //need add description
            }
            return true;
    }
    reply->deleteLater();
    return false;
}

void jsonparser::saveToDisk(QWidget* wgt, QNetworkReply *reply)
{
    // Если ошибки отсутсвуют
    if(!reply->error()){
    qDebug() << reply->error();
    QByteArray bytes = reply->readAll();
    QJsonDocument jsonDocument(QJsonDocument::fromJson(bytes));
    QString saveFileName = QFileDialog::getSaveFileName(wgt,
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
