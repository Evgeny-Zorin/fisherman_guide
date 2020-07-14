#ifndef WEBCLASS_H
#define WEBCLASS_H

#include <QObject>
#include <QWebChannel>
#include <QMessageBox>

class WebClass : public QObject
{
    Q_OBJECT
public:
    explicit WebClass(QObject *parent = nullptr);
public slots:
    void calledFromJs();
};

#endif // WEBCLASS_H
