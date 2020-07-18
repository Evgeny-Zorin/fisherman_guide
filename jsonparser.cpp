#include "jsonparser.h"

jsonparser::jsonparser(QObject *parent) : QObject(parent)
{

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

void jsonparser::parsWeather(QNetworkReply *reply, weather  *wtr)
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
            jv.toObject();
           qDebug() << jv.toObject();
           qDebug() << jv.toObject().value("temp");
            //то забираем массив из данного свойства
//             QJsonArray ja = jv.toArray();
//             qDebug() << ja;
//             // Перебирая все элементы массива
//             for(int i = 0; i < ja.count(); i++){
//                 QJsonObject subtree = ja.at(i).toObject();
//            qDebug() << "value:" << subtree.value("temp").toString();
//             }


    }
    reply->deleteLater();
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
