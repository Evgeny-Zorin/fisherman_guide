#include "webclass.h"
#include <QJsonObject>

WebClass::WebClass(QObject *parent) : QObject(parent)
{

}

void WebClass::calledFromJs()
{
    //QMessageBox::information(nullptr, "calledFromJs","I'm called by js!");
}

void WebClass::getCoordsFromScript(const QJsonValue &_qJsonVal)
{
     QJsonValue jv = _qJsonVal;
     // Если значение является массивом, ...
            if(jv.isArray()){
                // ... то забираем массив из данного свойства
                QJsonArray ja = jv.toArray();
                qDebug()<<" ja"<<ja;
                qDebug()<<ja[0].toDouble()<<ja[1].toDouble();
                lat = ja[0].toDouble();
                lon = ja[1].toDouble();
            }
}

