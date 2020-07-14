#ifndef WEBCLASS_H
#define WEBCLASS_H

#include <QObject>
#include <QWebChannel>
#include <QMessageBox>
#include <QVariant>
#include <QJsonArray>
#include <QBitArray>
#include <QJsonValue>

class WebClass : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString someattribute MEMBER m_someattribute)
public:
    explicit WebClass(QObject *parent = nullptr);

public slots:
    void calledFromJs();
    Q_INVOKABLE void getCoordsFromScript(const QJsonValue &_qJsonVal);

private:
        QString m_someattribute;
        double lat;
        double lon;
};

#endif // WEBCLASS_H
