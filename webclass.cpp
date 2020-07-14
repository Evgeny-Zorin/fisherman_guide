#include "webclass.h"

WebClass::WebClass(QObject *parent) : QObject(parent)
{

}

void WebClass::calledFromJs()
{
      QMessageBox::information(nullptr, "calledFromJs","I'm called by js!");
}
