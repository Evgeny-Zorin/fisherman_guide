#ifndef QJAVASCRIPTPROXI_H
#define QJAVASCRIPTPROXI_H

#include <QObject>
#include <QVariant>

class QJavaScriptProxi : public QObject
{
    Q_OBJECT
public:
    explicit QJavaScriptProxi(QObject *parent = nullptr);
    // Expose function to HTML/JS
    Q_INVOKABLE void getCoordsFromScript();
private:
    QVariant coords;

};

#endif // QJAVASCRIPTPROXI_H
