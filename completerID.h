#ifndef COMPLETERID_H
#define COMPLETERID_H

#include <QWidget>
#include <QCompleter>

class completerID : public QObject {
    Q_OBJECT

public:
    explicit completerID( QObject* parent = 0 );
    ~completerID();

private:
    QCompleter comp;
public:
    void mysetModel(QAbstractItemModel *model);
    QCompleter* getCompl();
};

#endif // COMPLETERID_H
