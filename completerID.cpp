#include "completerID.h"
#include <QCompleter>
#include <QDebug>

completerID::completerID(QObject *parent) : QObject(parent)
{
}

completerID::~completerID()
{
}

void completerID::mysetModel(QAbstractItemModel *model)
{
    comp.setModel(model);
    comp.setCompletionColumn(1);
//устанавливаем опцию поиска без учета регистра
    comp.setCaseSensitivity(Qt::CaseInsensitive);
}

QCompleter* completerID::getCompl()
{
    return &comp;
}
