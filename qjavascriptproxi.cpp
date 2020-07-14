#include "qjavascriptproxi.h"

QJavaScriptProxi::QJavaScriptProxi(QObject *parent) : QObject(parent)
{
    //Имя объекта будет использоваться в качестве переменной JS
    setObjectName("coords_QtProxy");
}

void QJavaScriptProxi::getCoordsFromScript()
{

}

