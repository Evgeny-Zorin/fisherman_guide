#ifndef CLASSFISH_H
#define CLASSFISH_H
#include <QString>


class classfish
{
public:
    classfish();

private:
    QString namefish;
    qint32 mintemp;
    qint32 maxtemp;
    QString area;
    QString type;
};

#endif // CLASSFISH_H
