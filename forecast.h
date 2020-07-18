#ifndef FORECAST_H
#define FORECAST_H

#include <QObject>
#include <QSqlTableModel>
#include "weather.h"

class forecast : public QObject
{
    Q_OBJECT
public:
    explicit forecast(QObject *parent = nullptr);

public:
    void makeanalysis(const weather* wtr, QSqlTableModel* model);

};

#endif // FORECAST_H
