#include "forecast.h"
#include <QDebug>

forecast::forecast(QObject *parent) : QObject(parent)
{

}

void forecast::makeanalysis(const weather *wtr, QSqlTableModel *model)
{
    double localTemp = wtr->getmainTemp();      //температура из прогноза погоды
//идем по циклу и обновляем данные прогноза для всей модели
    for(int i = 0; i < model->rowCount(); i++){
        double min = model->data(model->index(i, 3)).toDouble();    //читаем минимальную температуру клева
        double max = model->data(model->index(i, 4)).toDouble();    //читаем максимальную температуру клева
        double midlgoodtemp = (min + max)/2;    //среднее значение
        double delta = (max - midlgoodtemp)/4;  //используя смещенее температуры создадим прогноз
        //double stage = (min + midlgoodtemp)/4;
        //qDebug() <<"localTemp: "<<localTemp;
        //qDebug() <<"min: "<< min;
        //qDebug() <<"max: "<<max;
        //qDebug() <<"delta: "<<delta;
       //qDebug() << model->data(model->index(i, 3)).toString();  //minactivtemp in DB
       //qDebug() << model->data(model->index(i, 4)).toString();  //maxactivtemp in DB
       //model->setData(model->index(i, 2),"null");
       if (localTemp < min || localTemp > max){
           model->setData(model->index(i, 2),"1");
           continue;
       }
       if ((localTemp >= min && localTemp < min + delta) ||
               (localTemp <= max && localTemp > max - delta)){
           model->setData(model->index(i, 2),"2");
           continue;
       }
       if (((localTemp >= (min + delta)) && (localTemp < (min + 2*delta))) ||
               ((localTemp >= (max - 2*delta)) && (localTemp < (max - delta)))){
           model->setData(model->index(i, 2),"3");
           continue;
       }
       if (((localTemp >= (min + 2*delta)) && (localTemp < (min + 3*delta))) ||
               ((localTemp >= (max - 3*delta)) && (localTemp < (max - 2*delta)))){
           model->setData(model->index(i, 2),"4");
           continue;
       }
       if ((localTemp >= (min + 3*delta)) && (localTemp < (max - 3*delta))){
           model->setData(model->index(i, 2),"5");
           continue;
       }
     }
           //model->submitAll();
}
