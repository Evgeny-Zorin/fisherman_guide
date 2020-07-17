#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QObject>

class jsonparser : public QObject
{
    Q_OBJECT
public:
    explicit jsonparser(QObject *parent = nullptr);

signals:

};

#endif // JSONPARSER_H
